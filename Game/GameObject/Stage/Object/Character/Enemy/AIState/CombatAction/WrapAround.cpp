#include "WrapAround.h"
#include "./../../MathLibrary.h"

Enemy::EnemyAIStateCombat::WrapAround::WrapAround(EnemyAIStateCombat* owner)
	:
	m_owner(owner),
	m_animTimer(0),
	m_direction(1),
	m_timer(0)
{

}

Enemy::EnemyAIStateCombat::WrapAround::~WrapAround()
{

}

void Enemy::EnemyAIStateCombat::WrapAround::Enter()
{
	m_timer = 0;
	m_animTimer = 0;
	m_direction *= -1;
}

void Enemy::EnemyAIStateCombat::WrapAround::Update(float time)
{
	m_timer += time / 1000.0f;

	m_animTimer += time / ANIMATION_FPS;
	m_owner->m_enemy->m_standAnim.GetAnimation(m_animTimer, m_owner->m_enemy->m_bone, m_owner->m_enemy->m_model.GetBoneDate());

	const DirectX::XMFLOAT3 toPlayerVector = Crown::Math::VectorSub(m_owner->m_enemy->m_position, m_owner->m_enemy->m_enemyCollider.GetPlayerPosition());

	//	ƒvƒŒƒCƒ„[‚Ì•û‚ðŒü‚­
	m_owner->m_enemy->m_rotate.y = Crown::Math::RollNormal(m_owner->m_enemy->m_rotate.y);
	float roll = atan2(-toPlayerVector.x, -toPlayerVector.z) - m_owner->m_enemy->m_rotate.y;
	roll = Crown::Math::RollNormal(roll);
	roll = std::clamp(roll, -ROLL_SPEED * time, ROLL_SPEED * time);
	m_owner->m_enemy->m_rotate.y += roll;

	//	ˆÚ“®‚·‚é‚æ™
	DirectX::XMFLOAT3 moveVector = Crown::Math::VectorOuterProduct(DirectX::XMFLOAT3(sin(m_owner->m_enemy->m_rotate.y), 0, cos(m_owner->m_enemy->m_rotate.y)), DirectX::XMFLOAT3(0,1,0));
	moveVector = Crown::Math::VectorNormalize(moveVector);
	moveVector = Crown::Math::VectorScale(moveVector, MOVE_SPEED * time * m_direction);
	m_owner->m_enemy->m_position = Crown::Math::VectorAdd(m_owner->m_enemy->m_position, moveVector);

	if (m_timer > BRANCH_TIMING)
	{
		int actionNum = ((*m_owner->m_enemy->m_random.get())()) % (ATTACK + CHANGE);
		if (actionNum < CHANGE)
		{
			m_owner->ActionBranch();
		}
		m_timer = 0;
	}
}

void Enemy::EnemyAIStateCombat::WrapAround::Exit()
{
	m_timer = 0;
}