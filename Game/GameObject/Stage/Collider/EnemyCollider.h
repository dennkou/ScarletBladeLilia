#pragma once
#include "ColliderSystem.h"

class ColliderSystem::EnemyCollider
{
public:
	EnemyCollider();
	virtual ~EnemyCollider();

	

	void SetPosition(DirectX::XMFLOAT3 position) noexcept { m_position = position; }
	DirectX::XMFLOAT3 GetPosition() const noexcept { return m_position; }
private:
	DirectX::XMFLOAT3 m_position;
};