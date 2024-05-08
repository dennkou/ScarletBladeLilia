#include "CombatAttack1.h"
#include "./../../MathLibrary.h"

Enemy::EnemyAIStateCombat::Attack1::Attack1(EnemyAIStateCombat* owner)
	:
	m_owner(owner),
	m_animTimer(0.0f),
	m_attackCollider(0,
		{
			{ DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0) }
		})
{

}

Enemy::EnemyAIStateCombat::Attack1::~Attack1()
{

}

void Enemy::EnemyAIStateCombat::Attack1::Enter()
{

}

void Enemy::EnemyAIStateCombat::Attack1::Update(float time)
{
	m_animTimer += time / ANIMATION_FPS;

	m_owner->m_enemy->m_attackAnim.GetAnimation(m_animTimer, m_owner->m_enemy->m_bone, m_owner->m_enemy->m_model.GetBoneDate());

	DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(m_owner->m_enemy->m_rotate.x, m_owner->m_enemy->m_rotate.y, m_owner->m_enemy->m_rotate.z) * DirectX::XMMatrixTranslation(m_owner->m_enemy->m_position.x, m_owner->m_enemy->m_position.y, m_owner->m_enemy->m_position.z);
	m_attackCollider.SetWorld(matrix);

	if (m_animTimer < TRACKING_END_FLAME)
	{
		const DirectX::XMFLOAT3 toPlayerVector = VectorSub(m_owner->m_enemy->m_position, m_owner->m_enemy->m_enemyCollider.GetPlayerPosition());

		//	ƒvƒŒƒCƒ„[‚Ì•û‚ðŒü‚­
		m_owner->m_enemy->m_rotate.y = atan2(-toPlayerVector.x, -toPlayerVector.z);
	}

	if (ATTACK_START_FLAME <= m_animTimer && m_animTimer <= ATTACK_END_FLAME)
	{
		m_attackCollider.SetActive(true);
	}
	else
	{
		m_attackCollider.SetActive(false);
	}

	if (m_animTimer > m_owner->m_enemy->m_attackAnim.GetMaxFrame())
	{
		m_owner->m_state.ChangeState(StateType::Vigilance);
	}
}

void Enemy::EnemyAIStateCombat::Attack1::Exit()
{

}
