#include "InstanceCollection.h"
#include "PlayerCollider.h"
#include "EnemyCollider.h"
#include <math.h>
#include "./../../MathLibrary.h"

ColliderSystem::PlayerCollider::PlayerCollider(std::function<void(float)> hit)
	:
	m_collider(),
	m_hit(hit)
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
	m_collider.line.end.point = VectorAdd(m_capsuleVector, m_collider.line.start.point);
}

void ColliderSystem::PlayerCollider::SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector)
{
	m_capsuleVector = capsuleVector;
	m_collider.line.end.point = VectorAdd(m_capsuleVector, m_collider.line.start.point);
}

void ColliderSystem::PlayerCollider::CheckHitEnemy(EnemyCollider* enemyCollider)
{
	DirectX::XMFLOAT3 latestPoint = ColliderAlgorithm::GetLinePointMinPosition(enemyCollider->GetCollision().point.point, m_collider.line);
	float distance = std::sqrt(VectorSquareSize(VectorSub(latestPoint, enemyCollider->GetCollision().point.point)));

	if (distance <= 0.001f)
	{
		return;
	}

	float extrusionDistance = (enemyCollider->GetCollision().radius + m_collider.radius) - distance;

	if (extrusionDistance > 0)
	{
		DirectX::XMFLOAT3 direction = VectorNormalize(VectorSub(enemyCollider->GetCollision().point.point, latestPoint));
		//	‰Ÿ‚µo‚µ™
		enemyCollider->SetPosition(VectorAdd(enemyCollider->GetPosition(), VectorScale(direction, extrusionDistance / 2)));
		SetPosition(VectorAdd(GetPosition(), VectorScale(direction, extrusionDistance / -2)));
	}
}

void ColliderSystem::PlayerCollider::Hit(float damage)
{
	m_hit(damage);
}
