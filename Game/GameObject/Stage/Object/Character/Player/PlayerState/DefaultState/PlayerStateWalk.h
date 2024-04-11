#pragma once
#include "DefaultStateBase.h"

class Player::PlayerDefault::Walk : public Player::PlayerDefault::State
{
public:
	Walk(PlayerDefault* owner);
	virtual ~Walk();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	ˆÚ“®‚Ì“ü—Í‘€ì‚ª‚ ‚Á‚½‚æ™
	virtual void OnInputDash(bool input) override;
private:
	static constexpr float ROLL_SPEED = 0.1f;
	Player& m_player;
	PlayerDefault& m_owner;
	float m_timer;
	DirectX::XMFLOAT2 inputVector;
	bool m_end;
};