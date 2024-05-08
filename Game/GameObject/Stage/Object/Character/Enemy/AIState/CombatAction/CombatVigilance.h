#pragma once
#include "CombatActionState.h"
class Enemy::EnemyAIStateCombat::Vigilance : public Enemy::EnemyAIStateCombat::State
{
public:
	Vigilance(EnemyAIStateCombat* owner);
	~Vigilance();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	EnemyAIStateCombat* m_owner;
	float m_animTimer;
	float m_timer;
	float m_direction;
	static constexpr float MOVE_SPEED = 0.015f;
	static constexpr float DURATION = 1000.0f;
	static constexpr float ROLL_SPEED = 0.01f;
};