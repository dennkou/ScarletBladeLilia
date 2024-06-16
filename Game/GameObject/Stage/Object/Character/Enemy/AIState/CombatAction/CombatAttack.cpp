#include "CombatAttack.h"
#include "./../../MathLibrary.h"

Enemy::EnemyAIStateCombat::Attack::Attack(EnemyAIStateCombat* owner)
	:
	m_owner(owner),
	m_animTimer(0.0f),
	m_attackCollider(700, 
		{
			{ DirectX::XMFLOAT3(0,10,0),	DirectX::XMFLOAT3(-15.0f, 10, 95.0f),	DirectX::XMFLOAT3(-35.0f, 10, 25.0f) },
			{ DirectX::XMFLOAT3(0,10,0),	DirectX::XMFLOAT3(-15.0f, 10, 95.0f),	DirectX::XMFLOAT3(0.0f, 10, 95.0f) }
		})
{
}

Enemy::EnemyAIStateCombat::Attack::~Attack()
{
}

void Enemy::EnemyAIStateCombat::Attack::Enter()
{
	m_animTimer = 0.0f;
	m_attackCollider.SetActive(false);
}

void Enemy::EnemyAIStateCombat::Attack::Update(float time)
{
	Enemy* enemy = m_owner->m_enemy;

	m_animTimer += time / ANIMATION_FPS;
	enemy->m_attackAnim.GetAnimation(m_animTimer, enemy->m_bone, enemy->m_model.GetBoneDate());

	DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(enemy->m_rotate.x, enemy->m_rotate.y, enemy->m_rotate.z) * DirectX::XMMatrixTranslation(enemy->m_position.x, enemy->m_position.y, enemy->m_position.z);
	m_attackCollider.SetWorld(matrix);

	if (m_animTimer < TRACKING_END_FLAME || (ATTACK_TRACKING_START_FLAME <= m_animTimer && m_animTimer <= ATTACK_TRACKING_END_FLAME))
	{
		//	ƒvƒŒƒCƒ„[‚Ì•û‚ðŒü‚­
		const DirectX::XMFLOAT3 toPlayerVector = VectorSub(enemy->m_position, enemy->m_enemyCollider.GetPlayerPosition());
		float roll = atan2(-toPlayerVector.x, -toPlayerVector.z) - enemy->m_rotate.y;
		roll = RollNormal(roll);
		enemy->m_rotate.y += std::clamp(roll, -ATTACK_TRACKING_ROLL * time, ATTACK_TRACKING_ROLL * time);
	}

	if (ATTACK_START_FLAME <= m_animTimer && m_animTimer <= ATTACK_END_FLAME)
	{
		m_attackCollider.SetActive(true);
	}
	else
	{
		m_attackCollider.SetActive(false);
	}

	if (m_animTimer > enemy->m_attackAnim.GetMaxFrame())
	{
		m_owner->ActionBranch();
	}
}

void Enemy::EnemyAIStateCombat::Attack::Exit()
{
	m_attackCollider.SetActive(false);
}
