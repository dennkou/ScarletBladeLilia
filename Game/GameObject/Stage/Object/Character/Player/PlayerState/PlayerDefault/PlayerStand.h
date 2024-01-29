#pragma once
#include "./PlayerDefaultStateBase.h"
class Player::PlayerDefault::PlayerStand : public Player::PlayerDefault::PlayerDefaultStateBase
{
public:
	PlayerStand(PlayerDefault* owner);
	virtual ~PlayerStand();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;
private:
	Player* m_player;
	PlayerDefault* m_owner;
};