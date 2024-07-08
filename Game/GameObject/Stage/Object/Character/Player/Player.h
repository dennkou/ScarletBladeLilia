#pragma once
#include "./../../Game/GameObject/GameObject.h"
#include "./PlayerCamera.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"
#include "./../../../Collider/PlayerCollider.h"
#include "./UI/PlayerHpUi.h"
#include "./../HP.h"
#include "PlayerModel.h"

class Player : public Game::GameObject
{
public:
	//	�v���C���[�����J����p�����[�^���恙
	static constexpr DirectX::XMFLOAT3 HEAD_OFFSET = DirectX::XMFLOAT3(0,15,0);

	Player(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate);
	virtual ~Player() override;

	virtual void OnGameUpdate(Timer& timer) override;
	virtual void OnPlayStartAnimation() override;
	virtual void OnPlayStart() override;
	virtual void OnPlayRestart() override;

	//	���͌n��
	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	�ړ��̓��͂��ς�����恙
	virtual void OnInputAttackDown() override;						//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() override;					//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;	//	
	virtual void OnInputDash(bool input) override;
private:
	void StateSetUp();
	void ColliderUpdate();

	//	���͂ɂ��J��������]������恙
	void CameraRoll(DirectX::XMFLOAT2 input);
	void Hit(float damage);

	//	�v���C���[�̃X�e�[�^�X���恙
	static constexpr int MAX_HP = 1000;								//	�v���C���[�̍ő�HP���恙
	static constexpr float WALK_ACCELERATION = 0.2f;
	static constexpr float WALK_DECELERATION = 0.985f;
	static constexpr float WALK_SPEED = 0.01f;
	static constexpr float RUN_SPEED = 0.1f;

	DirectX::XMFLOAT3 m_startPosition;
	DirectX::XMFLOAT3 m_startRotate;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotate;
	PlayerCamera m_camera;
	ColliderSystem::PlayerCollider m_collider;
	HP m_hp;
	PlayerHpUi m_hpUi;
	PlayerModel m_model;
	DirectX::XMFLOAT2 m_inputMove;									//	�ړ������̓��͂��恙
	DirectX::XMFLOAT3 m_velocity;

	class PlayerState;
	class PlayerTitle;
	class PlayerStand;
	class PlayerWalk;
	class PlayerRun;
	class PlayerAttack;
	class PlayerSlashAttack;
	class PlayerAvoidance;
	class PlayerDied;
	enum class StateID
	{
		Title,
		Stand,
		Walk,
		Run,
		Attack,
		SlashAttack,
		Avoidance,
		Died,
	};
	Crown::FiniteStateMachine<StateID, PlayerState> m_playerState;	//	��Ԃ������X�e�[�g�}�V�����恙
};