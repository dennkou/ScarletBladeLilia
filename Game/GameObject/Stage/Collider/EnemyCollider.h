#pragma once
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include <functional>

class ColliderSystem::EnemyCollider
{
public:
	EnemyCollider(std::function<void(int)> hit);
	virtual ~EnemyCollider();

	void Hit(int damage);

	void SetPosition(DirectX::XMFLOAT3 position) noexcept;
	void SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector) noexcept;
	DirectX::XMFLOAT3 GetPosition() { return m_capsule.line.start.point; }
	void SetRadius(float radius) noexcept { m_capsule.radius = radius; }
	ColliderAlgorithm::Capsule GetCollision() const noexcept { return m_capsule; }
	void SetPlayerPosition(DirectX::XMFLOAT3 pos) { m_playerPosition = pos; }
	DirectX::XMFLOAT3 GetPlayerPosition() { return m_playerPosition; }
private:
	ColliderAlgorithm::Capsule m_capsule;
	DirectX::XMFLOAT3 m_capsuleVector;
	std::function<void(int)> m_hit;
	DirectX::XMFLOAT3 m_playerPosition;
};