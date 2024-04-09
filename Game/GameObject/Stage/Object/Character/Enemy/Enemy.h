#pragma once
#include "./../Character.h"
#include "./../../../Collider/EnemyCollider.h"
#include "./UI/EnemyUi.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"

class NavigationAI
{
public:
	virtual void SetPosition(DirectX::XMFLOAT3 position) = 0;
	virtual DirectX::XMFLOAT3 MovingPosition() = 0;
private:
};

class Enemy : public Character
{
public:
	Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate, NavigationAI* navigationAI);
	virtual ~Enemy();
	virtual void OnGameUpdate(Timer& timer) override;
	virtual void OnPlayStart() override;

	void HitPlayerAttack(int damage);
private:
	static constexpr int MAX_HP = 100;
	ColliderSystem::EnemyCollider m_enemyCollider;

	EnemyUi m_ui;
	static constexpr DirectX::XMFLOAT3 UI_POSITION_OFFSET = DirectX::XMFLOAT3(0, 15, 0);

	class EnemyState;
	class EnemyStateTitle;
	class EnemyStatePatrol;
	class EnemyStateCombat;
	enum class State
	{
		Title,
		Patrol,
		Combat,
	};
	Crown::FiniteStateMachine<State, EnemyState> m_state;
};