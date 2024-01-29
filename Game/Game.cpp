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
	m_gameObjectName(),		//	�Q�[���I�u�W�F�N�g�̔z�񂾂恙
	m_updateTime(0),
	m_updateNum(0),
#endif // DISPLAY_LOG
	m_endFlag(false)
{
#ifdef DISPLAY_LOG
	CreateGameObject<DebugLog>(&m_timer);			//	�f�o�b�N���O
#endif // DISPLAY_LOG


	//	�V�X�e���n�I�u�W�F�N�g�̐���������恙
	CreateGameObject<GameInput>();
	CreateGameObject<GameUI>();			//	�Q�[����UI���恙
	CreateGameObject<Stage>();			//	�X�e�[�W�̐���������恙

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
	std::cout << "���σt���[�����[�g��" << 1 / (m_updateTime / m_updateNum / 1000) << "���恙" << std::endl;

	system("Pause");
#endif // DISPLAY_LOG
}

// �X�V�̎��s��
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
		std::cout << m_gameObjectName[gameObject] << "�N���X���폜�����恙" << std::endl;
	}
	#endif // DISPLAY_LOG

	//	�w�肳�ꂽ�Q�[���I�u�W�F�N�g��T����nullptr������恙���̊֐��̓Q�[���I�u�W�F�N�g�̃f�X�g���N�^�ŌĂ΂�邩��폜�͂��Ȃ��恙
	*std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) = nullptr;
}