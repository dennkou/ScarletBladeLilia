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
	m_animTimer += time / ANIMATION_FPS;

	m_owner->m_enemy->m_attackAnim.GetAnimation(m_animTimer, m_owner->m_enemy->m_bone, m_owner->m_enemy->m_model.GetBoneDate());

	DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(m_owner->m_enemy->m_rotate.x, m_owner->m_enemy->m_rotate.y, m_owner->m_enemy->m_rotate.z) * DirectX::XMMatrixTranslation(m_owner->m_enemy->m_position.x, m_owner->m_enemy->m_position.y, m_owner->m_enemy->m_position.z);
	m_attackCollider.SetWorld(matrix);

	if (m_animTimer < TRACKING_END_FLAME || (ATTACK_TRACKING_START_FLAME <= m_animTimer && m_animTimer <= ATTACK_TRACKING_END_FLAME))
	{
		//	ƒvƒŒƒCƒ„[‚Ì•û‚ðŒü‚­
		const DirectX::XMFLOAT3 toPlayerVector = VectorSub(m_owner->m_enemy->m_position, m_owner->m_enemy->m_enemyCollider.GetPlayerPosition());
		float roll = atan2(-toPlayerVector.x, -toPlayerVector.z) - m_owner->m_enemy->m_rotate.y;
		roll = fmod(roll, 360.0f);
		if (roll < -180)
		{
			roll += 360;
		}
		else if (roll > 180)
		{
			roll -= 360;
		}
		roll = std::clamp(roll, -ATTACK_TRACKING_ROLL * time, ATTACK_TRACKING_ROLL * time);
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

void Enemy::EnemyAIStateCombat::Attack::Exit()
{
	m_attackCollider.SetActive(false);
}
