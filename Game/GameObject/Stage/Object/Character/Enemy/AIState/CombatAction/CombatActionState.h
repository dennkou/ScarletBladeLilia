#pragma once
#include "./../EnemyAIStateCombat.h"

class Enemy::EnemyAIStateCombat::State
{
public:
	State() = default;
	virtual ~State() = default;

	virtual void Enter() = 0;
	virtual void Update(float time) = 0;
	virtual void Exit() = 0;
private:

};