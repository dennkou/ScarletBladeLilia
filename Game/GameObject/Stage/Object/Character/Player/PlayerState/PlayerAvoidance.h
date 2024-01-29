#pragma once
#include "./PlayerState.h"
class Player::PlayerAvoidance : public Player::PlayerState
{
public:
	PlayerAvoidance(Player* player);
	virtual ~PlayerAvoidance();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	Player* m_player;
};