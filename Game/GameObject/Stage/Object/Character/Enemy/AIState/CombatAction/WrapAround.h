#pragma once
#include "CombatActionState.h"

class Enemy::EnemyAIStateCombat::WrapAround : public Enemy::EnemyAIStateCombat::IState
{
public:
	WrapAround(EnemyAIStateCombat* owner);
	virtual ~WrapAround();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	EnemyAIStateCombat* m_owner;
	float m_animTimer;
	float m_direction;
	float m_timer;
	static constexpr float MOVE_SPEED = 0.015f;
	static constexpr float ROLL_SPEED = 0.0025f;

	static constexpr float BRANCH_TIMING = 1.0f;

	static constexpr int ATTACK = 2;
	static constexpr int CHANGE = 1;
};

