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

class ColliderSystem::PlayerAttackCollider
{
public:
	PlayerAttackCollider() = default;
	PlayerAttackCollider(int damage, std::initializer_list<ColliderAlgorithm::Triangle> collider);
	virtual ~PlayerAttackCollider() = default;

	void CheckHitEnemy(EnemyCollider* enemyCollider);

	void SetPlayerWorld(DirectX::XMMATRIX& playerWorld);

	void SetActive(bool active);
private:
	bool m_active;
	DirectX::XMMATRIX m_playerWorld;
	std::vector<ColliderAlgorithm::Triangle> m_collider;
	int m_damage;
	std::vector<EnemyCollider*> m_hitEnemyList;

#ifdef DEBUG_MODEL
	Crown::RenderObject::Model m_debugModel;
#endif // DEBUG_MODEL
};