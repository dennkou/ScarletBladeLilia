#include "EnemyStateTitle.h"

Enemy::EnemyStateTitle::EnemyStateTitle(Enemy* enemy)
	:
	m_enemy(enemy)
{
}

Enemy::EnemyStateTitle::~EnemyStateTitle()
{
}

void Enemy::EnemyStateTitle::Enter()
{
}

void Enemy::EnemyStateTitle::Update(float time)
{
}

void Enemy::EnemyStateTitle::Exit()
{
}

void Enemy::EnemyStateTitle::OnDamage(int damage)
{
}

void Enemy::EnemyStateTitle::OnPlayStart()
{
	m_enemy->m_state.ChangeState(State::Patrol);
}
