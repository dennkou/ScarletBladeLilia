#pragma once
#include "PlayerState.h"

class Player::PlayerWalk : public Player::PlayerState
{
public:
	PlayerWalk(Player* player);
	virtual ~PlayerWalk();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	移動の入力操作があったよ☆
	virtual void OnInputDash(bool input) override;
	virtual void OnInputAttackDown() override;						//	攻撃ボタンが押されたよ☆
	virtual void OnInputAvoidanceDown() override;					//	回避ボタンが押されたよ☆
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;	//	カメラの入力があったよ☆
private:
	static constexpr float ROLL_SPEED = 0.1f;
	Player* m_player;
	float m_animTimer;
	DirectX::XMFLOAT2 m_inputVector;
	bool m_end;
	float m_speed;
};