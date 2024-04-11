#include "PlayerStateStand.h"
#include "../../MathLibrary.h"



Player::PlayerDefault::Stand::Stand(PlayerDefault* owner)
	:
	m_owner(*owner),
	m_player(*owner->m_owner),
	m_timer(0.0f)
{
}

Player::PlayerDefault::Stand::~Stand()
{
}

void Player::PlayerDefault::Stand::Enter()
{
	m_player.m_velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Player::PlayerDefault::Stand::Exit()
{
}

void Player::PlayerDefault::Stand::Update(float time)
{
	m_timer += time / ANIMATION_FPS * STAND_ANIM_SPEED;
	m_player.m_standAnim.GetAnimation(m_timer, m_player.m_bone, m_player.GetModel().GetBoneDate());
}

void Player::PlayerDefault::Stand::OnInputMove(DirectX::XMFLOAT2 input)
{
	if (VectorSquareSize(input) > 0.0f)
	{
		m_owner.m_state.ChangeState(StateID::WALK);
	}
}

void Player::PlayerDefault::Stand::OnInputDash(bool input)
{
	if (input)
	{
		m_owner.m_state.ChangeState(StateID::RUN);
	}
}