#include "Enemy.h"

Enemy::Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate)
	:
	Character(game, positon, rotate, L"Resource/Model/PMX/enemy.pmx", MAX_HP),
	m_enemyCollider([&](int damage) { HitPlayerAttack(damage); })
{
	m_enemyCollider.SetRadius(2.0f);
	m_enemyCollider.SetCapsuleVector(DirectX::XMFLOAT3(0,20,0));
	m_enemyCollider.SetPosition(GetPosition());
}

Enemy::~Enemy()
{

}

void Enemy::OnGameUpdate(Timer& timer)
{
	timer;

	SetPosition(m_enemyCollider.GetPosition());
}

void Enemy::HitPlayerAttack(int damage)
{
	Damage(damage);

	std::cout << "“–‚½‚Á‚½[™" << std::endl;

	if (!GetHp())
	{
		EventTrigger(&GameObject::OnEnemyDied, this);

	}
}