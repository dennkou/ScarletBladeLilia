#pragma once

#include <cassert>
#include <unordered_map>
#include <memory>

// =====================================================================
// 
// ～～～～～　　　☆　使い方　☆　　　～～～～～
// 
// 
// 
// Crown::FiniteStateMachine<ステートの切り替えに使用する型名, ステートインターフェース>
// で宣言☆
// 
// 
// 
// <登録する型>RegisterState(識別値, コンストラクタの引数);
// でステートを登録☆
// 
// 
// 
// SetEnterFunction(&ステートインターフェース::ステートの開始時に呼び出す関数名);
// SetExitFunction(&ステートインターフェース::ステートの終了時に呼び出す関数名);
// でステートの開始時、終了時に呼び出す関数を登録できるよ☆（任意）
// 
// 
// 
// ChangeState(識別値);
// で識別値のステートに切り替えができるよ☆
// 
// 
// 
// CollFunction<戻り値>(&ステートインターフェース::呼び出したい関数名, 関数の引数);
// で現在のステートの関数を呼び出せるよ☆
// 
// 
// 
// GetState(識別値);
// でこのステートが保持している実体のポインタを返すよ☆
// 
// 
// 
// ☆☆☆　細かい仕様　☆☆☆
// 
// このクラスは非スレッドセーフだよ☆
// ステート切り替え時にnew/deleteは呼び出さないよ☆
// ChangeStateを呼び出さない限り[現在のステート]は無い状態だよ☆
// インスタンスの管理はこのクラスが行うよ☆
// このクラスが管理しているインスタンスのdeleteは呼ばないでね☆
// RegisterStateは識別値が被った時、古いインスタンスを削除して新しいインスタンスにするよ☆
// SetEnterFunction、SetExitFunctionに引数は渡せないよ☆
// ChangeStateは現在のステートが無くても呼び出せるよ☆
// CollFunctionの戻り値はオーバーロードしない限り必須ではないよ☆
// 
// 
// 
// =====================================================================

namespace Crown
{
	/// <summary>
	/// ステートパターンで使用するステートマシンだよ☆
	/// </summary>
	/// <typeparam name="StateID"> ステートの識別値 </typeparam>
	/// <typeparam name="IState"> ステートのインターフェース </typeparam>
	template<class StateID, class IState>
	class FiniteStateMachine
	{
	public:
		FiniteStateMachine()
			:
			m_currentState(nullptr),
			m_enterFunction(),
			m_exitFunction()
		{
		}

		~FiniteStateMachine()
		{
			if (m_enterFunction)
			{
				(m_currentState->*m_exitFunction)();
			}
		}
		
		/// <summary>
		/// 状態を登録するよ☆
		/// </summary>
		/// <typeparam name="State"></typeparam>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="stateID"></param>
		/// <param name="...args"></param>
		template<class IState, class ...Args>
		void RegisterState(StateID stateID, Args... args)
		{
			m_stateTable[stateID].reset(new IState(args...));
		}

		/// <summary>
		/// ステートを切り替えるよ☆
		/// </summary>
		/// <param name="state"></param>
		void ChangeState(StateID key)
		{
			if (m_stateTable.find(key) == m_stateTable.end())
			{
				return;
			}
			m_currentStateID = key;
			if (m_exitFunction && m_currentState != nullptr)
			{
				(m_currentState->*m_exitFunction)();
			}
			IState* next = m_stateTable[key].get();
			if (m_enterFunction)
			{
				(next->*m_enterFunction)();
			}
			m_currentState = next;
		}

		/// <summary>
		/// ステート内の任意の関数を呼び出すよ☆
		/// </summary>
		/// <typeparam name="Return"> 関数が返す値の型だよ☆ </typeparam>
		/// <typeparam name="...Args"> 関数の引数だよ☆ </typeparam>
		/// <param name="collFunction"> 呼び出す関数の関数ポインタだよ☆ </param>
		/// <returns></returns>
		template<class Return, class ...Args>
		Return CallStateFunction(Return(IState::*function)(Args...), Args ...args)
		{
			assert(m_currentState);
			return (m_currentState->*function)(args...);
		}

		/// <summary>
		/// ステート開始時に呼び出される関数の指定だよ☆
		/// </summary>
		/// <param name="stratFunction"></param>
		void SetEnterFunction(void(IState::*enterFunction)())
		{
			m_enterFunction = enterFunction;
		}

		/// <summary>
		/// ステート終了時に呼び出される関数の指定だよ☆
		/// </summary>
		/// <param name="endFunction"></param>
		void SetExitFunction(void(IState::*exitFunction)())
		{
			m_exitFunction = exitFunction;
		}

		inline StateID GetStateID() const noexcept { return m_currentStateID; }

		/// <summary>
		/// 状態を削除するよ☆
		/// </summary>
		/// <param name="stateID"></param>
		void DeleteState(StateID stateID)
		{
			m_stateTable.erase(stateID);
		}
	private:
		void(IState::*m_enterFunction)();									//	開始時に呼び出す関数だよ☆
		void(IState::*m_exitFunction)();									//	終了時に呼び出す関数だよ☆
		std::unordered_map<StateID, std::unique_ptr<IState>> m_stateTable;	//	遷移可能なステートの一覧だよ☆
		IState* m_currentState;												//	現在のステート☆
		StateID m_currentStateID;											//	現在のステートID☆
	};
}