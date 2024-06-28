#include "PlayerAttack.h"
#include <numbers>
#include "./../../Crown/Object/Algorithm.h"

Player::PlayerAttack::PlayerAttack(Player* player)
	:
	m_player(player),
	m_attackCollider(30,
		{
			{ DirectX::XMFLOAT3(0,10,0),DirectX::XMFLOAT3(0, 10, 25),				DirectX::XMFLOAT3(21.6506f, 10, 12.5f) },
			{ DirectX::XMFLOAT3(0,10,0),DirectX::XMFLOAT3(21.6506f, 10, 12.5f),		DirectX::XMFLOAT3(21.6506f, 10,-12.5f) },
			{ DirectX::XMFLOAT3(0,10,0),DirectX::XMFLOAT3(21.6506f, 10,-12.5f),		DirectX::XMFLOAT3(0, 10, -25) },
			{ DirectX::XMFLOAT3(0,10,0),DirectX::XMFLOAT3(0, 10, -25),				DirectX::XMFLOAT3(-21.6506f, 10,-12.5f) },
			{ DirectX::XMFLOAT3(0,10,0),DirectX::XMFLOAT3(-21.6506f, 10,-12.5f),	DirectX::XMFLOAT3(-21.6506f, 10, 12.5f) },
			{ DirectX::XMFLOAT3(0,10,0),DirectX::XMFLOAT3(-21.6506f, 10, 12.5f),	DirectX::XMFLOAT3(0, 10, 25) },
		}), 
	m_attackFlag(false),
	m_startPosition(DirectX::XMFLOAT3(0,0,0)),
	m_animTimer(0),
	m_inputAttack(false)
{
}

Player::PlayerAttack::~PlayerAttack()
{

}

void Player::PlayerAttack::Enter()
{
	m_startPosition = m_player->m_position;
	m_attackFlag = false;
	m_inputAttack = false;
}

void Player::PlayerAttack::Update(float time)
{
	m_animTimer += time;

	//	アニメーション終了時デフォルトに戻るよ☆
	if (m_animTimer > END_FLAME)
	{
		m_player->m_playerState.ChangeState(StateID::Stand);
	}

	//	キャラクターを移動させるよ☆
	FlameProcess(0, MOVING_END_FLAME, [&](float t)
		{
			t;
			m_player->m_position.x -= sin(m_player->m_rotate.y) * MOVING_DISTANCE / MOVING_END_FLAME * time;
			m_player->m_position.z -= cos(m_player->m_rotate.y) * MOVING_DISTANCE / MOVING_END_FLAME * time;
		});
	CameraAnim(m_animTimer);
	m_player->m_camera.SetPosition(m_player->m_position);

	//	移動入力によるキャンセル受付だよ☆
	if (m_animTimer >= MOVE_CANCEL_FLAME && (m_player->m_inputMove.x || m_player->m_inputMove.y))
	{
		m_player->m_playerState.ChangeState(StateID::Walk);
	}

	if (m_inputAttack && (m_animTimer> NEXT_ATTACK_FLAME))
	{
		m_player->m_playerState.ChangeState(StateID::SlashAttack);
	}

	//	攻撃判定の生成＆削除だよ☆
	if (m_animTimer > ATTACK_END_FLAME && m_attackFlag)
	{
		m_attackCollider.SetActive(false);
	}
	if (m_animTimer > ATTACK_START_FLAME && !m_attackFlag)
	{
		m_attackCollider.SetActive(true);
		m_attackFlag = true;
	}
	DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(m_player->m_rotate.x, m_player->m_rotate.y, m_player->m_rotate.z) * DirectX::XMMatrixTranslation(m_player->m_position.x, m_player->m_position.y, m_player->m_position.z);
	m_attackCollider.SetPlayerWorld(matrix);
}

void Player::PlayerAttack::Exit()
{
	m_animTimer = 0;
	m_player->m_camera.SetFovAngle(DEFAULT_FOV_ANGLE);
}

void Player::PlayerAttack::OnInputMove(DirectX::XMFLOAT2 input)
{
	input;
}

void Player::PlayerAttack::OnInputAttackDown()
{
	if (m_animTimer > NEXT_ATTACK_FLAME)
	{
		m_player->m_playerState.ChangeState(StateID::SlashAttack);
	}
	m_inputAttack = true;
}

void Player::PlayerAttack::OnInputCamera(DirectX::XMFLOAT2 input)
{
	m_player->CameraRoll(input);
}

void Player::PlayerAttack::CameraAnim(float animFlame)
{
	animFlame;
}

void Player::PlayerAttack::FlameProcess(float start, float end, std::function<void(float)> process) const
{
	if (start <= m_animTimer && m_animTimer <= end)
	{
		process((m_animTimer - start) / (end - start));
	}
}