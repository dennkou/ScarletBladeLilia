#pragma once
#include "CombatActionState.h"
#include "./../../Game/GameObject/Stage/Collider/EnemyAttackCollider.h"

class Enemy::EnemyAIStateCombat::Attack1 : public Enemy::EnemyAIStateCombat::State
{
public:
	Attack1(EnemyAIStateCombat* owner);
	virtual ~Attack1();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	static constexpr float ATTACK_START_FLAME = 56;
	static constexpr float ATTACK_END_FLAME = 59;
	static constexpr float TRACKING_END_FLAME = 40;
	EnemyAIStateCombat* m_owner;
	ColliderSystem::EnemyAttackCollider m_attackCollider;
	float m_animTimer;
};