#include "PlayerStateRun.h"
#include "./../../MathLibrary.h"
#include <algorithm>
#include "./../PlayerModel.h"



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
	m_state = State::Start;
}

void Player::PlayerRun::Exit()
{

}

void Player::PlayerRun::Update(float time)
{
	m_animTimer += time / PlayerModel::ANIMATION_FPS;

	float inputAngle = m_player->m_camera.GetRotate().y + atan2(-m_player->m_inputMove.x, -m_player->m_inputMove.y);
	m_inputVector.x += sin(inputAngle) * ROLL_SPEED * time;
	m_inputVector.y += cos(inputAngle) * ROLL_SPEED * time;
	m_inputVector = Crown::Math::VectorNormalize(m_inputVector);

	//	位置を更新するよ☆
	float moveAngle = atan2(m_inputVector.x, m_inputVector.y);
	m_player->m_position.x -= sin(moveAngle) * RUN_SPEED * time;
	m_player->m_position.z -= cos(moveAngle) * RUN_SPEED * time;

	//	角度を更新するよ☆
	m_player->m_rotate.y = moveAngle;

	//	位置を更新するよ☆
	m_player->m_camera.SetPosition(m_player->m_position);

	switch (m_state)
	{
	case Player::PlayerRun::State::Start:
		m_state = State::Continuation;
		break;
	case Player::PlayerRun::State::Continuation:

		break;
	case Player::PlayerRun::State::End:
		break;
	case Player::PlayerRun::State::Turn:
		break;
	}

	//	アニメーションを再生するよ☆
	switch (m_state)
	{
	case Player::PlayerRun::State::Start:

		break;
	case Player::PlayerRun::State::Continuation:
		m_player->m_model.GetRunAnim().GetAnimation(m_animTimer * RUN_ANIM_SPEED, m_player->m_model.GetBone(), m_player->m_model.GetBoneMap());
		break;
	case Player::PlayerRun::State::End:

		break;
	case Player::PlayerRun::State::Turn:
		break;
	}
}

void Player::PlayerRun::OnInputMove(DirectX::XMFLOAT2 input)
{
	input;
	if (Crown::Math::VectorSquareSize(input) < 0.1)
	{
		m_player->m_playerState.ChangeState(StateID::Stand);
	}
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