#include "PlayerStateWalk.h"
#include "./../../MathLibrary.h"

Player::PlayerDefault::Walk::Walk(PlayerDefault* owner)
	:
	m_owner(*owner),
	m_player(*owner->m_owner),
	m_timer(0.0f),
	m_end(false)
{
}

Player::PlayerDefault::Walk::~Walk()
{
}

void Player::PlayerDefault::Walk::Enter()
{
	m_timer = 0.0f;
	inputVector.x = sin(m_player.GetRotate().y);
	inputVector.y = cos(m_player.GetRotate().y);
}

void Player::PlayerDefault::Walk::Exit()
{
}

void Player::PlayerDefault::Walk::Update(float time)
{
	//	ローカル変数の定義だよ☆
	float inputAngle = m_player.m_camera.GetRotate().y + atan2(-m_player.m_inputMove.x, -m_player.m_inputMove.y);
	float inputPower = sqrt(VectorSquareSize(m_player.m_inputMove));
	DirectX::XMFLOAT3 rotate = m_player.GetRotate();
	DirectX::XMFLOAT3 position = m_player.GetPosition();
	auto Start = [&]
		{
			m_player.m_walkStartAnim.GetAnimation(m_timer, m_player.m_bone, m_player.GetModel().GetBoneDate());
			m_timer += time / ANIMATION_FPS * WALK_START_ANIM_SPEED;
		};
	auto Normal = [&]
		{
			m_timer += time / ANIMATION_FPS * WALK_ANIM_SPEED;
			m_player.m_walkAnim.GetAnimation(m_timer - m_player.m_walkStartAnim.GetMaxFrame(), m_player.m_bone, m_player.GetModel().GetBoneDate());
		};
	auto End = [&]
		{
			m_owner.m_state.ChangeState(StateID::STAND);
		};
	auto Roll = [&]
		{
			inputVector.x = sin(inputAngle);
			inputVector.y = cos(inputAngle);
		};

	inputVector.x += sin(inputAngle) * ROLL_SPEED;
	inputVector.y += cos(inputAngle) * ROLL_SPEED;
	inputVector = VectorNormalize(inputVector);


	//	位置を更新するよ☆
	position.x -= inputVector.x * WALK_SPEED * time;
	position.z -= inputVector.y * WALK_SPEED * time;


	//	角度を更新するよ☆
	//rotate.y = atan2(-m_player.m_velocity.x, -m_player.m_velocity.z);
	rotate.y = atan2(inputVector.x, inputVector.y);


	//	アニメーションを再生するよ☆
	if (m_timer <= m_player.m_walkStartAnim.GetMaxFrame())
	{
		Start();
	}
	else if (inputPower <= 0.0f)
	{
		End();
	}
	else if(VectorInnerProduct(inputVector, DirectX::XMFLOAT2(sin(inputAngle), cos(inputAngle))) < cos(DirectX::XMConvertToRadians(90)))
	{
		Roll();
	}
	else
	{
		Normal();
	}

	//	変数を更新するよ☆
	m_player.SetPosition(position);
	m_player.m_camera.SetPosition(position);
	m_player.SetRotate(rotate);
}

void Player::PlayerDefault::Walk::OnInputMove(DirectX::XMFLOAT2 input)
{
}

void Player::PlayerDefault::Walk::OnInputDash(bool input)
{
	if (input)
	{
		m_owner.m_state.ChangeState(StateID::RUN);
	}
}