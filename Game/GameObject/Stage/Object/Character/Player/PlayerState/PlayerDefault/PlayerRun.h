#pragma once
#include "./PlayerDefaultStateBase.h"
class Player::PlayerDefault::PlayerRun : public Player::PlayerDefault::PlayerDefaultStateBase
{
public:
	PlayerRun(PlayerDefault* owner);
	virtual ~PlayerRun();

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
};	