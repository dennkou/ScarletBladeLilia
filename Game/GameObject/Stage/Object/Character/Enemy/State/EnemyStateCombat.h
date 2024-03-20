#pragma once
#include "EnemyState.h"

class Enemy::EnemyStateCombat : public Enemy::EnemyState
{
public:
	EnemyStateCombat(Enemy* enemy);
	virtual ~EnemyStateCombat();

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;
private:
	Enemy* m_enemy;
};

