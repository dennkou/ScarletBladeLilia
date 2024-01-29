#include "Game.h"
#include <algorithm>
#include "./GameObject/GameObject.h"



#include "./GameObject/Input/GameInput.h"
#include "./GameObject/UI/GameUI.h"
#include "./GameObject/Stage/Stage.h"
#include "./Debug/DebugLog.h"

Game::Game()
	:
#ifdef DISPLAY_LOG
	m_gameObjectName(),		//	ゲームオブジェクトの配列だよ☆
	m_updateTime(0),
	m_updateNum(0),
#endif // DISPLAY_LOG
	m_endFlag(false)
{
#ifdef DISPLAY_LOG
	CreateGameObject<DebugLog>(&m_timer);			//	デバックログ
#endif // DISPLAY_LOG


	//	システム系オブジェクトの生成をするよ☆
	CreateGameObject<GameInput>();
	CreateGameObject<GameUI>();			//	ゲームのUIだよ☆
	CreateGameObject<Stage>();			//	ステージの生成をするよ☆

	EventTrigger(&GameObject::OnGameInitialize);
}

Game::~Game()
{
	EventTrigger(&GameObject::OnGameFinalize);
	for (unsigned int i = 0, size = static_cast<unsigned int>(m_gameObjects.size()); i < size; ++i)
	{
		delete m_gameObjects[i];
	}

#ifdef DISPLAY_LOG
	std::cout << "平均フレームレートは" << 1 / (m_updateTime / m_updateNum / 1000) << "だよ☆" << std::endl;

	system("Pause");
#endif // DISPLAY_LOG
}

// 更新の実行☆
void Game::Update()
{
	m_timer.Update();
	EventTrigger<Timer&>(&GameObject::OnGameUpdate, m_timer);
#ifdef DISPLAY_LOG
	m_updateTime += m_timer.GetTime();
	++m_updateNum;
#endif // DISPLAY_LOG
}

void Game::GameEnd()
{
	m_endFlag = true;
}

void Game::DeleteGameObject(GameObject* gameObject)
{
	#ifdef DISPLAY_LOG
	{
		std::cout << m_gameObjectName[gameObject] << "クラスを削除したよ☆" << std::endl;
	}
	#endif // DISPLAY_LOG

	//	指定されたゲームオブジェクトを探してnullptrを入れるよ☆この関数はゲームオブジェクトのデストラクタで呼ばれるから削除はしないよ☆
	*std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) = nullptr;
}