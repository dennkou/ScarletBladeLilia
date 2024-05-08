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

	//	位置を更新するよ☆
	m_player->m_position.x -= sin(atan2(m_inputVector.x, m_inputVector.y)) * RUN_SPEED * time;
	m_player->m_position.z -= cos(atan2(m_inputVector.x, m_inputVector.y)) * RUN_SPEED * time;

	//	角度を更新するよ☆
	m_player->m_rotate.y = atan2(m_inputVector.x, m_inputVector.y);

	//	変数を更新するよ☆
	m_player->m_camera.SetPosition(m_player->m_position);

	m_animTimer += time / ANIMATION_FPS * RUN_ANIM_SPEED;
	m_player->m_runAnim.GetAnimation(m_animTimer - m_player->m_walkStartAnim.GetMaxFrame(), m_player->m_bone, m_player->m_model.GetBoneDate());
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