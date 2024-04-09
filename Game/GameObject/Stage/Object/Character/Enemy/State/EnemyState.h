#pragma once
#include "./../Enemy.h"
class Enemy::EnemyState
{
public:
	EnemyState() = default;
	virtual ~EnemyState() = default;

	virtual void Enter() = 0;
	virtual void Update(float time) = 0;
	virtual void Exit() = 0;
	virtual void OnDamage(int damage) = 0;

	virtual void OnPlayStart() {};
private:

};