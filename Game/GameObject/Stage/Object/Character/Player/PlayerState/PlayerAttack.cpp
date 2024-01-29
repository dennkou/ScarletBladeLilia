#include "PlayerAttack.h"
#include <numbers>
#include "./../../Crown/Object/Algorithm.h"

Player::PlayerAttack::PlayerAttack(Player* player)
	:
	m_player(player)
{

}

Player::PlayerAttack::~PlayerAttack()
{

}

void Player::PlayerAttack::Enter()
{
	m_player->m_stateTimer = 0;
	m_startPosition = m_player->GetPosition();
}

void Player::PlayerAttack::Update(float time)
{
	m_player->m_stateTimer += time;
	float animFlame = m_player->m_stateTimer / ANIMATION_FPS;
	m_player->m_drawingSwordAttackAnim.GetAnimation(animFlame, m_player->m_bone, m_player->GetModel().GetBoneDate());

	//	アニメーション終了時デフォルトに戻るよ☆
	if (animFlame > m_player->m_drawingSwordAttackAnim.GetMaxFrame())
	{
		m_player->m_playerState.ChangeState(StateID::Normal);
	}

	//	キャラクターを移動させるよ☆
	FlameProcess(0, MOVING_END_FLAME, [&](float t)
		{
			float rotate = m_player->GetModel().GetRotate().y;
			DirectX::XMFLOAT3 position = m_player->GetModel().GetPosition();
			position.x -= sin(rotate) * MOVING_DISTANCE / MOVING_END_FLAME * time;
			position.z -= cos(rotate) * MOVING_DISTANCE / MOVING_END_FLAME * time;
			m_player->SetPosition(position);
		});

	CameraAnim(animFlame);

	m_player->m_camera.SetPosition(m_player->GetPosition());

	//	移動入力によるキャンセル受付だよ☆
	if (animFlame >= MOVE_CANCEL_FLAME && (m_player->m_inputMove.x || m_player->m_inputMove.y))
	{
		m_player->m_playerState.ChangeState(StateID::Normal);
	}

}

void Player::PlayerAttack::Exit()
{
	m_player->m_stateTimer = 0;
	m_player->m_camera.SetFovAngle(DEFAULT_FOV_ANGLE);
}

void Player::PlayerAttack::OnInputMove(DirectX::XMFLOAT2 input)
{

}

void Player::PlayerAttack::CameraAnim(float animFlame)
{
	FlameProcess(0, 15, [&](float t)
		{
			m_player->m_camera.SetFovAngle(std::lerp(DEFAULT_FOV_ANGLE, MAX_FOV_ANGLE, t));
		});
	FlameProcess(15, 22, [&](float t)
		{
			m_player->m_camera.SetFovAngle(std::lerp(MAX_FOV_ANGLE, DEFAULT_FOV_ANGLE, t));
		});

	FlameProcess(0, 25, [&](float t)
		{
			DirectX::XMFLOAT3 rotate = m_player->m_camera.GetRotate();
			rotate.x = std::lerp(rotate.x, 0, 0.05f);
			rotate.y = std::lerp(rotate.y, m_player->GetRotate().y - std::numbers::pi, 0.05f);
			m_player->m_camera.SetRotate(rotate);
		});

}

void Player::PlayerAttack::FlameProcess(float start, float end, std::function<void(float)> process)
{
	float animFlame = m_player->m_stateTimer / ANIMATION_FPS;
	if (start <= animFlame && animFlame <= end)
	{
		process((animFlame - start) / (end - start));
	}
}