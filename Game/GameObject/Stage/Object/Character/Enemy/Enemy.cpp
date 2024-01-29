#include "Enemy.h"

Enemy::Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate)
	:
	Character(game, positon, rotate, L"Resource/Model/PMX/enemy.pmx", MAX_HP)
{

}

Enemy::~Enemy()
{

}

void Enemy::OnGameUpdate(Timer& timer)
{
	timer;
}