#pragma once
#include <DirectXMath.h>
#include "ColliderSystem.h"



class ColliderSystem::PlayerCollider
{
public:
	PlayerCollider(DirectX::XMFLOAT3 top, DirectX::XMFLOAT3 down, float radius);
	~PlayerCollider();

	void Update();
	void SetPause(DirectX::XMFLOAT3 top, DirectX::XMFLOAT3 down);
	void SetPosition(DirectX::XMFLOAT3 position);

	inline float GetRadius() { return m_radius; }
	inline DirectX::XMFLOAT3 GetTop() { return m_top; }
	inline DirectX::XMFLOAT3 GetDown() { return m_down; }
	inline float GetMoveRotate() { return m_moveRotate; }
	inline DirectX::XMFLOAT3 GetPosition() { return m_position; }
private:
	DirectX::XMFLOAT3 m_top;
	DirectX::XMFLOAT3 m_down;
	float m_radius;
	DirectX::XMFLOAT3 m_position;
	float m_moveRotate;
};