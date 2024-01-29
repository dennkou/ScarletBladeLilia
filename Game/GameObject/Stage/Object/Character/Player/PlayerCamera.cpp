#include "PlayerCamera.h"
#include "./../../Crown/Object/RenderSystem/Camera.h"
#include "./Player.h"



PlayerCamera::PlayerCamera()
	:
	m_position(),
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
	eye.x = (sin(m_rotate.y) *	cos(-m_rotate.x) * (-m_distance)) + m_position.x + POSITION_OFFSET.x;
	eye.y = (					sin(-m_rotate.x) * (-m_distance)) + m_position.y + POSITION_OFFSET.y;
	eye.z = (cos(m_rotate.y) *	cos(-m_rotate.x) * (-m_distance)) + m_position.z + POSITION_OFFSET.z;

	Crown::RenderObject::Camera::GetInstance()->SetEye(eye);
	Crown::RenderObject::Camera::GetInstance()->SetRotate(m_rotate);
	Crown::RenderObject::Camera::GetInstance()->SetFovAngle(m_fovAngle);
}