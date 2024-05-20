#pragma once
#include "CombatActionState.h"
#include "./../../Game/GameObject/Stage/Collider/EnemyAttackCollider.h"

class Enemy::EnemyAIStateCombat::ThrustAttack : public Enemy::EnemyAIStateCombat::IState
{
public:
	ThrustAttack(EnemyAIStateCombat* owner);
	virtual ~ThrustAttack();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	static constexpr float ATTACK_START_FLAME = 15;
	static constexpr float ATTACK_END_FLAME = 20;
	static constexpr float TRACKING_END_FLAME = 10;
	static constexpr float ATTACK_TRACKING_ROLL = 0.001f;
	EnemyAIStateCombat* m_owner;
	ColliderSystem::EnemyAttackCollider m_attackCollider;
	float m_animTimer;
};