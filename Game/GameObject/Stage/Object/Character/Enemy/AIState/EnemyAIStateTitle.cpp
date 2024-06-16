#include "EnemyAIStateTitle.h"

Enemy::EnemyAIStateTitle::EnemyAIStateTitle(Enemy* enemy)
	:
	m_enemy(enemy)
{
}

Enemy::EnemyAIStateTitle::~EnemyAIStateTitle()
{
}

void Enemy::EnemyAIStateTitle::Enter()
{
}

void Enemy::EnemyAIStateTitle::Update(float time)
{
	time;
}

void Enemy::EnemyAIStateTitle::Exit()
{
}

void Enemy::EnemyAIStateTitle::OnDamage(float damage)
{
	damage;
}

void Enemy::EnemyAIStateTitle::OnPlayStart()
{
	m_enemy->m_aiState.ChangeState(AIState::Patrol);
}