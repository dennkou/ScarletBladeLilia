#pragma once

#include <cassert>
#include <unordered_map>
#include <memory>

// =====================================================================
// 
// �`�`�`�`�`�@�@�@���@�g�����@���@�@�@�`�`�`�`�`
// 
// 
// 
// Crown::FiniteStateMachine<�X�e�[�g�̐؂�ւ��Ɏg�p����^��, �X�e�[�g�C���^�[�t�F�[�X>
// �Ő錾��
// 
// 
// 
// <�o�^����^>RegisterState(���ʒl, �R���X�g���N�^�̈���);
// �ŃX�e�[�g��o�^��
// 
// 
// 
// SetEnterFunction(&�X�e�[�g�C���^�[�t�F�[�X::�X�e�[�g�̊J�n���ɌĂяo���֐���);
// SetExitFunction(&�X�e�[�g�C���^�[�t�F�[�X::�X�e�[�g�̏I�����ɌĂяo���֐���);
// �ŃX�e�[�g�̊J�n���A�I�����ɌĂяo���֐���o�^�ł���恙�i�C�Ӂj
// 
// 
// 
// ChangeState(���ʒl);
// �Ŏ��ʒl�̃X�e�[�g�ɐ؂�ւ����ł���恙
// 
// 
// 
// CollFunction<�߂�l>(&�X�e�[�g�C���^�[�t�F�[�X::�Ăяo�������֐���, �֐��̈���);
// �Ō��݂̃X�e�[�g�̊֐����Ăяo����恙
// 
// 
// 
// GetState(���ʒl);
// �ł��̃X�e�[�g���ێ����Ă�����̂̃|�C���^��Ԃ��恙
// 
// 
// 
// �������@�ׂ����d�l�@������
// 
// ���̃N���X�͔�X���b�h�Z�[�t���恙
// �X�e�[�g�؂�ւ�����new/delete�͌Ăяo���Ȃ��恙
// ChangeState���Ăяo���Ȃ�����[���݂̃X�e�[�g]�͖�����Ԃ��恙
// �C���X�^���X�̊Ǘ��͂��̃N���X���s���恙
// ���̃N���X���Ǘ����Ă���C���X�^���X��delete�͌Ă΂Ȃ��łˁ�
// RegisterState�͎��ʒl����������A�Â��C���X�^���X���폜���ĐV�����C���X�^���X�ɂ���恙
// SetEnterFunction�ASetExitFunction�Ɉ����͓n���Ȃ��恙
// ChangeState�͌��݂̃X�e�[�g�������Ă��Ăяo����恙
// CollFunction�̖߂�l�̓I�[�o�[���[�h���Ȃ�����K�{�ł͂Ȃ��恙
// 
// 
// 
// =====================================================================

namespace Crown
{
	/// <summary>
	/// �X�e�[�g�p�^�[���Ŏg�p����X�e�[�g�}�V�����恙
	/// </summary>
	/// <typeparam name="StateID"> �X�e�[�g�̎��ʒl </typeparam>
	/// <typeparam name="IState"> �X�e�[�g�̃C���^�[�t�F�[�X </typeparam>
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
		/// ��Ԃ�o�^����恙
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
		/// �X�e�[�g��؂�ւ���恙
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
		/// �X�e�[�g���̔C�ӂ̊֐����Ăяo���恙
		/// </summary>
		/// <typeparam name="Return"> �֐����Ԃ��l�̌^���恙 </typeparam>
		/// <typeparam name="...Args"> �֐��̈������恙 </typeparam>
		/// <param name="collFunction"> �Ăяo���֐��̊֐��|�C���^���恙 </param>
		/// <returns></returns>
		template<class Return, class ...Args>
		Return CallStateFunction(Return(IState::*function)(Args...), Args ...args)
		{
			assert(m_currentState);
			return (m_currentState->*function)(args...);
		}

		/// <summary>
		/// �X�e�[�g�J�n���ɌĂяo�����֐��̎w�肾�恙
		/// </summary>
		/// <param name="stratFunction"></param>
		void SetEnterFunction(void(IState::*enterFunction)())
		{
			m_enterFunction = enterFunction;
		}

		/// <summary>
		/// �X�e�[�g�I�����ɌĂяo�����֐��̎w�肾�恙
		/// </summary>
		/// <param name="endFunction"></param>
		void SetExitFunction(void(IState::*exitFunction)())
		{
			m_exitFunction = exitFunction;
		}

		inline StateID GetStateID() const noexcept { return m_currentStateID; }

		/// <summary>
		/// ��Ԃ��폜����恙
		/// </summary>
		/// <param name="stateID"></param>
		void DeleteState(StateID stateID)
		{
			m_stateTable.erase(stateID);
		}
	private:
		void(IState::*m_enterFunction)();									//	�J�n���ɌĂяo���֐����恙
		void(IState::*m_exitFunction)();									//	�I�����ɌĂяo���֐����恙
		std::unordered_map<StateID, std::unique_ptr<IState>> m_stateTable;	//	�J�ډ\�ȃX�e�[�g�̈ꗗ���恙
		IState* m_currentState;												//	���݂̃X�e�[�g��
		StateID m_currentStateID;											//	���݂̃X�e�[�gID��
	};
}