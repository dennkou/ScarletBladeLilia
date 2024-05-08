#include "CombatVigilance.h"
#include "./../../MathLibrary.h"
#include "./../../../Player/Player.h"

Enemy::EnemyAIStateCombat::Vigilance::Vigilance(EnemyAIStateCombat* owner)
	:
	m_owner(owner),
	m_animTimer(0),
	m_timer(0),
	m_direction(1)
{
}

Enemy::EnemyAIStateCombat::Vigilance::~Vigilance()
{

}

void Enemy::EnemyAIStateCombat::Vigilance::Enter()
{
	m_animTimer = 0;
	m_timer = 0;
}

void Enemy::EnemyAIStateCombat::Vigilance::Update(float time)
{
	m_animTimer += time / ANIMATION_FPS;
	m_timer += time;

	m_owner->m_enemy->m_standAnim.GetAnimation(m_animTimer, m_owner->m_enemy->m_bone, m_owner->m_enemy->m_model.GetBoneDate());
	std::cout << m_timer << std::endl;
	std::cout << m_animTimer << std::endl;

	const DirectX::XMFLOAT3 toPlayerVector = VectorSub(m_owner->m_enemy->m_position, m_owner->m_enemy->m_enemyCollider.GetPlayerPosition());

	//	移動するよ☆
	DirectX::XMFLOAT3 moveVector = VectorOuterProduct(toPlayerVector, DirectX::XMFLOAT3(0,1,0));
	moveVector = VectorNormalize(moveVector);
	moveVector = VectorScale(moveVector, MOVE_SPEED * time * m_direction);
	m_owner->m_enemy->m_position = VectorAdd(m_owner->m_enemy->m_position, moveVector);

	//	プレイヤーの方を向く
	m_owner->m_enemy->m_rotate.y = RollNormal(m_owner->m_enemy->m_rotate.y);
	float roll = atan2(-toPlayerVector.x, -toPlayerVector.z) - m_owner->m_enemy->m_rotate.y;
	roll = RollNormal(roll);
	roll = std::clamp(roll, -ROLL_SPEED * time, ROLL_SPEED * time);
	m_owner->m_enemy->m_rotate.y += roll;

	//	ここから行動分岐だよ☆
	if (m_timer > DURATION)
	{
		if (VectorDistance(m_owner->m_enemy->m_enemyCollider.GetPlayerPosition(), m_owner->m_enemy->m_position) < ATTACK_DISTANCE)
		{
			int actionNum = ((*m_owner->m_enemy->m_random.get())()) % 5;
			switch (actionNum)
			{
			case 0:
				m_owner->m_state.ChangeState(StateType::Attack);
				break;
			case 1:
				m_direction *= -1;
				break;
			case 2:
			case 3:
				m_owner->m_state.ChangeState(StateType::Attack1);
				break;
			default:
				m_timer = 0;
				break;
			}
		}
	}
}

void Enemy::EnemyAIStateCombat::Vigilance::Exit()
{

}