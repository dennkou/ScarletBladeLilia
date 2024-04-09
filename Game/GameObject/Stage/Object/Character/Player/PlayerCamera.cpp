#include "PlayerCamera.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./Player.h"



PlayerCamera::PlayerCamera()
	:
	m_position(),
	m_center(),
	m_rotate(),
	m_distance(),
	m_fovAngle(1.0471976f)
{

}

PlayerCamera::~PlayerCamera()
{

}

void PlayerCamera::UpdateCameraTransform()
{
	DirectX::XMFLOAT3 eye;
	eye.x = (sin(m_rotate.y) *	cos(-m_rotate.x) * (-m_distance)) + m_center.x + POSITION_OFFSET.x;
	eye.y = (					sin(-m_rotate.x) * (-m_distance)) + m_center.y + POSITION_OFFSET.y;
	eye.z = (cos(m_rotate.y) *	cos(-m_rotate.x) * (-m_distance)) + m_center.z + POSITION_OFFSET.z;

	m_center.x += (m_position.x - m_center.x) * 0.05f;
	m_center.y += (m_position.y - m_center.y) * 0.05f;
	m_center.z += (m_position.z - m_center.z) * 0.05f;

	Crown::RenderObject::Camera::GetInstance()->SetEye(eye);
	Crown::RenderObject::Camera::GetInstance()->SetRotate(m_rotate);
	Crown::RenderObject::Camera::GetInstance()->SetFovAngle(m_fovAngle);
}