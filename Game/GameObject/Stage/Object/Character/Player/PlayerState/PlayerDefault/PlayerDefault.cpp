#include "PlayerDefault.h"
#include "./../../Crown/Object/Input.h"

#include "./PlayerStand.h"
#include "./PlayerRun.h"

Player::PlayerDefault::PlayerDefault(Player* owner)
	:
	m_owner(owner),
	m_state(),
	m_nowState(State::Stand),
	m_speed(0.0)
{
	//	ステートマシンの設定だよ☆
	m_state.RegisterState<PlayerStand>(State::Stand, this);
	m_state.RegisterState<PlayerRun>(State::Run, this);
	m_state.SetEnterFunction(&PlayerDefaultStateBase::Enter);
	m_state.SetExitFunction(&PlayerDefaultStateBase::Exit);
	m_state.ChangeState(State::Stand);
}

Player::PlayerDefault::~PlayerDefault()
{

}

void Player::PlayerDefault::Enter()
{
	MoveAngleUpdate();
	m_owner->m_stateTimer = 0;
	if (m_owner->m_inputMove.x || m_owner->m_inputMove.y)
	{
		m_state.ChangeState(State::Run);
		m_nowState = State::Run;
	}
	else
	{
		m_state.ChangeState(State::Stand);
		m_nowState = State::Stand;
	}
	m_speed = 0.0f;
}

void Player::PlayerDefault::Exit()
{

}

void Player::PlayerDefault::Update(float time)
{
	m_owner->m_stateTimer += time;
	m_state.CallFunction(&PlayerDefaultStateBase::Update, time);
	m_owner->m_camera.SetPosition(m_owner->GetPosition());
}

void Player::PlayerDefault::OnInputMove(DirectX::XMFLOAT2 input)
{
	MoveAngleUpdate();
	m_state.CallFunction(&PlayerDefaultStateBase::OnInputMove, input);
	if (input.x || input.y)
	{
		if (m_nowState == State::Stand)
		{
			m_state.ChangeState(State::Run);
			m_nowState = State::Run;
		}
	}
	else
	{
		m_state.ChangeState(State::Stand);
		m_nowState = State::Stand;
	}

	m_owner->m_camera.SetPosition(m_owner->GetPosition());
}

void Player::PlayerDefault::OnInputAttackDown()
{
	m_owner->m_playerState.ChangeState(StateID::Attack);
}

void Player::PlayerDefault::OnInputAvoidanceDown()
{

}

void Player::PlayerDefault::OnInputCamera(DirectX::XMFLOAT2 input)
{
	//	カメラを回転させるよ☆
	m_owner->CameraRoll(input);

	MoveAngleUpdate();
}

void Player::PlayerDefault::MoveAngleUpdate()
{
	m_owner->m_targetAngle = atan2(-m_owner->m_inputMove.x, -m_owner->m_inputMove.y) + m_owner->m_camera.GetRotate().y;
}