#pragma once
#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "./../Game.h"
#include <DirectXMath.h>

class Enemy;

//	�C�x���g�����p�̑O���錾���恙
class Timer;

class Game::GameObject
{
public:
	GameObject(Game* game) : m_game(game) {}
	virtual ~GameObject() { m_game->DeleteGameObject(this); }


	//	�C�x���g�̒�`���恙
	//	�V�X�e���n��
	virtual void OnGameInitialize() {};								//	�Q�[�����N�������恙
	virtual void OnGameFinalize() {}								//	�Q�[�����I������恙
	virtual void OnGameUpdate(Timer& timer) { timer; }				//	�X�V��

	//	���͌n��
	virtual void OnInputMenu() {}									//	���j���[�{�^���������ꂽ�恙

	//	�Q�[���v���C��
	virtual void OnInputMove(DirectX::XMFLOAT2 input) { input; }	//	�ړ��̓��͂��ς�����恙
	virtual void OnInputAttackDown() {}								//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() {}							//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) { input; }	//	�J���������삳�ꂽ�恙
	virtual void OnInputDash(bool input) { input; }					//	�_�b�V���{�^���������ꂽ&�����ꂽ�恙

	//	���j���[��
	virtual void OnInputSelect() {}
	virtual void OnInputCursorUp() {}
	virtual void OnInputCursorDown() {}
	virtual void OnInputCursorLeft() {}
	virtual void OnInputCursorRight() {}
	virtual void OnInputCursorMove(DirectX::XMFLOAT2 input) { input; }

	//	�V�[���J�ڌn��
	virtual void OnPlayStartAnimation() {}							//	�^�C�g������v���C���J�n�����恙
	virtual void OnPlayStart() {}									//	�Q�[���v���C���J�n��
	virtual void OnPlayEnd() {}										//	�Q�[���v���C���I����
	virtual void OnPlayRestart() {}									//	�Q�[���v���C���ĊJ��

	//	�v���C�n��
	virtual void OnSlowUIOperation() {}								//	�Q�[���S�̂��X���[���[�V�����ɂ���ׂ�UI�����삳�ꂽ�恙
	virtual void OnStopUIOperation() {}								//	�Q�[���S�̂��~����ׂ�UI�����삳�ꂽ�恙
	virtual void OnStartUIOperation() {}							//	


	virtual void OnPlayerDied()	{}
	virtual void OnEnemyDied(Enemy* diedEnemy) { diedEnemy; }		//	
	virtual void OnPlayerLockon() {}
	virtual void OnEnemyLockon() {}
	virtual void OnEnemyCreate() {}

	//	�Q�[���I�u�W�F�N�g��ǉ�����恙
	template<class Type, class... Argument>
	Type* CreateGameObject(Argument... argument)
	{
		return m_game->CreateGameObject<Type, Argument...>(argument...);
	}

	//	�C�x���g�����s����恙
	template<class... Argument>
	void EventTrigger(void (GameObject::* handler)(Argument...), Argument... argument)
	{
		m_game->EventTrigger(handler, argument...);
	}
protected:


	void GameEnd()
	{
		m_game->GameEnd();
	}
private:
	Game* m_game;
};
#endif GAME_OBJECT