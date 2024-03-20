#include "EnemyStatePatrol.h"



Enemy::EnemyStatePatrol::EnemyStatePatrol(Enemy* enemy, SearchAlgorithm* searchAlgorithm, NavigationAI* navigationAI)
	:
	m_enemy(enemy),
	m_searchAlgorithm(searchAlgorithm),
	m_navigationAI(navigationAI)
{

}

Enemy::EnemyStatePatrol::~EnemyStatePatrol()
{

}

void Enemy::EnemyStatePatrol::Enter()
{

}

void Enemy::EnemyStatePatrol::Update()
{
	if (m_searchAlgorithm->IsEngagement())
	{
		m_enemy->m_state.ChangeState(Enemy::State::Combat);
	}


}

void Enemy::EnemyStatePatrol::Exit()
{

}

void Enemy::EnemyStatePatrol::OnDamage(int damage)
{
}
