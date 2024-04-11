#pragma once
#include "./PlayerState.h"
class Player::PlayerTitle : public Player::PlayerState
{
public:
	PlayerTitle(Player* owner);
	virtual ~PlayerTitle() override;

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;
	virtual void OnPlayStartAnimation() override;
private:
	void CameraUpdate();

	static constexpr DirectX::XMFLOAT3 CAMERA_TITLE_POSITION = DirectX::XMFLOAT3(0.0f, -2.0f, -5.0f);
	static constexpr DirectX::XMFLOAT3 CAMERA_TITLE_ROTATE = DirectX::XMFLOAT3(0.0f, -0.8f, 0.0f);
	static constexpr float CAMERA_TITLE_DISTANCE = 5.0f;

	static constexpr float CHANGE_ANIMATION_TIME = 1500.0f;

	static constexpr DirectX::XMFLOAT3 CAMERA_PLAY_POSITION = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	static constexpr DirectX::XMFLOAT3 CAMERA_PLAY_ROTATE = DirectX::XMFLOAT3(0.0f, -3.1415926535897932384626433832795f, 0.0f);
	static constexpr float CAMERA_PLAY_DISTANCE = 25.0f;
	Player* m_owner;
	bool m_startFlag;
	float m_timer;
};