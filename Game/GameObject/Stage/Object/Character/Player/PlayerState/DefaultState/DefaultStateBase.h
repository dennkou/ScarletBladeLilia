#pragma once
#include "./../PlayerDefault.h"

class Player::PlayerDefault::State
{
public:
	State() = default;
	virtual ~State() = default;

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update(float time) = 0;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) = 0;		//	ˆÚ“®‚Ì“ü—Í‘€ì‚ª‚ ‚Á‚½‚æ™
	virtual void OnInputDash(bool input) = 0;
private:
};