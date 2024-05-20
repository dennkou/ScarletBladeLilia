#pragma once
#include "CombatActionState.h"
class Enemy::EnemyAIStateCombat::Shot : public Enemy::EnemyAIStateCombat::IState
{
public:
	Shot(EnemyAIStateCombat* owner);
	virtual ~Shot();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
private:
	EnemyAIStateCombat* m_owner;
	bool m_shotFlag;
	float m_timer;

	static constexpr float SHOT_FLAME = 20;
};