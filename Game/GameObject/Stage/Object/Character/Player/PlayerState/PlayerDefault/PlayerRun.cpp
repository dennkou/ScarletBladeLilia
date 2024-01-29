#include "PlayerRun.h"
#include <algorithm>
#include <numbers>
#include "./../../Crown/Object/Algorithm.h"

Player::PlayerDefault::PlayerRun::PlayerRun(PlayerDefault* owner)
	:
	m_owner(owner),
	m_player(owner->m_owner)
{

}

Player::PlayerDefault::PlayerRun::~PlayerRun()
{

}

void Player::PlayerDefault::PlayerRun::Enter()
{
	m_player->m_stateTimer = 0.0f;
}

void Player::PlayerDefault::PlayerRun::Update(float time)
{
	//	プレイヤーのアニメーションだよ☆
	{
		if (m_player->m_stateTimer / ANIMATION_FPS <= m_player->m_runStartAnim.GetMaxFrame())
		{
			m_player->m_runStartAnim.GetAnimation(m_player->m_stateTimer / ANIMATION_FPS, m_player->m_bone, m_player->GetModel().GetBoneDate());
		}
		else
		{
			m_player->m_runAnim.GetAnimation((m_player->m_stateTimer / ANIMATION_FPS) + m_player->m_runStartAnim.GetMaxFrame(), m_player->m_bone, m_player->GetModel().GetBoneDate());
		}
	}

	//	プレイヤーの回転処理だよ☆
	{
		float rotate = m_player->GetModel().GetRotate().y;
		m_player->m_targetAngle = Crown::RotateNormalize(m_player->m_targetAngle);
		rotate = Crown::RotateNormalize(rotate);
		rotate += std::clamp(Crown::RotateNormalize(m_player->m_targetAngle - rotate), -ROLL_SPEED * time, ROLL_SPEED * time);

		m_player->GetModel().SetRotate(DirectX::XMFLOAT3(0, rotate, 0));
	}

	//	プレイヤーの移動処理だよ☆
	{
		float rotate = m_player->GetModel().GetRotate().y;
		DirectX::XMFLOAT3 position = m_player->GetModel().GetPosition();
		position.x -= sin(rotate) * SPEED * time;
		position.z -= cos(rotate) * SPEED * time;
		m_player->SetPosition(position);
	}
}

void Player::PlayerDefault::PlayerRun::Exit()
{

}