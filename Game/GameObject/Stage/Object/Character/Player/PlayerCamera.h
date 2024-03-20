#pragma once
#include <DirectXMath.h>
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"

class Player;

class PlayerCamera
{
public:
	PlayerCamera();
	~PlayerCamera();

	inline DirectX::XMFLOAT3 GetRotate() const noexcept { return m_rotate; }
	inline void SetRotate(DirectX::XMFLOAT3 rotate) noexcept { m_rotate = rotate; }
	inline DirectX::XMFLOAT3 GetPosition() const noexcept { return m_position; }
	inline void SetPosition(DirectX::XMFLOAT3 position) noexcept { m_position = position; }
	inline float GetDistance() const noexcept { return m_distance; }
	inline void SetDistance(float distance) noexcept { m_distance = distance; }
	inline float GetFovAngle() const noexcept { return m_fovAngle; }
	inline void SetFovAngle(float fovAngle) noexcept { m_fovAngle = fovAngle; }

	void UpdateCameraTransform();
private:

	static constexpr float DEFAULT_FOV_ANGLE = DirectX::XMConvertToRadians(60);
	static constexpr DirectX::XMFLOAT3 POSITION_OFFSET = DirectX::XMFLOAT3(0, 13, 0);

	float m_distance;
	DirectX::XMFLOAT3 m_rotate;
	DirectX::XMFLOAT3 m_position;
	float m_fovAngle;
};