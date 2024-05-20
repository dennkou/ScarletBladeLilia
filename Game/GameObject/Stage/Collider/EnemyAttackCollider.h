#pragma once
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include <initializer_list>
#include <vector>

#ifdef _DEBUG
#define DEBUG_MODEL
#endif // _DEBUG

#ifdef DEBUG_MODEL
#include "./../../Crown/Object/RenderSystem/Model/Model.h"
#endif // DEBUG_MODEL

class ColliderSystem::EnemyAttackCollider
{
public:
	EnemyAttackCollider(float damage, std::initializer_list<ColliderAlgorithm::Triangle> collider);
	virtual ~EnemyAttackCollider();

	void CheckHitPlayer(PlayerCollider* playerCollider);
	void SetWorld(DirectX::XMMATRIX& world);

	void SetActive(bool active);
private:
	bool m_active;
	DirectX::XMMATRIX m_world;
	std::vector<ColliderAlgorithm::Triangle> m_collider;
	float m_damage;
	std::vector<PlayerCollider*> m_hitPlayerList;

#ifdef DEBUG_MODEL
	Crown::RenderObject::Model m_debugModel;
#endif // DEBUG_MODEL
};