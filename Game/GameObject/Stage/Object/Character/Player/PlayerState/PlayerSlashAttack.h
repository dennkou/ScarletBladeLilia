#pragma once
#include "PlayerState.h"
#include "./../../../../Collider/PlayerAttackCollider.h"

class Player::PlayerSlashAttack : public Player::PlayerState
{
public:
	PlayerSlashAttack(Player* player);
	virtual ~PlayerSlashAttack();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	//	入力系☆
	virtual void OnInputMove(DirectX::XMFLOAT2 input) { input; }	//	移動の入力操作があったよ☆
	virtual void OnInputAttackDown() {}								//	攻撃ボタンが押されたよ☆
	virtual void OnInputAvoidanceDown() {}							//	回避ボタンが押されたよ☆
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) { input; }	//	カメラの入力があったよ☆
	virtual void OnInputDash(bool input) { input; }
	virtual void OnPlayStartAnimation() {};							//	プレイ開始が選択されたよ☆
	virtual void OnPlayStart() {};									//	ゲームプレイが開始されたよ☆
private:
	static constexpr float ROTATE_START_FLAME = 0;								//	
	static constexpr float ROTATE_END_FLAME = 5;								//	
	static constexpr float ROTATE_SPEED = DirectX::XMConvertToRadians(10.0f);	//	
	static constexpr float ATTACK_START_FLAME = 10;								//	
	static constexpr float ATTACK_END_FLAME = 13;								//	
	static constexpr float MOVING_DISTANCE = 6.0f;								//	
	static constexpr float MOVE_END = 60.0f;									//	
	static constexpr float END = 70.0f;											//	

	Player* m_player;
	float m_animTimer;

	ColliderSystem::PlayerAttackCollider m_attackCollider;
};