#include "Bullet.h"
#include "./../../MathLibrary.h"

Bullet::Bullet(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 vector)
	:
	GameObject(game)
{
	m_moveVector = vector;
	m_position = position;
	m_collider.SetPosition(position);
	m_model.SetPosition(position);

	m_model.LoadPMX(L"Resource/Model/PMX/bullet.pmx");
}

Bullet::~Bullet()
{
}

void Bullet::OnGameUpdate(Timer& time)
{
	m_position = VectorAdd(m_position, VectorScale(m_moveVector, SPEED * time.GetEnemyTime()));
	m_model.SetPosition(m_position);
}