#pragma once
#include "./../../../../../../../DesignPatterns/FiniteStateMachine.h"
#include "./../EnemyState.h"


class Enemy::EnemyStateCombat : public Enemy::EnemyState
{
public:
	EnemyStateCombat(Enemy* enemy);
	virtual ~EnemyStateCombat();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;
private:
	Enemy* m_enemy;

	class State;
	class StateVigilance;

	enum class StateType
	{
		Vigilance,
	};

	Crown::FiniteStateMachine<StateType, State> m_state;
};