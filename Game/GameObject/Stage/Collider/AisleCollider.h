#pragma once
#include <DirectXMath.h>
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include "InstanceCollection.h"

class ColliderSystem::AisleCollider
{
public:
	AisleCollider();
	~AisleCollider();

	void CheckHitPlayer(PlayerCollider* player);
	void CheckHitEnemy(EnemyCollider* enemy);

	inline void SetPosition(DirectX::XMFLOAT3 position) noexcept { m_position = position; }
	void SetRotate(float rotate) noexcept { m_rotate = rotate; }
	void SetAisleNum(unsigned int num) noexcept { m_aisleNum = num; }
private:
	DirectX::XMFLOAT3 CheckHitSphere(const DirectX::XMFLOAT3 spherePosition, float radius);
	DirectX::XMFLOAT3 m_position;
	float m_rotate;
	unsigned int m_aisleNum;
};