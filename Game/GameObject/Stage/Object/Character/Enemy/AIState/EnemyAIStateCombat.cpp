#include "EnemyAIStateCombat.h"
#include "CombatAction/CombatActionState.h"
#include "CombatAction/CombatAttack.h"
#include "CombatAction/Approach.h"
#include "CombatAction/WrapAround.h"
#include "CombatAction/ThrustAttack.h"
#include "./../../MathLibrary.h"

Enemy::EnemyAIStateCombat::EnemyAIStateCombat(Enemy* enemy)
	:
	m_enemy(enemy)
{
	m_state.RegisterState<Approach>(StateID::Approach, this);
	m_state.RegisterState<WrapAround>(StateID::WrapAround, this);
	m_state.RegisterState<Attack>(StateID::Attack, this);
	m_state.RegisterState<ThrustAttack>(StateID::ThrustAttack, this);
	m_state.SetEnterFunction(&IState::Enter);
	m_state.SetExitFunction(&IState::Exit);

	m_state.ChangeState(StateID::WrapAround);
}

Enemy::EnemyAIStateCombat::~EnemyAIStateCombat()
{

}

void Enemy::EnemyAIStateCombat::Enter()
{
	 
}

void Enemy::EnemyAIStateCombat::Update(float time)
{
	m_state.CallStateFunction(&IState::Update, time);
}

void Enemy::EnemyAIStateCombat::Exit()
{

}

void Enemy::EnemyAIStateCombat::OnDamage(float damage)
{
	damage;
}

void Enemy::EnemyAIStateCombat::OnWallHit()
{
	m_state.CallStateFunction(&IState::WallHit);
}

void Enemy::EnemyAIStateCombat::ActionBranch()
{
	float distance = Crown::Math::VectorDistance(m_enemy->m_position, m_enemy->m_enemyCollider.GetPlayerPosition());
	int actionNum = ((*m_enemy->m_random.get())()) % (ATTACK + MOVE);
	if (actionNum < ATTACK)
	{
		if (distance < ATTACK_DISTANCE)
		{
			m_state.ChangeState(StateID::Attack);
		}
		else
		{
			m_state.ChangeState(StateID::ThrustAttack);
		}
	}
	else
	{
		if (APPROACH_DISTANCE < Crown::Math::VectorDistance(m_enemy->m_position, m_enemy->m_enemyCollider.GetPlayerPosition()))
		{
			m_state.ChangeState(StateID::Approach);
		}
		else
		{
			m_state.ChangeState(StateID::WrapAround);
		}
	}
}
