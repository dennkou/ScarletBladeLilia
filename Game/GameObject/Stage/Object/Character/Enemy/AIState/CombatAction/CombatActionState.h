#pragma once
#include "./../EnemyAIStateCombat.h"

class Enemy::EnemyAIStateCombat::IState
{
public:
	IState() = default;
	virtual ~IState() = default;

	virtual void Enter() = 0;
	virtual void Update(float time) = 0;
	virtual void Exit() = 0;
private:

};