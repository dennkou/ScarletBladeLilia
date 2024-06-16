#include "Approach.h"
#include "./../../MathLibrary.h"

Enemy::EnemyAIStateCombat::Approach::Approach(EnemyAIStateCombat* owner)
	:
	m_owner(owner),
	m_animTimer(0.0),
	m_moveFlag(false)
{

}

Enemy::EnemyAIStateCombat::Approach::~Approach()
{

}

void Enemy::EnemyAIStateCombat::Approach::Enter()
{
	m_moveFlag = false;
}

void Enemy::EnemyAIStateCombat::Approach::Update(float time)
{
	Enemy* enemy = m_owner->m_enemy;

	//	�A�j���[�V�������恙
	m_animTimer += time / ANIMATION_FPS;
	enemy->m_standAnim.GetAnimation(m_animTimer, enemy->m_bone, enemy->m_model.GetBoneDate());

	const DirectX::XMFLOAT3 toPlayerVector = VectorSub(enemy->m_enemyCollider.GetPlayerPosition(), enemy->m_position);

	//	�ړ�����恙
	if (m_moveFlag)
	{
		DirectX::XMFLOAT3 moveVector = VectorNormalize(toPlayerVector);
		moveVector = VectorScale(moveVector, MOVE_SPEED * time);
		enemy->m_position = VectorAdd(enemy->m_position, moveVector);
	}

	//	�v���C���[�̕��������恙
	enemy->m_rotate.y = RollNormal(enemy->m_rotate.y);
	float roll = atan2(toPlayerVector.x, toPlayerVector.z) - enemy->m_rotate.y;
	roll = RollNormal(roll);
	enemy->m_rotate.y += std::clamp(roll, -ROLL_SPEED * time, ROLL_SPEED * time);

	//	�I���������恙
	if (VectorSquareSize(toPlayerVector) < STOP_DISTANCE * STOP_DISTANCE && m_moveFlag)
	{
		m_owner->ActionBranch();
	}

	//	�ړ��J�n���邩���恙
	if (-DirectX::XMConvertToRadians(MOVE_START_ANGLE) < roll && roll < DirectX::XMConvertToRadians(MOVE_START_ANGLE))
	{
		m_moveFlag = true;
	}
}

void Enemy::EnemyAIStateCombat::Approach::Exit()
{
	m_moveFlag = false;
}