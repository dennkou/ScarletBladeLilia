#pragma once
#include "./PlayerDefaultStateBase.h"

class Player::PlayerDefault::PlayerMove : public Player::PlayerDefault::PlayerDefaultStateBase
{
public:
	PlayerMove(PlayerDefault* owner);
	virtual ~PlayerMove();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	static constexpr float ROLL_SPEED = 0.01f;
	static constexpr float SPEED = 0.01f;
	static constexpr float ACCELERATION = 0.01f;
	static constexpr float DECELERATION = 0.01f;

	Player* m_player;
	PlayerDefault* m_owner;
	DirectX::XMFLOAT3 m_velocity;
};	