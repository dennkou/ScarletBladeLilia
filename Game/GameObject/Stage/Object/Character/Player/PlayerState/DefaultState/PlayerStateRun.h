#pragma once
#include "DefaultStateBase.h"
class Player::PlayerDefault::Run : public Player::PlayerDefault::State
{
public:
	Run(PlayerDefault* owner);
	virtual ~Run();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	ˆÚ“®‚Ì“ü—Í‘€ì‚ª‚ ‚Á‚½‚æ™
	virtual void OnInputDash(bool input) override;
private:
	PlayerDefault& m_owner;
	Player& m_player;
	static constexpr float ROLL_SPEED = 0.05f;
	DirectX::XMFLOAT2 inputVector;

	float m_timer;
};