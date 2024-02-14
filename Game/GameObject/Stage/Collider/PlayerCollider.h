#pragma once
#include <DirectXMath.h>
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include "InstanceCollection.h"
#include <vector>

class ColliderSystem::PlayerCollider
{
public:
	PlayerCollider();
	~PlayerCollider();

	DirectX::XMFLOAT3 GetPosition() { return m_collider.line.start.point; }
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector);	

	void CheckHitEnemy(EnemyCollider* enemyCollider);
private:

	ColliderAlgorithm::Capsule m_collider;
	DirectX::XMFLOAT3 m_capsuleVector;								//	当たり判定用カプセルコライダーの傾きと距離だよ☆
};