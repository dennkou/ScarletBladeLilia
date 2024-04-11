#pragma once
#include "DefaultStateBase.h"
class Player::PlayerDefault::Stand : public Player::PlayerDefault::State
{
public:
	Stand(PlayerDefault* owner);
	virtual ~Stand();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	移動の入力操作があったよ☆
	virtual void OnInputDash(bool input) override;
private:
	Player& m_player;
	PlayerDefault& m_owner;
	float m_timer;
};