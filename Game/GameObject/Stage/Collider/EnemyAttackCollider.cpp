#include "EnemyAttackCollider.h"
#include "PlayerCollider.h"
#include "InstanceCollection.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"

ColliderSystem::EnemyAttackCollider::EnemyAttackCollider(float damage, std::initializer_list<ColliderAlgorithm::Triangle> collider)
	:
	m_world(DirectX::XMMatrixIdentity()),
	m_damage(damage),
	m_collider(collider),
	m_hitPlayerList(),
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

ColliderSystem::EnemyAttackCollider::~EnemyAttackCollider()
{

}

void ColliderSystem::EnemyAttackCollider::CheckHitPlayer(PlayerCollider* playerCollider)
{
	bool isHit = false;
	std::vector<PlayerCollider*>::iterator it = std::find(m_hitPlayerList.begin(), m_hitPlayerList.end(), playerCollider);
	bool isAlreadyHitting = it != m_hitPlayerList.end();

	for (ColliderAlgorithm::Triangle triangle : m_collider)
	{
		DirectX::XMMATRIX triangleMatrix;
		DirectX::XMVECTOR scale, rotate, transform;

		//	プレイヤーを基準にしたローカル座標をワールド座標に変更するよ☆
		for (int i = 0; i < 3; ++i)
		{
			triangleMatrix = DirectX::XMMatrixTranslation(triangle.point[i].point.x, triangle.point[i].point.y, triangle.point[i].point.z)
				* m_world;

			DirectX::XMMatrixDecompose(&scale, &rotate, &transform, triangleMatrix);
			DirectX::XMStoreFloat3(&triangle.point[i].point, transform);
		}

		isHit = ColliderAlgorithm::CheckHitCapsuleTriangle(playerCollider->GetCapsule(), triangle);
		if (isHit)
		{
			break;
		}
	}

	if (!isAlreadyHitting && isHit)
	{
		playerCollider->Hit(m_damage);
		m_hitPlayerList.push_back(playerCollider);
	}
	if (isAlreadyHitting && !isHit)
	{
		m_hitPlayerList.erase(it);
	}
}

void ColliderSystem::EnemyAttackCollider::SetWorld(DirectX::XMMATRIX& world)
{
	m_world = world;
#ifdef DEBUG_MODEL
	m_debugModel.GetMaterialMesh(0).GetMaterial(Crown::RenderObject::MaterialTag::Normal).GetConstBuffer(0).SetParameter(1, world);
	m_debugModel.GetMaterialMesh(0).GetMaterial(Crown::RenderObject::MaterialTag::Normal).GetConstBuffer(0).SetParameter(2, Crown::RenderObject::Camera::GetInstance()->GetView() * Crown::RenderObject::Camera::GetInstance()->GetProjection());
#endif // DEBUG_MODEL
}

void ColliderSystem::EnemyAttackCollider::SetActive(bool active)
{
	if (m_active != active)
	{
		if (active)
		{
			ColliderSystem::m_instanceCollection.SetEnemyAttackCollider(this);
		}
		else
		{
			ColliderSystem::m_instanceCollection.DeleteEnemyAttackCollider(this);
			m_hitPlayerList.clear();
		}
#ifdef DEBUG_MODEL
		m_debugModel.SetDrawFlag(active);
#endif // DEBUG_MODEL
		m_active = active;
	}
}