#pragma once
//	デバックモードの設定だよ☆
#ifdef _DEBUG

//	ログ表示系☆
//#define DISPLAY_LOG		//	ログを表示するよ☆
#ifdef DISPLAY_LOG
#include <unordered_map>
#define IGNORE_UPDATE_EVENT	//	アップデートイベントのログを表示しないよ☆
#endif // DISPLAY_LOG



#endif // _DEBUG

#include <vector>
#include <memory>
#include "./DesignPatterns/FiniteStateMachine.h"
#include <iostream>
#include <chrono>
#include <mutex>




#include "./Timer.h"



#ifndef GAME
#define GAME
/// <summary>
/// <para>このクラスにGameObjectを継承したクラスを登録してゲームを組み立ていくよ☆</para>
/// <para>各ゲームオブジェクトはイベントを介して連携していくよ☆</para>
/// </summary>
class Game
{
public:
	// ゲームオブジェクトの位置を表す型だよ☆
	using ObjectIndex = unsigned int;

	// ゲーム内で使用するクラス☆
	class GameObject;
	Game();
	~Game();

	// 更新の実行☆
	void Update();

	//	ゲームが終了する場面かを示すフラグを返すよ☆
	inline bool GetGameEndFlag() const noexcept { return m_endFlag; }

private:
	
	void GameEnd();

	// イベントを実行するよ☆
	template<class... Argument>
	void EventTrigger(void(GameObject::*handler)(Argument...), Argument... argument)
	{
		//	存在しないオブジェクトを削除しながら全てのオブジェクトのイベントを呼ぶよ☆

		unsigned int moveNum = 0;	//	存在してなかったオブジェクトの数だよ☆

		ObjectIndex i = 0;

		//	指定された関数の呼び出し☆　実行によりサイズが変更するかも知れないからsize関数を使用するよ☆
		static const ObjectIndex objectNum = static_cast<ObjectIndex>(m_gameObjects.size());
		while (i + moveNum < objectNum)
		{
			m_gameObjects[i] = m_gameObjects[i + moveNum];

			//	オブジェクトが存在するか確認☆
			if (m_gameObjects[i])
			{
				(m_gameObjects[i]->*handler)(argument...);
				++i;
			}
			else
			{
				++moveNum;
			}
		}

		m_gameObjects.erase(m_gameObjects.end() - moveNum, m_gameObjects.end());	//	削除した分要素を減らすよ☆
	}

	//	ゲームオブジェクトを追加するよ☆
	template<class Type, class... Argument>
	GameObject* CreateGameObject(Argument... argument)
	{
		GameObject* ret = new Type(this,argument...);
		m_gameObjects.emplace_back(ret);

		#ifdef DISPLAY_LOG
		{
			m_gameObjectName[ret] = typeid(Type).name();
			std::cout << m_gameObjectName[ret] << "クラスを生成したよ☆" << std::endl;

		}
		#endif // DISPLAY_LOG

		return ret;
	}

	//	ゲームオブジェクトを削除するよ☆
	void DeleteGameObject(GameObject* gameObject);



	std::vector<GameObject*> m_gameObjects;							//	ゲームオブジェクトの配列だよ☆

	#ifdef DISPLAY_LOG
		std::unordered_map<GameObject*, std::string> m_gameObjectName;	//	ゲームオブジェクト名の配列だよ☆

		double m_updateTime;
		unsigned long long m_updateNum;
	#endif // DISPLAY_LOG
	bool m_endFlag;

	Timer m_timer;
};
#endif GAME