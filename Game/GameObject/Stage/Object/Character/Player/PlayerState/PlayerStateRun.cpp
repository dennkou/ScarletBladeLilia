#include "PlayerStateRun.h"
#include "./../../MathLibrary.h"
#include <algorithm>



Player::PlayerRun::PlayerRun(Player* player)
	:
	m_player(player),
	m_animTimer(0.0f),
	m_inputVector(0.0f,0.0f)
{
}

Player::PlayerRun::~PlayerRun()
{
}

void Player::PlayerRun::Enter()
{
	m_animTimer = 0.0f;
	m_inputVector.x = sin(m_player->m_rotate.y);
	m_inputVector.y = cos(m_player->m_rotate.y);
}

void Player::PlayerRun::Exit()
{
}

void Player::PlayerRun::Update(float time)
{
	float inputAngle = m_player->m_camera.GetRotate().y + atan2(-m_player->m_inputMove.x, -m_player->m_inputMove.y);
	m_inputVector.x += sin(inputAngle) * ROLL_SPEED * time;
	m_inputVector.y += cos(inputAngle) * ROLL_SPEED * time;
	m_inputVector = VectorNormalize(m_inputVector);

	//	�ʒu���X�V����恙
	float moveAngle = atan2(m_inputVector.x, m_inputVector.y);
	m_player->m_position.x -= sin(moveAngle) * RUN_SPEED * time;
	m_player->m_position.z -= cos(moveAngle) * RUN_SPEED * time;

	//	�p�x���X�V����恙
	m_player->m_rotate.y = moveAngle;

	//	�ϐ����X�V����恙
	m_player->m_camera.SetPosition(m_player->m_position);
}

void Player::PlayerRun::OnInputMove(DirectX::XMFLOAT2 input)
{
	input;
}

void Player::PlayerRun::OnInputDash(bool input)
{
	if (!input)
	{
		m_player->m_playerState.ChangeState(StateID::Walk);
	}
}

void Player::PlayerRun::OnInputAttackDown()
{
	m_player->m_playerState.ChangeState(StateID::Attack);
}

void Player::PlayerRun::OnInputAvoidanceDown()
{

}

void Player::PlayerRun::OnInputCamera(DirectX::XMFLOAT2 input)
{
	m_player->CameraRoll(input);
}