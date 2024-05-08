#include "EnemyAIStateCombat.h"
#include "CombatAction/CombatActionState.h"
#include "CombatAction/CombatVigilance.h"
#include "CombatAction/CombatAttack.h"
#include "CombatAction/CombatAttack1.h"

Enemy::EnemyAIStateCombat::EnemyAIStateCombat(Enemy* enemy)
	:
	m_enemy(enemy)
{
	m_state.RegisterState<Vigilance>(StateType::Vigilance, this);
	m_state.RegisterState<Attack>(StateType::Attack, this);
	m_state.RegisterState<Attack1>(StateType::Attack1, this);
	m_state.SetEnterFunction(&State::Enter);
	m_state.SetExitFunction(&State::Exit);

	m_state.ChangeState(StateType::Vigilance);
}

Enemy::EnemyAIStateCombat::~EnemyAIStateCombat()
{

}

void Enemy::EnemyAIStateCombat::Enter()
{
	 
}

void Enemy::EnemyAIStateCombat::Update(float time)
{
	m_state.CallStateFunction(&State::Update, time);
}

void Enemy::EnemyAIStateCombat::Exit()
{

}

void Enemy::EnemyAIStateCombat::OnDamage(int damage)
{
	damage;
}