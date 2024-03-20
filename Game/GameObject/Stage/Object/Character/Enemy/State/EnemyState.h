#pragma once
#include "./../Enemy.h"
class Enemy::EnemyState
{
public:
	EnemyState() = default;
	~EnemyState() = default;

	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void OnDamage(int damage) = 0;
private:

};