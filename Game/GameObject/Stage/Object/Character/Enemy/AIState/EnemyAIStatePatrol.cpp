#include "EnemyAIStatePatrol.h"
#include "./../../MathLibrary.h"


Enemy::EnemyAIStatePatrol::EnemyAIStatePatrol(Enemy* enemy, NavigationAI* navigationAI)
	:
	m_enemy(enemy),
	m_navigationAI(navigationAI),
	m_animTimer(0.0f)
{

}

Enemy::EnemyAIStatePatrol::~EnemyAIStatePatrol()
{

}

void Enemy::EnemyAIStatePatrol::Enter()
{
	m_animTimer = 0.0f;
}

void Enemy::EnemyAIStatePatrol::Update(float time)
{
	m_animTimer += time / ANIMATION_FPS;

	m_navigationAI->SetPosition(m_enemy->m_position);
	if (m_enemy->m_enemyCollider.IsEngagement())
	{
		m_enemy->m_aiState.ChangeState(Enemy::AIState::Combat);
	}

	//	�ړ��������恙
	DirectX::XMFLOAT3 toTargetPosition = VectorSub(m_navigationAI->MovingPosition(), m_enemy->m_position);
	DirectX::XMFLOAT3 moveVector = VectorScale(VectorNormalize(toTargetPosition), SPEED * time);
	m_vector = VectorAdd(m_vector, moveVector);
	m_vector = VectorScale(m_vector, FRICTION);
	
	//	��]�̏������恙
	m_enemy->m_rotate.y = atan2(m_vector.x, m_vector.z);

	m_enemy->m_position = VectorAdd(m_vector,m_enemy->m_position);

	//	�v���C���[�����E���ɓ����Ă��邩�̔��肾�恙
	DirectX::XMFLOAT3 toPlayerVector = VectorSub(m_enemy->m_position, m_enemy->m_enemyCollider.GetPlayerPosition());
	if (VectorSquareSize(toPlayerVector) <= (SEARCH_DISTANCE * SEARCH_DISTANCE))
	{
		DirectX::XMFLOAT3 front;
		front.x = sin(m_enemy->m_rotate.y) * cos(m_enemy->m_rotate.x);
		front.y = sin(m_enemy->m_rotate.x);
		front.z = cos(m_enemy->m_rotate.y) * cos(m_enemy->m_rotate.x);
		front = VectorNormalize(front);
		if (VectorInnerProduct(front, toPlayerVector) <= DirectX::XMConvertToRadians(SEARCH_ANGLE))
		{
			m_enemy->m_aiState.ChangeState(AIState::Combat);
		}
	}


	m_enemy->m_standAnim.GetAnimation(m_animTimer, m_enemy->m_bone, m_enemy->m_model.GetBoneDate());
}

void Enemy::EnemyAIStatePatrol::Exit()
{

}

void Enemy::EnemyAIStatePatrol::OnDamage(int damage)
{
	damage;
}