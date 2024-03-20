#pragma once
#include "./../EnemyState.h"

class Enemy::EnemyStatePatrol : public Enemy::EnemyState
{
public:
	EnemyStatePatrol(Enemy* enemy, SearchAlgorithm* searchAlgorithm, NavigationAI* navigationAI);
	virtual ~EnemyStatePatrol();

	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;

private:
	Enemy* m_enemy;
	SearchAlgorithm* m_searchAlgorithm;
	NavigationAI* m_navigationAI;
};