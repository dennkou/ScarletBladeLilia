#include "EnemyCollider.h"
#include "InstanceCollection.h"
#include "./../../MathLibrary.h"


ColliderSystem::EnemyCollider::EnemyCollider(std::function<void(float)> hit, std::function<void(void)> wallHit)
	:
	m_hit(hit),
	m_wallHit(wallHit)
{
	m_instanceCollection.SetEnemyCollider(this);
}

ColliderSystem::EnemyCollider::~EnemyCollider()
{
	m_instanceCollection.DeleteEnemyCollider(this);
}

void ColliderSystem::EnemyCollider::Hit(float damage)
{
	m_hit(damage);
}

void ColliderSystem::EnemyCollider::HitWall()
{
	m_wallHit();
}

void ColliderSystem::EnemyCollider::SetPosition(DirectX::XMFLOAT3 position) noexcept
{
	m_sphere.point = position;
}

void ColliderSystem::EnemyCollider::SetActive(bool active)
{
	if (m_isActive == active)
	{
		return;
	}

	m_isActive = active;
	if (active)
	{
		m_instanceCollection.SetEnemyCollider(this);
	}
	else
	{
		m_instanceCollection.DeleteEnemyCollider(this);
	}
}