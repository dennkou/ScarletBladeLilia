#include "PlayerAnimWalk.h"

PlayerModel::PlayerAnimWalk::PlayerAnimWalk(PlayerModel* owner)
{
	m_owner = owner; 
	m_timer = 0;
	m_walkAnim.LaodVMD(L"Resource/Motion/•à‚«.vmd");
}

PlayerModel::PlayerAnimWalk::~PlayerAnimWalk()
{
}

void PlayerModel::PlayerAnimWalk::Enter()
{
	m_timer = 0;
}

void PlayerModel::PlayerAnimWalk::Update(float time)
{
	m_timer += time / ANIMATION_FPS;
	m_walkAnim.GetAnimation(m_timer, m_owner->m_bone, m_owner->m_model.GetBoneDate());
}

void PlayerModel::PlayerAnimWalk::Exit()
{

}

float PlayerModel::PlayerAnimWalk::GetAnimTime()
{
	return m_timer;
}

float PlayerModel::PlayerAnimWalk::GetMaxFrame()
{
	return m_walkAnim.GetMaxFrame();
}

void PlayerModel::PlayerAnimWalk::Attack()
{

}

void PlayerModel::PlayerAnimWalk::Avoidance()
{

}

void PlayerModel::PlayerAnimWalk::Move(DirectX::XMFLOAT2 move)
{

}