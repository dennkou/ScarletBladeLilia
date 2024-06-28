#include "PlayerAnimStand.h"

PlayerModel::PlayerAnimStand::PlayerAnimStand(PlayerModel* model)
{
	m_owner = model;
	m_timer = 0;
	m_standAnim.LaodVMD(L"Resource/Motion/‘Ò‹@.vmd");
}

PlayerModel::PlayerAnimStand::~PlayerAnimStand()
{

}

void PlayerModel::PlayerAnimStand::Enter()
{
	m_timer = 0;
}

void PlayerModel::PlayerAnimStand::Update(float time)
{
	m_timer += time / ANIMATION_FPS;
	m_standAnim.GetAnimation(m_timer, m_owner->m_bone, m_owner->m_model.GetBoneDate());
}

void PlayerModel::PlayerAnimStand::Exit()
{

}

void PlayerModel::PlayerAnimStand::Move(DirectX::XMFLOAT2 move)
{
	m_owner->m_state.ChangeState(AnimState::StandToWalk);
}