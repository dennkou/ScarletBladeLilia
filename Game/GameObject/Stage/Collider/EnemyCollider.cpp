#include "EnemyCollider.h"
#include "InstanceCollection.h"


ColliderSystem::EnemyCollider::EnemyCollider(std::function<void(int)> hit)
	:
	m_hit(hit)
{
	m_instanceCollection.SetEnemyCollider(this);
}

ColliderSystem::EnemyCollider::~EnemyCollider()
{
	m_instanceCollection.DeleteEnemyCollider(this);
}

void ColliderSystem::EnemyCollider::Hit(int damage)
{
	m_hit(damage);
}

void ColliderSystem::EnemyCollider::SetPosition(DirectX::XMFLOAT3 position) noexcept
{
	m_capsule.line.start.point = position;
	m_capsule.line.end.point = ColliderAlgorithm::VectorAdd(m_capsuleVector, m_capsule.line.start.point);
}

void ColliderSystem::EnemyCollider::SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector) noexcept
{
	m_capsuleVector = capsuleVector;
	m_capsule.line.end.point = ColliderAlgorithm::VectorAdd(m_capsuleVector, m_capsule.line.start.point);
}