#include "PlayerAttackCollider.h"
#include "EnemyCollider.h"
#include "InstanceCollection.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"

ColliderSystem::PlayerAttackCollider::PlayerAttackCollider(float damage, std::initializer_list<ColliderAlgorithm::Triangle> collider)
	:
	m_playerWorld(DirectX::XMMatrixIdentity()),
	m_damage(damage),
	m_collider(collider),
	m_hitEnemyList(),
	m_active(false)

#ifdef DEBUG_MODEL
	,m_debugModel()
#endif // DEBUG_MODEL
{
#ifdef DEBUG_MODEL
	m_debugModel.Create(collider, DirectX::XMFLOAT4(1,0,0,0.5));
	m_debugModel.SetDrawFlag(false);
#endif // DEBUG_MODEL
}

void ColliderSystem::PlayerAttackCollider::CheckHitEnemy(EnemyCollider* enemyCollider)
{
	bool isHit = false;
	std::vector<EnemyCollider*>::iterator it = std::find(m_hitEnemyList.begin(), m_hitEnemyList.end(), enemyCollider);
	bool isAlreadyHitting = it != m_hitEnemyList.end();

	for (ColliderAlgorithm::Triangle triangle : m_collider)
	{
		DirectX::XMMATRIX triangleMatrix;
		DirectX::XMVECTOR scale, rotate, transform;

		//	プレイヤーを基準にしたローカル座標をワールド座標に変更するよ☆
		for (int i = 0; i < 3; ++i)
		{
			triangleMatrix = DirectX::XMMatrixTranslation(triangle.point[i].point.x, triangle.point[i].point.y, triangle.point[i].point.z)
				* m_playerWorld;

			DirectX::XMMatrixDecompose(&scale, &rotate, &transform, triangleMatrix);
			DirectX::XMStoreFloat3(&triangle.point[i].point, transform);
		}

		isHit = ColliderAlgorithm::CheckHitSphereTriangle(enemyCollider->GetCollision(), triangle);
		if (isHit)
		{
			break;
		}
	}

	if (!isAlreadyHitting && isHit)
	{
		enemyCollider->Hit(m_damage);
		m_hitEnemyList.push_back(enemyCollider);
	}
	if (isAlreadyHitting && !isHit)
	{
		m_hitEnemyList.erase(it);
	}
}

void ColliderSystem::PlayerAttackCollider::SetPlayerWorld(DirectX::XMMATRIX& playerWorld)
{
	m_playerWorld = playerWorld;
#ifdef DEBUG_MODEL
	m_debugModel.GetMaterialMesh(0).GetMaterial(Crown::RenderObject::MaterialTag::Normal).GetConstBuffer(0).SetParameter(1,  playerWorld);
	m_debugModel.GetMaterialMesh(0).GetMaterial(Crown::RenderObject::MaterialTag::Normal).GetConstBuffer(0).SetParameter(2,  Crown::RenderObject::Camera::GetInstance()->GetView() * Crown::RenderObject::Camera::GetInstance()->GetProjection());
#endif // DEBUG_MODEL
}

void ColliderSystem::PlayerAttackCollider::SetActive(bool active)
{
	if (m_active != active)
	{
		if (active)
		{
			ColliderSystem::m_instanceCollection.SetPlayerAttackCollider(this);
		}
		else
		{
			ColliderSystem::m_instanceCollection.DeletePlayerAttackCollider(this);
			m_hitEnemyList.clear();
		}
#ifdef DEBUG_MODEL
		m_debugModel.SetDrawFlag(active);
#endif // DEBUG_MODEL
		m_active = active;
	}
}