#pragma once
#include "PlayerState.h"

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
	Player* m_player;
	float m_animTimer;

	static constexpr float MOVING_DISTANCE = 6.0f;
};