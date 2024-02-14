#include "InstanceCollection.h"
#include "PlayerCollider.h"
#include "EnemyCollider.h"
#include <math.h>

ColliderSystem::PlayerCollider::PlayerCollider()
	:
	m_collider()
{
	m_collider.radius = 3.0f;
	ColliderSystem::m_instanceCollection.SetPlayerCollider(this);
}

ColliderSystem::PlayerCollider::~PlayerCollider()
{
	ColliderSystem::m_instanceCollection.DeletePlayerCollider();
}

void ColliderSystem::PlayerCollider::SetPosition(DirectX::XMFLOAT3 position)
{
	m_collider.line.start.point = position;
	m_collider.line.end.point = ColliderAlgorithm::VectorAdd(m_capsuleVector, m_collider.line.start.point);
}

void ColliderSystem::PlayerCollider::SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector)
{
	m_capsuleVector = capsuleVector;
	m_collider.line.end.point = ColliderAlgorithm::VectorAdd(m_capsuleVector, m_collider.line.start.point);
}

void ColliderSystem::PlayerCollider::CheckHitEnemy(EnemyCollider* enemyCollider)
{
	float distance = std::sqrt(ColliderAlgorithm::SegmentSegmentMinLengthSquare(enemyCollider->GetCollision().line, m_collider.line));

	if (distance <= 0.001f)
	{
		return;
	}

	float extrusionDistance = (enemyCollider->GetCollision().radius + m_collider.radius) - distance;

	if (extrusionDistance > 0)
	{
		DirectX::XMFLOAT3 direction = ColliderAlgorithm::VectorNormalize(ColliderAlgorithm::VectorSub(enemyCollider->GetPosition(), GetPosition()));

		enemyCollider->SetPosition(ColliderAlgorithm::VectorAdd(enemyCollider->GetPosition(), ColliderAlgorithm::VectorScale(direction, extrusionDistance / 2)));
		SetPosition(ColliderAlgorithm::VectorAdd(GetPosition(), ColliderAlgorithm::VectorScale(direction, extrusionDistance / -2)));
	}
}