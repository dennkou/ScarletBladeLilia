#pragma once
#include "PlayerAnimState.h"
class PlayerModel::PlayerAnimStandToWalk : public PlayerModel::PlayerAnimState
{
public:
	PlayerAnimStandToWalk(PlayerModel* model);
	virtual ~PlayerAnimStandToWalk();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;

	virtual float GetAnimTime() override;
	virtual float GetMaxFrame() override;
private:
	PlayerModel* m_owner;
	float m_timer;
	Crown::RenderObject::AnimationData m_standToWalkAnim;
};