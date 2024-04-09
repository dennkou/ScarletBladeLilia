#include "EnemyStatePatrol.h"
#include "./../../MathLibrary.h"


Enemy::EnemyStatePatrol::EnemyStatePatrol(Enemy* enemy, NavigationAI* navigationAI)
	:
	m_enemy(enemy),
	m_navigationAI(navigationAI)
{

}

Enemy::EnemyStatePatrol::~EnemyStatePatrol()
{

}

void Enemy::EnemyStatePatrol::Enter()
{

}

void Enemy::EnemyStatePatrol::Update(float time)
{
	m_navigationAI->SetPosition(m_enemy->GetPosition());
	if (m_enemy->m_enemyCollider.IsEngagement())
	{
		m_enemy->m_state.ChangeState(Enemy::State::Combat);
	}

	//	ˆÚ“®ˆ—‚¾‚æ™
	DirectX::XMFLOAT3 toTargetPosition = VectorSub(m_navigationAI->MovingPosition(), m_enemy->GetPosition());
	DirectX::XMFLOAT3 moveVector = VectorScale(VectorNormalize(toTargetPosition), SPEED * time);
	m_vector = VectorAdd(m_vector, moveVector);
	m_vector = VectorSub(m_vector, VectorScale(m_vector, FRICTION * time));
	
	//	‰ñ“]‚Ìˆ—‚¾‚æ™
	DirectX::XMFLOAT3 rotate = m_enemy->GetRotate();
	rotate.y = atan2(m_vector.x, m_vector.z);
	m_enemy->SetRotate(rotate);

	m_enemy->SetPosition(VectorAdd(m_vector,m_enemy->GetPosition()));

	//	ƒvƒŒƒCƒ„[‚ªŽ‹ŠE“à‚É“ü‚Á‚Ä‚¢‚é‚©‚Ì”»’è‚¾‚æ™
	DirectX::XMFLOAT3 toPlayerVector = VectorSub(m_enemy->GetPosition(), m_enemy->m_enemyCollider.GetPlayerPosition());

	std::cout << toPlayerVector.x << "," << toPlayerVector.y << "," << toPlayerVector.z << std::endl;
	if (VectorSquareSize(toPlayerVector) <= (SEARCH_DISTANCE * SEARCH_DISTANCE))
	{
		float tmp = rotate.y - atan2(toPlayerVector.z, toPlayerVector.x);
		if (abs(tmp) < DirectX::XMConvertToRadians(SEARCH_ANGLE))
		{
			m_enemy->m_state.ChangeState(State::Combat);
		}
	}
}

void Enemy::EnemyStatePatrol::Exit()
{

}

void Enemy::EnemyStatePatrol::OnDamage(int damage)
{

}