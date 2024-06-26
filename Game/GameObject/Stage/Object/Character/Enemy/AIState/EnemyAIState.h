#pragma once
#include "./../Enemy.h"
class Enemy::EnemyAIState
{
public:
	EnemyAIState() = default;
	virtual ~EnemyAIState() = default;

	virtual void Enter() = 0;
	virtual void Update(float time) = 0;
	virtual void Exit() = 0;
	virtual void OnDamage(float damage) { damage; };
	virtual void OnWallHit() {};

	virtual void OnPlayStart() {};
private:

};