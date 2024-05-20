#include "PlayerAttack.h"
#include <numbers>
#include "./../../Crown/Object/Algorithm.h"

Player::PlayerAttack::PlayerAttack(Player* player)
	:
	m_player(player),
	m_attackCollider(25,
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
	m_animTimer(0)
{
}

Player::PlayerAttack::~PlayerAttack()
{

}

void Player::PlayerAttack::Enter()
{
	m_startPosition = m_player->m_position;
	m_attackFlag = false;
}

void Player::PlayerAttack::Update(float time)
{
	time *= 1.5;
	m_animTimer += time;
	float animFlame = m_animTimer / ANIMATION_FPS;
	m_player->m_drawingSwordAttackAnim.GetAnimation(animFlame, m_player->m_bone, m_player->m_model.GetBoneDate());

	//	アニメーション終了時デフォルトに戻るよ☆
	if (animFlame > m_player->m_drawingSwordAttackAnim.GetMaxFrame())
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
	CameraAnim(animFlame);
	m_player->m_camera.SetPosition(m_player->m_position);

	//	移動入力によるキャンセル受付だよ☆
	if (animFlame >= MOVE_CANCEL_FLAME && (m_player->m_inputMove.x || m_player->m_inputMove.y))
	{
		m_player->m_playerState.ChangeState(StateID::Walk);
	}

	//	攻撃判定の生成＆削除だよ☆
	if (animFlame > ATTACK_END_FLAME && m_attackFlag)
	{
		m_attackCollider.SetActive(false);
	}
	if (animFlame > ATTACK_START_FLAME && !m_attackFlag)
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

void Player::PlayerAttack::OnInputCamera(DirectX::XMFLOAT2 input)
{
	m_player->CameraRoll(input);
}

void Player::PlayerAttack::CameraAnim(float animFlame)
{
	animFlame;
	//return;
	//FlameProcess(0, 15, [&](float t)
	//	{
	//		m_player->m_camera.SetFovAngle(std::lerp(DEFAULT_FOV_ANGLE, MAX_FOV_ANGLE, t));
	//	});
	//FlameProcess(15, 22, [&](float t)
	//	{
	//		m_player->m_camera.SetFovAngle(std::lerp(MAX_FOV_ANGLE, DEFAULT_FOV_ANGLE, t));
	//	});
	//FlameProcess(0, 25, [&](float t)
	//	{
	//		t;
	//		DirectX::XMFLOAT3 rotate = m_player->m_camera.GetRotate();
	//		rotate.x = std::lerp(rotate.x, 0.0f, 0.05f);
	//		rotate.y = std::lerp(rotate.y, static_cast<float>(m_player->GetRotate().y - std::numbers::pi), 0.05f);
	//		m_player->m_camera.SetRotate(rotate);
	//	});

}

void Player::PlayerAttack::FlameProcess(float start, float end, std::function<void(float)> process) const
{
	float animFlame = m_animTimer / ANIMATION_FPS;
	if (start <= animFlame && animFlame <= end)
	{
		process((animFlame - start) / (end - start));
	}
}