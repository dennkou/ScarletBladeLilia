#pragma once
#include <DirectXMath.h>
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include "InstanceCollection.h"
#include <vector>
#include <functional>

class ColliderSystem::PlayerCollider
{
public:
	PlayerCollider(std::function<void(float)> hit);
	virtual ~PlayerCollider();

	DirectX::XMFLOAT3 GetPosition() { return m_collider.line.start.point; }
	float GetRadius() { return m_collider.radius; }
	ColliderAlgorithm::Capsule GetCapsule() { return m_collider; }
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector);	

	void CheckHitEnemy(EnemyCollider* enemyCollider);

	void Hit(float damage);
private:
	std::function<void(float)> m_hit;
	ColliderAlgorithm::Capsule m_collider;
	DirectX::XMFLOAT3 m_capsuleVector;								//	当たり判定用カプセルコライダーの傾きと距離だよ☆
};