#include "PlayerTitle.h"

Player::PlayerTitle::PlayerTitle(Player* owner)
	:
	m_owner(owner)
{
	m_owner->m_hpUi.SetAlpha(0);
	m_owner->m_hpUi.SetPlayerHPPercent(0);
}

Player::PlayerTitle::~PlayerTitle()
{
}

void Player::PlayerTitle::Enter()
{
	m_startFlag = false;
	m_timer = 0.0f;

	m_owner->m_camera.SetPosition(CAMERA_TITLE_POSITION);
	m_owner->m_camera.SetRotate(CAMERA_TITLE_ROTATE);
	m_owner->m_camera.SetDistance(CAMERA_TITLE_DISTANCE);
}

void Player::PlayerTitle::Exit()
{
	m_owner->m_hpUi.SetAlpha(1);
	m_owner->m_hpUi.SetPlayerHPPercent(static_cast<float>(m_owner->GetHp()) / m_owner->GetMaxHp());

	m_owner->m_camera.SetPosition(CAMERA_PLAY_POSITION);
	m_owner->m_camera.SetRotate(CAMERA_PLAY_ROTATE);
	m_owner->m_camera.SetDistance(CAMERA_PLAY_DISTANCE);
}

void Player::PlayerTitle::Update(float time)
{
	m_owner->m_hpUi.SetAlpha(m_timer / CHANGE_ANIMATION_TIME);
	m_owner->m_hpUi.SetPlayerHPPercent(std::lerp(0, m_owner->GetHp() / m_owner->GetMaxHp(), m_timer / CHANGE_ANIMATION_TIME));

	m_owner->m_standAnim.GetAnimation(0.0f, m_owner->m_bone, m_owner->GetModel().GetBoneDate());
	if (m_startFlag)
	{
		m_timer += time;								//	ステートタイマーの更新☆
		if (m_timer >= CHANGE_ANIMATION_TIME)
		{
			m_owner->m_playerState.ChangeState(StateID::Normal);
			m_owner->EventTrigger(&Game::GameObject::OnPlayStart);
		}
		else
		{
			CameraUpdate();
		}
	}
}

void Player::PlayerTitle::OnPlayStartAnimation()
{
	m_startFlag = true;
}

void Player::PlayerTitle::CameraUpdate()
{
	float timeRatio = m_timer / CHANGE_ANIMATION_TIME;

	DirectX::XMFLOAT3 tmp;
	tmp.x = std::lerp(CAMERA_TITLE_POSITION.x, CAMERA_PLAY_POSITION.x, timeRatio);
	tmp.y = std::lerp(CAMERA_TITLE_POSITION.y, CAMERA_PLAY_POSITION.y, timeRatio);
	tmp.z = std::lerp(CAMERA_TITLE_POSITION.z, CAMERA_PLAY_POSITION.z, timeRatio);
	m_owner->m_camera.SetPosition(tmp);

	tmp.x = std::lerp(CAMERA_TITLE_ROTATE.x, CAMERA_PLAY_ROTATE.x, timeRatio);
	tmp.y = std::lerp(CAMERA_TITLE_ROTATE.y, CAMERA_PLAY_ROTATE.y, timeRatio);
	tmp.z = std::lerp(CAMERA_TITLE_ROTATE.z, CAMERA_PLAY_ROTATE.z, timeRatio);
	m_owner->m_camera.SetRotate(tmp);

	m_owner->m_camera.SetDistance(std::lerp(CAMERA_TITLE_DISTANCE, CAMERA_PLAY_DISTANCE, timeRatio));
}
