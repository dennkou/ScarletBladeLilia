#include "PlayerStateWalk.h"
#include "./../../MathLibrary.h"

Player::PlayerWalk::PlayerWalk(Player* player)
	:
	m_player(player),
	m_inputVector(),
	m_time(0.0f)
{
}

Player::PlayerWalk::~PlayerWalk()
{
}

void Player::PlayerWalk::Enter()
{
	m_inputVector.x = 0.0f;
	m_inputVector.y = 0.0f;
	m_time = 0.0f;
	m_state = State::Start;
}

void Player::PlayerWalk::Exit()
{
}

void Player::PlayerWalk::Update(float time)
{
	m_time += time / PlayerModel::ANIMATION_FPS;

	//	ローカル変数の定義だよ☆
	float inputAngle = m_player->m_camera.GetRotate().y + atan2(-m_player->m_inputMove.x, -m_player->m_inputMove.y);
	float inputPower = sqrt(Crown::Math::VectorSquareSize(m_player->m_inputMove));

	if (inputPower > 0.1f)
	{
		m_inputVector.x += sin(inputAngle) * ROLL_SPEED * time;
		m_inputVector.y += cos(inputAngle) * ROLL_SPEED * time;
		if (Crown::Math::VectorSquareSize(m_inputVector) > 1.0f)
		{
			m_inputVector = Crown::Math::VectorNormalize(m_inputVector);
		}
	}

	//	位置を更新するよ☆
	m_player->m_position.x -= m_inputVector.x * WALK_SPEED * time;
	m_player->m_position.z -= m_inputVector.y * WALK_SPEED * time;

	//	角度を更新するよ☆
	m_player->m_rotate.y = atan2(m_inputVector.x, m_inputVector.y);

	//	アニメーションを再生する前に状態を遷移させるよ☆
	switch (m_state)
	{
	case Player::PlayerWalk::State::Start:
		if (m_time >= m_player->m_model.GetWalkStartAnim().GetMaxFrame())
		{
			m_state = State::Continuation;
			m_time -= m_player->m_model.GetWalkStartAnim().GetMaxFrame();
		}
		break;
	case Player::PlayerWalk::State::Continuation:
		if (inputPower <= 0.0f)
		{
			m_state = State::End;
		}
		if (Crown::Math::VectorInnerProduct(m_inputVector, DirectX::XMFLOAT2(sin(inputAngle), cos(inputAngle))) < cos(DirectX::XMConvertToRadians(135)))
		{
			//m_state = State::Turn;
		}
		break;
	case Player::PlayerWalk::State::End:
		m_player->m_playerState.ChangeState(StateID::Stand);
		break;
	case Player::PlayerWalk::State::Turn:
		m_inputVector.x = sin(inputAngle);
		m_inputVector.y = cos(inputAngle);
		m_state = State::Continuation;
		break;
	}

	//	アニメーションを再生するよ☆
	switch (m_state)
	{
	case Player::PlayerWalk::State::Start:
		m_player->m_model.GetWalkStartAnim().GetAnimation(m_time, m_player->m_model.GetBone(), m_player->m_model.GetBoneMap());
		break;
	case Player::PlayerWalk::State::Continuation:
		m_player->m_model.GetWalkAnim().GetAnimation(m_time, m_player->m_model.GetBone(), m_player->m_model.GetBoneMap());
		break;
	case Player::PlayerWalk::State::End:
		break;
	case Player::PlayerWalk::State::Turn:
		break;
	}

	//	変数を更新するよ☆
	m_player->m_camera.SetPosition(m_player->m_position);
}

void Player::PlayerWalk::OnInputMove(DirectX::XMFLOAT2 input)
{
	input;
}

void Player::PlayerWalk::OnInputDash(bool input)
{
	if (input)
	{
		m_player->m_playerState.ChangeState(StateID::Run);
	}
}

void Player::PlayerWalk::OnInputAttackDown()
{
	m_player->m_playerState.ChangeState(StateID::Attack);
}

void Player::PlayerWalk::OnInputAvoidanceDown()
{
}

void Player::PlayerWalk::OnInputCamera(DirectX::XMFLOAT2 input)
{
	m_player->CameraRoll(input);
}