#include "PlayerDefault.h"
#include "./DefaultState/DefaultStateBase.h"
#include "./DefaultState/PlayerStateStand.h"
#include "./DefaultState/PlayerStateWalk.h"
#include "./DefaultState/PlayerStateRun.h"

Player::PlayerDefault::PlayerDefault(Player* owner)
	:
	m_owner(owner)
{
	m_state.RegisterState<Stand>(StateID::STAND, this);
	m_state.RegisterState<Walk>(StateID::WALK, this);
	m_state.RegisterState<Run>(StateID::RUN, this);
	m_state.SetEnterFunction(&State::Enter);
	m_state.SetExitFunction(&State::Exit);
	m_state.ChangeState(StateID::STAND);
}

Player::PlayerDefault::~PlayerDefault()
{

}

void Player::PlayerDefault::Enter()
{
	m_state.ChangeState(StateID::STAND);
}

void Player::PlayerDefault::Exit()
{

}

void Player::PlayerDefault::Update(float time)
{
	m_state.CallFunction(&State::Update, time);
}

void Player::PlayerDefault::OnInputMove(DirectX::XMFLOAT2 input)
{
	m_state.CallFunction(&State::OnInputMove, input);
}

void Player::PlayerDefault::OnInputAttackDown()
{
	m_owner->m_playerState.ChangeState(Player::StateID::Attack);
}

void Player::PlayerDefault::OnInputAvoidanceDown()
{

}

void Player::PlayerDefault::OnInputCamera(DirectX::XMFLOAT2 input)
{
	//	ƒJƒƒ‰‚ð‰ñ“]‚³‚¹‚é‚æ™
	m_owner->CameraRoll(input);
}

void Player::PlayerDefault::OnInputDash(bool input)
{
	m_state.CallFunction(&State::OnInputDash, input);
}