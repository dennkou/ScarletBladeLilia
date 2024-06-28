#pragma once
#include "PlayerAnimState.h"

class PlayerModel::PlayerAnimStand : public PlayerModel::PlayerAnimState
{
public:
	PlayerAnimStand(PlayerModel* model);
	virtual ~PlayerAnimStand();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;

	virtual float GetAnimTime() override { return m_timer; }
	virtual float GetMaxFrame() override { return m_standAnim.GetMaxFrame(); }

	virtual void Move(DirectX::XMFLOAT2 move) override;
private:
	PlayerModel* m_owner;
	float m_timer;
	Crown::RenderObject::AnimationData m_standAnim;
};