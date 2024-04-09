#pragma once
#include "./../EnemyStateCombat.h"

class Enemy::EnemyStateCombat::State
{
public:
	State() = default;
	virtual ~State() = default;

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
private:
};