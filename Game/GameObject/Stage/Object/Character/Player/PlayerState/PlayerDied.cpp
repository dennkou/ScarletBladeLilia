#include "PlayerDied.h"

Player::PlayerDied::PlayerDied(Player* player)
	:
	m_player(player)
{

}

Player::PlayerDied::~PlayerDied()
{

}

void Player::PlayerDied::Enter()
{
	m_player->EventTrigger(&GameObject::OnPlayerDied);
}

void Player::PlayerDied::Exit()
{

}

void Player::PlayerDied::Update(float time)
{
	time;
}