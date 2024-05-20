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
	virtual void OnDamage(float damage) override;
private:
	void ActionBranch();

	Enemy* m_enemy;

	class IState;
	class WrapAround;
	class Approach;
	class Attack;
	class ThrustAttack;
	class Shot;

	enum class StateID
	{
		WrapAround,
		Approach,
		Attack,
		ThrustAttack,
		Shot,
	};

	Crown::FiniteStateMachine<StateID, IState> m_state;


	static constexpr float APPROACH_DISTANCE = 100.0f;
	static constexpr float ATTACK_DISTANCE = 110.0f;

	//	各行動の重みパラメータだよ☆
	static constexpr int ATTACK = 4;
	static constexpr int MOVE = 4;
};