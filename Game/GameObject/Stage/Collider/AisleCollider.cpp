#include "AisleCollider.h"
#include "PlayerCollider.h"
#include "EnemyCollider.h"
#include "./../../MathLibrary.h"

#include <iostream>

ColliderSystem::AisleCollider::AisleCollider()
{
	ColliderSystem::m_instanceCollection.SetAisleCollider(this);
}

ColliderSystem::AisleCollider::~AisleCollider()
{
	ColliderSystem::m_instanceCollection.DeleteAisleCollider(this);
}

void ColliderSystem::AisleCollider::CheckHitPlayer(PlayerCollider* player)
{
	player->SetPosition(CheckHitSphere(player->GetPosition(),player->GetRadius()));
}

void ColliderSystem::AisleCollider::CheckHitEnemy(EnemyCollider* enemy)
{
	enemy->SetPosition(CheckHitSphere(enemy->GetPosition(),enemy->GetCollision().radius));
}

DirectX::XMFLOAT3 ColliderSystem::AisleCollider::CheckHitSphere(const DirectX::XMFLOAT3 spherePosition, float radius)
{
	const float aisleSize = 100.0f;
	const float aisleWidth = 170.0f;
	const float fence = 147.5f;
	const DirectX::XMFLOAT3 pillarPosition = DirectX::XMFLOAT3(97.0f, 0.0f, aisleSize / 2);
	float pillarSize = 14.0f;
	//	プレイヤーとの通路線上の最短地点を算出☆
	const DirectX::XMFLOAT3 closestPoint = ColliderAlgorithm::GetLinePointMinPosition(spherePosition, ColliderAlgorithm::LineSegment(m_position, VectorAdd(m_position, DirectX::XMFLOAT3(sin(m_rotate) * (m_aisleNum * aisleSize), 0, cos(m_rotate) * (m_aisleNum * aisleSize)))));

	//	プレイヤーが通路線上からどれだけ離れているか計算☆
	float distance = VectorDistance(closestPoint, spherePosition);

	//	プレイヤーが近くにいないので当たり判定を実行しない
	if (distance > aisleWidth)
	{
		return spherePosition;
	}

	//	フェンス部分との当たり判定だよ☆
	if (distance > fence - radius)
	{
		return VectorAdd
			(
				VectorScale(VectorNormalize(VectorSub(spherePosition, closestPoint)), (fence - radius)),
				closestPoint
			);
		
	}


	float t = VectorDistance(closestPoint, m_position);
	int index = (t + aisleSize / 2) / aisleSize;
	DirectX::XMFLOAT3 aislePosition = VectorAdd(m_position, DirectX::XMFLOAT3(sin(m_rotate) * (index * aisleSize), 0, cos(m_rotate) * (index * aisleSize)));
	DirectX::XMFLOAT3 ret;
	auto CheckHitPillar = [&](DirectX::XMFLOAT3 position)
		{
			DirectX::XMFLOAT3 localPillarPosition = DirectX::XMFLOAT3(position.x * cos(-m_rotate) - position.z * sin(-m_rotate), position.y, position.x * sin(-m_rotate) + position.z * cos(-m_rotate));
			DirectX::XMFLOAT3 worldPillarPosition = VectorAdd(aislePosition, localPillarPosition);
			float extrusion = (pillarSize + radius) - VectorDistance(worldPillarPosition, spherePosition);
			if (extrusion > 0.0f)
			{
				DirectX::XMFLOAT3 extrusionVector = VectorScale(VectorNormalize(VectorSub(spherePosition, worldPillarPosition)), extrusion);
				ret = VectorAdd(spherePosition, extrusionVector);
				return true;
			}
			else
			{
				return false;
			}
		};
	if (CheckHitPillar(pillarPosition))
	{
		return ret;
	}
	if (CheckHitPillar(VectorScale(pillarPosition, -1)))
	{
		return ret;
	}
	if (CheckHitPillar(DirectX::XMFLOAT3(pillarPosition.x * -1, pillarPosition.y, pillarPosition.z)))
	{
		return ret;
	}
	if (CheckHitPillar(DirectX::XMFLOAT3(pillarPosition.x, pillarPosition.y, pillarPosition.z * -1)))
	{
		return ret;
	}
	return spherePosition;
}