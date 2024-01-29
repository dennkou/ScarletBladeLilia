#include "Character.h"



Character::Character(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate, std::wstring modelName, int maxHp)
	:
	Object(game, position, rotate, modelName),
	m_maxHp(maxHp),
	m_hp(maxHp)
{
}

Character::~Character()
{

}

void Character::Damage(int damage)
{
	m_hp -= damage;
	if (m_hp < 0)
	{
		m_hp = 0;
	}
}

void Character::HpRecover(int recoverNum)
{
	m_hp += recoverNum;
	if (m_hp > m_maxHp)
	{
		m_hp = m_maxHp;
	}
}

void Character::Move(const DirectX::XMFLOAT3& position)
{
	SetPosition(position);
}

void Character::Rall(float angle)
{
	SetRotate(DirectX::XMFLOAT3(0, angle, 0));
}