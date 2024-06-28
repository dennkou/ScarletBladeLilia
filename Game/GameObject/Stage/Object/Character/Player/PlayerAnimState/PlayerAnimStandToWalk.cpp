#include "PlayerAnimStandToWalk.h"

PlayerModel::PlayerAnimStandToWalk::PlayerAnimStandToWalk(PlayerModel* model)
{
	m_owner = model;
	m_timer = 0;
	m_standToWalkAnim.LaodVMD(L"Resource/Motion/•à‚«Žn‚ß.vmd");
}

PlayerModel::PlayerAnimStandToWalk::~PlayerAnimStandToWalk()
{
}

void PlayerModel::PlayerAnimStandToWalk::Enter()
{
	m_timer = 0;
}

void PlayerModel::PlayerAnimStandToWalk::Update(float time)
{
	m_timer += time / ANIMATION_FPS;
	m_standToWalkAnim.GetAnimation(m_timer, m_owner->m_bone, m_owner->m_model.GetBoneDate());
	if (m_timer > m_standToWalkAnim.GetMaxFrame())
	{
		m_owner->m_state.ChangeState(AnimState::Walk);
	}
}

void PlayerModel::PlayerAnimStandToWalk::Exit()
{
}

float PlayerModel::PlayerAnimStandToWalk::GetAnimTime()
{
	return m_timer;
}

float PlayerModel::PlayerAnimStandToWalk::GetMaxFrame()
{
	return m_standToWalkAnim.GetMaxFrame();
}
