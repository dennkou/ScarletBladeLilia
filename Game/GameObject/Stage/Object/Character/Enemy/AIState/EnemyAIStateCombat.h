#pragma once
#include "./../../../../../../DesignPatterns/FiniteStateMachine.h"
#include "./EnemyAIState.h"


class Enemy::EnemyAIStateCombat : public Enemy::EnemyAIState
{
public:
	EnemyAIStateCombat(Enemy* enemy);
	virtual ~EnemyAIStateCombat();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;
private:
	static constexpr float ATTACK_DISTANCE = 150;

	Enemy* m_enemy;

	class State;
	class Vigilance;
	class Attack;
	class Attack1;

	enum class StateType
	{
		Vigilance,
		Attack,
		Attack1,
	};

	Crown::FiniteStateMachine<StateType, State> m_state;
};