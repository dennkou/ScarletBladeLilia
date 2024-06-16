#include "PlayerAvoidance.h"

Player::PlayerAvoidance::PlayerAvoidance(Player* player)
	:
	m_player(player),
	m_animTimer(0.0f)
{

}

Player::PlayerAvoidance::~PlayerAvoidance()
{

}

void Player::PlayerAvoidance::Enter()
{

}

void Player::PlayerAvoidance::Update(float time)
{
	m_animTimer += time;

	m_player->m_frontStepAnim.GetAnimation(m_animTimer, m_player->m_bone, m_player->m_model.GetBoneDate());
}

void Player::PlayerAvoidance::Exit()
{

}