#pragma once
#include <DirectXMath.h>
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include "InstanceCollection.h"

class ColliderSystem::BulletCollider
{
public:
	BulletCollider()
		:
		m_position(),
		m_radius()
	{
	};
	~BulletCollider() {};

	inline constexpr void SetPosition(DirectX::XMFLOAT3 position) noexcept { m_position = position; }
	inline constexpr DirectX::XMFLOAT3 GetPosition() const noexcept { return m_position; }
	inline constexpr void SetRadius(float radius) noexcept { m_radius = radius; }
	inline constexpr float GetRadius() const noexcept { return m_radius; }
private:
	DirectX::XMFLOAT3 m_position;
	float m_radius;
};

