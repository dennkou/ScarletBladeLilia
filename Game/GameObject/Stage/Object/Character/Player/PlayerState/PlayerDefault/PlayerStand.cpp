#include "PlayerStand.h"

Player::PlayerDefault::PlayerStand::PlayerStand(PlayerDefault* owner)
	:
	m_owner(owner),
	m_player(owner->m_owner)
{

}

Player::PlayerDefault::PlayerStand::~PlayerStand()
{

}

void Player::PlayerDefault::PlayerStand::Enter()
{
	m_player->m_stateTimer = 0;
}

void Player::PlayerDefault::PlayerStand::Update(float time)
{
	time;

	m_player->m_standAnim.GetAnimation(m_player->m_stateTimer / ANIMATION_FPS, m_player->m_bone, m_player->GetModel().GetBoneDate());
}

void Player::PlayerDefault::PlayerStand::Exit()
{

}

void Player::PlayerDefault::PlayerStand::OnInputMove(DirectX::XMFLOAT2 input)
{
	input;
	m_owner->m_state.ChangeState(State::Move);
}