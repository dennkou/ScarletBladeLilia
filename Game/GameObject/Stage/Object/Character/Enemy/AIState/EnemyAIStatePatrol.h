#pragma once
#include "./EnemyAIState.h"

class Enemy::EnemyAIStatePatrol : public Enemy::EnemyAIState
{
public:
	EnemyAIStatePatrol(Enemy* enemy, NavigationAI* navigationAI);
	virtual ~EnemyAIStatePatrol();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;

private:
	static constexpr float SPEED = 0.01f;		//	‘¬“x‚¾‚æ™
	static constexpr float FRICTION = 0.8f;		//	–€C‚¾‚æ™
	static constexpr float SEARCH_DISTANCE = 500.0f;
	static constexpr float SEARCH_ANGLE = 60.0f;

	Enemy* m_enemy;
	NavigationAI* m_navigationAI;
	DirectX::XMFLOAT3 m_vector;

	float m_animTimer;
};