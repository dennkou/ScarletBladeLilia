#pragma once
#include "./PlayerDefault.h"
class Player::PlayerDefault::PlayerDefaultStateBase
{
public:
	PlayerDefaultStateBase() {};
	virtual ~PlayerDefaultStateBase() {};

	virtual void Enter() = 0;
	virtual void Update(float time) = 0;
	virtual void Exit() = 0;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) { input; };
};