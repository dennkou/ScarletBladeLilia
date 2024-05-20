#pragma once
#include "PlayerState.h"

class Player::PlayerDied : public Player::PlayerState
{
public:
	PlayerDied(Player* player);
	virtual ~PlayerDied();

	virtual void Enter();
	virtual void Exit();
	virtual void Update(float time);
private:
	Player* m_player;
};