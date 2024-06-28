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
	//	���[�J���ϐ��̒�`���恙
	float inputAngle = m_player->m_camera.GetRotate().y + atan2(-m_player->m_inputMove.x, -m_player->m_inputMove.y);
	float inputPower = sqrt(VectorSquareSize(m_player->m_inputMove));

	if (inputPower > 0.1f)
	{
		m_inputVector.x += sin(inputAngle) * ROLL_SPEED;
		m_inputVector.y += cos(inputAngle) * ROLL_SPEED;
		m_inputVector = VectorNormalize(m_inputVector);
	}

	//	�ʒu���X�V����恙
	m_player->m_position.x -= m_inputVector.x * WALK_SPEED * time;
	m_player->m_position.z -= m_inputVector.y * WALK_SPEED * time;


	//	�p�x���X�V����恙
	m_player->m_rotate.y = atan2(m_inputVector.x, m_inputVector.y);

	m_player->m_model.Move(m_inputVector);
	if (inputPower <= 0.0f)
	{
		//	�I�����̋������恙
		m_player->m_playerState.ChangeState(StateID::Stand);
	}
	else if(VectorInnerProduct(m_inputVector, DirectX::XMFLOAT2(sin(inputAngle), cos(inputAngle))) < cos(DirectX::XMConvertToRadians(135)))
	{
		//	�U������̋������恙
		m_inputVector.x = sin(inputAngle);
		m_inputVector.y = cos(inputAngle);
	}

	//	�ϐ����X�V����恙
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