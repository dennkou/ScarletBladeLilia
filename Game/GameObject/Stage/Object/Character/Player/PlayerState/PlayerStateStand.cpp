#include "PlayerStateStand.h"
#include "../../MathLibrary.h"



Player::PlayerStand::PlayerStand(Player* player)
	:
	m_player(player),
	m_animTimer(0.0f)
{
}

Player::PlayerStand::~PlayerStand()
{
}

void Player::PlayerStand::Enter()
{
	m_player->m_velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Player::PlayerStand::Exit()
{
}

void Player::PlayerStand::Update(float time)
{
	m_animTimer += time / ANIMATION_FPS * STAND_ANIM_SPEED;
	m_player->m_standAnim.GetAnimation(m_animTimer, m_player->m_bone, m_player->m_model.GetBoneDate());
}

void Player::PlayerStand::OnInputMove(DirectX::XMFLOAT2 input)
{
	if (VectorSquareSize(input) > 0.0f)
	{
		m_player->m_playerState.ChangeState(StateID::Walk);
	}
}

void Player::PlayerStand::OnInputDash(bool input)
{
	if (input && VectorSquareSize(m_player->m_inputMove) > 0.3f)
	{
		m_player->m_playerState.ChangeState(StateID::Run);
	}
}

void Player::PlayerStand::OnInputAttackDown()
{
	m_player->m_playerState.ChangeState(StateID::Attack);
}

void Player::PlayerStand::OnInputAvoidanceDown()
{
}

void Player::PlayerStand::OnInputCamera(DirectX::XMFLOAT2 input)
{
	m_player->CameraRoll(input);
}