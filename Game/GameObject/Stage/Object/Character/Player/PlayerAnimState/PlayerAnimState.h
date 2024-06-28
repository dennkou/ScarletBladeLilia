#pragma once
#include "../PlayerModel.h"

class PlayerModel::PlayerAnimState
{
public:
	PlayerAnimState() {};
	virtual ~PlayerAnimState() {};

	virtual void Enter() = 0;
	virtual void Update(float time) = 0;
	virtual void Exit() = 0;

	virtual float GetAnimTime() = 0;
	virtual float GetMaxFrame() = 0;

	virtual void Attack() {}
	virtual void Avoidance() {}
	virtual void Move(DirectX::XMFLOAT2 move) { move; }
private:

};