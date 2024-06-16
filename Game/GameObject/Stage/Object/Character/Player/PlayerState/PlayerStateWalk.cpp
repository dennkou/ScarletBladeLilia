#include "PlayerStateWalk.h"
#include "./../../MathLibrary.h"

Player::PlayerWalk::PlayerWalk(Player* player)
	:
	m_player(player),
	m_animTimer(0.0f),
	m_end(false),
	m_inputVector(),
	m_speed(0.0f)
{
}

Player::PlayerWalk::~PlayerWalk()
{
}

void Player::PlayerWalk::Enter()
{
	m_animTimer = 0.0f;
	m_inputVector.x = 0.0f;
	m_inputVector.y = 0.0f;
	m_speed = 0.0f;
}

void Player::PlayerWalk::Exit()
{
}

void Player::PlayerWalk::Update(float time)
{
	//	ローカル変数の定義だよ☆
	float inputAngle = m_player->m_camera.GetRotate().y + atan2(-m_player->m_inputMove.x, -m_player->m_inputMove.y);
	float inputPower = sqrt(VectorSquareSize(m_player->m_inputMove));

	if (inputPower > 0.1f)
	{
		m_inputVector.x += sin(inputAngle) * ROLL_SPEED;
		m_inputVector.y += cos(inputAngle) * ROLL_SPEED;
		m_inputVector = VectorNormalize(m_inputVector);
	}

	//	位置を更新するよ☆
	m_player->m_position.x -= m_inputVector.x * WALK_SPEED * time;
	m_player->m_position.z -= m_inputVector.y * WALK_SPEED * time;


	//	角度を更新するよ☆
	m_player->m_rotate.y = atan2(m_inputVector.x, m_inputVector.y);


	if (m_animTimer <= m_player->m_walkStartAnim.GetMaxFrame())
	{
		//	開始時の挙動だよ☆
		m_player->m_walkStartAnim.GetAnimation(m_animTimer, m_player->m_bone, m_player->m_model.GetBoneDate());
		m_animTimer += time / ANIMATION_FPS * WALK_START_ANIM_SPEED;
	}
	else if (inputPower <= 0.0f)
	{
		//	終了時の挙動だよ☆
		m_player->m_playerState.ChangeState(StateID::Stand);
	}
	else if(VectorInnerProduct(m_inputVector, DirectX::XMFLOAT2(sin(inputAngle), cos(inputAngle))) < cos(DirectX::XMConvertToRadians(135)))
	{
		//	振り向きの挙動だよ☆
		m_inputVector.x = sin(inputAngle);
		m_inputVector.y = cos(inputAngle);
	}
	else
	{
		//	通常の挙動だよ☆
		m_animTimer += time / ANIMATION_FPS * WALK_ANIM_SPEED;
		m_player->m_walkAnim.GetAnimation(m_animTimer - m_player->m_walkStartAnim.GetMaxFrame(), m_player->m_bone, m_player->m_model.GetBoneDate());
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