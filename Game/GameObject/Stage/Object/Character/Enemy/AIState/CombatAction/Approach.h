#pragma once
#include "CombatActionState.h"

class Enemy::EnemyAIStateCombat::Approach : public Enemy::EnemyAIStateCombat::IState
{
public:
	Approach(EnemyAIStateCombat* owner);
	virtual ~Approach();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	EnemyAIStateCombat* m_owner;
	float m_animTimer;

	bool m_moveFlag;

	static constexpr float MOVE_SPEED = 0.1f;
	static constexpr float ROLL_SPEED = 0.001f;
	static constexpr float STOP_DISTANCE = 100.0f;
	static constexpr float MOVE_START_ANGLE = 15.0f;
};