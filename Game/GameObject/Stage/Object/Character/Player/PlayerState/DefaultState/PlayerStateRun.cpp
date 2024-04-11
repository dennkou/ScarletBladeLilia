#include "PlayerStateRun.h"
#include "./../../MathLibrary.h"



Player::PlayerDefault::Run::Run(PlayerDefault* owner)
	:
	m_owner(*owner),
	m_player(*owner->m_owner),
	m_timer(0.0f)
{
}

Player::PlayerDefault::Run::~Run()
{
}

void Player::PlayerDefault::Run::Enter()
{
	m_timer = 0.0f;
	inputVector.x = sin(m_player.GetRotate().y);
	inputVector.y = cos(m_player.GetRotate().y);
}

void Player::PlayerDefault::Run::Exit()
{
}

void Player::PlayerDefault::Run::Update(float time)
{
	float inputAngle = m_player.m_camera.GetRotate().y + atan2(-m_player.m_inputMove.x, -m_player.m_inputMove.y);
	DirectX::XMFLOAT3 position = m_player.GetPosition();
	DirectX::XMFLOAT3 rotate = m_player.GetRotate();

	inputVector.x += sin(inputAngle) * ROLL_SPEED;
	inputVector.y += cos(inputAngle) * ROLL_SPEED;
	inputVector = VectorNormalize(inputVector);

	//	位置を更新するよ☆
	position.x -= inputVector.x * RUN_SPEED * time;
	position.z -= inputVector.y * RUN_SPEED * time;

	//	角度を更新するよ☆
	rotate.y = atan2(inputVector.x, inputVector.y);

	//	変数を更新するよ☆
	m_player.SetPosition(position);
	m_player.m_camera.SetPosition(position);
	m_player.SetRotate(rotate);

	m_timer += time / ANIMATION_FPS * RUN_ANIM_SPEED;
	m_player.m_runAnim.GetAnimation(m_timer - m_player.m_walkStartAnim.GetMaxFrame(), m_player.m_bone, m_player.GetModel().GetBoneDate());
}

void Player::PlayerDefault::Run::OnInputMove(DirectX::XMFLOAT2 input)
{
}

void Player::PlayerDefault::Run::OnInputDash(bool input)
{
	if (!input)
	{
		m_owner.m_state.ChangeState(StateID::WALK);
	}
}
