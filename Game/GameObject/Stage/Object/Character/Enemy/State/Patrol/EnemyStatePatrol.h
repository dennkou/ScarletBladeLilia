#pragma once
#include "./../EnemyState.h"

class Enemy::EnemyStatePatrol : public Enemy::EnemyState
{
public:
	EnemyStatePatrol(Enemy* enemy, NavigationAI* navigationAI);
	virtual ~EnemyStatePatrol();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;
	virtual void OnDamage(int damage) override;

private:
	static constexpr float SPEED = 0.01f;		//	‘¬“x‚¾‚æ™
	static constexpr float FRICTION = 0.05f;		//	–€C‚¾‚æ™
	static constexpr float SEARCH_DISTANCE = 50.0f;
	static constexpr float SEARCH_ANGLE = 60.0f;

	Enemy* m_enemy;
	NavigationAI* m_navigationAI;
	DirectX::XMFLOAT3 m_vector;
};