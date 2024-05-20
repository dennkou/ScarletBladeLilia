#pragma once
#include <random>
#include "./../../Crown/Object/RenderSystem/Model/Model.h"
#include "./../HP.h"
#include "./../../Game/GameObject/GameObject.h"
#include "./../../../Collider/EnemyCollider.h"
#include "./UI/EnemyUi.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"

class NavigationAI
{
public:
	virtual void SetPosition(DirectX::XMFLOAT3 position) = 0;
	virtual DirectX::XMFLOAT3 MovingPosition() = 0;
private:
};

class Enemy : public Game::GameObject
{
public:
	Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate, NavigationAI* navigationAI, std::shared_ptr<std::mt19937> random);
	virtual ~Enemy();
	virtual void OnGameUpdate(Timer& timer) override;
	virtual void OnPlayStart() override;
private:
	void HitPlayerAttack(float damage);
	void HitWall();

	static constexpr int MAX_HP = 500;

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotate;

	float m_damageAnimTimer;
	DirectX::XMFLOAT3 m_damageAnim;
	static constexpr float DamageAnimLength = 0.3f;

	DirectX::XMFLOAT3 m_offset; //	当たり判定のオフセット位置だよ☆
	ColliderSystem::EnemyCollider m_enemyCollider;

	static constexpr DirectX::XMFLOAT3 BONE_OFFSET = DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f);
	Crown::RenderObject::Model m_model;

	HP m_hp;

	EnemyUi m_ui;
	static constexpr DirectX::XMFLOAT3 UI_POSITION_OFFSET = DirectX::XMFLOAT3(0, 30, 0);

	//	AIのステートだよ☆
	class EnemyAIState;
	class EnemyAIStateTitle;
	class EnemyAIStatePatrol;
	class EnemyAIStateCombat;
	enum class AIState
	{
		Title,
		Patrol,
		Combat,
	};
	Crown::FiniteStateMachine<AIState, EnemyAIState> m_aiState;

	DirectX::XMMATRIX m_bone[255];

	static constexpr float ANIMATION_FPS = 30;						//	アニメーションのフレームレートだよ☆
	Crown::RenderObject::AnimationData m_standAnim;
	Crown::RenderObject::AnimationData m_attackAnim;
	Crown::RenderObject::AnimationData m_attack1Anim;
	Crown::RenderObject::AnimationData m_shotAnim;
	std::shared_ptr<std::mt19937> m_random;
};