#pragma once
#include "./EnemyAIState.h"

class Enemy::EnemyAIStateTitle : public Enemy::EnemyAIState
{
public:
	EnemyAIStateTitle(Enemy* enemy);
	~EnemyAIStateTitle();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;

	virtual void OnPlayStart() override;
private:
	Enemy* m_enemy;
};