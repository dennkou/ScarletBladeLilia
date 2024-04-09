#pragma once
#include "./EnemyState.h"

class Enemy::EnemyStateTitle : public Enemy::EnemyState
{
public:
	EnemyStateTitle(Enemy* enemy);
	~EnemyStateTitle();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;

	virtual void OnPlayStart() override;
private:
	Enemy* m_enemy;
};