#include "PlayerTitle.h"

Player::PlayerTitle::PlayerTitle(Player* owner)
	:
	m_owner(owner)
{
	m_owner->m_hpUi.SetAlpha(0);
	m_owner->m_hpUi.SetPlayerHPPercent(0);

	m_owner->m_model.GetStandAnim().GetAnimation(0.0f, m_owner->m_model.GetBone(), m_owner->m_model.GetBoneMap());
}

Player::PlayerTitle::~PlayerTitle()
{
}

void Player::PlayerTitle::Enter()
{
	m_startFlag = false;
	m_animTimer = 0.0f;

	m_owner->m_camera.SetPosition(CAMERA_TITLE_POSITION);
	m_owner->m_camera.SetRotate(CAMERA_TITLE_ROTATE);
	m_owner->m_camera.SetDistance(CAMERA_TITLE_DISTANCE);
}

void Player::PlayerTitle::Exit()
{
	m_owner->m_hpUi.SetAlpha(1);
	m_owner->m_hpUi.SetPlayerHPPercent(m_owner->m_hp.GetHPPercent());

	m_owner->m_camera.SetPosition(CAMERA_PLAY_POSITION);
	m_owner->m_camera.SetRotate(CAMERA_PLAY_ROTATE);
	m_owner->m_camera.SetDistance(CAMERA_PLAY_DISTANCE);
}

void Player::PlayerTitle::Update(float time)
{
	m_owner->m_hpUi.SetAlpha(m_animTimer / CHANGE_ANIMATION_TIME);
	m_owner->m_hpUi.SetPlayerHPPercent(std::lerp(0.0f, m_owner->m_hp.GetHPPercent(), m_animTimer / CHANGE_ANIMATION_TIME));

	if (m_startFlag)
	{
		m_animTimer += time;								//	ステートタイマーの更新☆
		if (m_animTimer >= CHANGE_ANIMATION_TIME)
		{
			m_owner->m_playerState.ChangeState(StateID::Stand);
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
	float timeRatio = m_animTimer / CHANGE_ANIMATION_TIME;

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
