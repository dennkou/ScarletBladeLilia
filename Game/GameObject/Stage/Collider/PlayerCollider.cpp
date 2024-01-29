#include "PlayerCollider.h"

ColliderSystem::PlayerCollider::PlayerCollider(DirectX::XMFLOAT3 top, DirectX::XMFLOAT3 down, float radius)
	:
	m_top(top),
	m_down(down),
	m_radius(radius)
{

}

ColliderSystem::PlayerCollider::~PlayerCollider()
{

}

void ColliderSystem::PlayerCollider::Update()
{

}

void ColliderSystem::PlayerCollider::SetPause(DirectX::XMFLOAT3 top, DirectX::XMFLOAT3 down)
{
	m_top = top;
	m_down = down;
}

void ColliderSystem::PlayerCollider::SetPosition(DirectX::XMFLOAT3 position)
{
	m_position = position;
}