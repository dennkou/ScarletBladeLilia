#pragma once
#include "PlayerAnimState.h"

class PlayerModel::PlayerAnimWalk : public PlayerModel::PlayerAnimState
{
public:
	PlayerAnimWalk(PlayerModel* owner);
	virtual ~PlayerAnimWalk();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;

	virtual float GetAnimTime() override;
	virtual float GetMaxFrame() override;

	virtual void Attack();
	virtual void Avoidance();
	virtual void Move(DirectX::XMFLOAT2 move);
private:
	PlayerModel* m_owner;
	float m_timer;
	Crown::RenderObject::AnimationData m_walkAnim;
};