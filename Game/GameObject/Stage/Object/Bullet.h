#pragma once
#include "./../../GameObject.h"
#include "./../Collider/BulletCollider.h"
#include "./../../Crown/Object/RenderSystem/Model/Model.h"

class Bullet : public Game::GameObject
{
public:
	Bullet(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 vector);
	virtual ~Bullet();

	virtual void OnGameUpdate(Timer& time) override;
private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_moveVector;
	ColliderSystem::BulletCollider m_collider;
	Crown::RenderObject::Model m_model;

	static constexpr float SPEED = 1.0f;
};