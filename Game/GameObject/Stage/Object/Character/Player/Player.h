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
	//	プレイヤーが公開するパラメータだよ☆
	static constexpr DirectX::XMFLOAT3 HEAD_OFFSET = DirectX::XMFLOAT3(0,15,0);

	Player(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate);
	virtual ~Player() override;

	virtual void OnGameUpdate(Timer& timer) override;
	virtual void OnPlayStartAnimation() override;
	virtual void OnPlayStart() override;
	virtual void OnPlayRestart() override;

	//	入力系☆
	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	移動の入力が変わったよ☆
	virtual void OnInputAttackDown() override;						//	攻撃ボタンが押されたよ☆
	virtual void OnInputAvoidanceDown() override;					//	回避ボタンが押されたよ☆
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;	//	
	virtual void OnInputDash(bool input) override;
private:
	void StateSetUp();
	void ColliderUpdate();

	//	入力によりカメラを回転させるよ☆
	void CameraRoll(DirectX::XMFLOAT2 input);
	void Hit(float damage);

	//	プレイヤーのステータスだよ☆
	static constexpr int MAX_HP = 1000;								//	プレイヤーの最大HPだよ☆
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
	DirectX::XMFLOAT2 m_inputMove;									//	移動方向の入力だよ☆
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
	Crown::FiniteStateMachine<StateID, PlayerState> m_playerState;	//	状態を示すステートマシンだよ☆
};