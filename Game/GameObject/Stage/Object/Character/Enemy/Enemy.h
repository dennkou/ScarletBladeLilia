#pragma once
#include "./../Character.h"

class Enemy : public Character
{
public:
	Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate);
	virtual ~Enemy();

	virtual void OnGameUpdate(Timer& timer) override;
private:
	static constexpr int MAX_HP = 1000;
};