#include "HP.h"

HP::HP(float maxHp)
	:
	m_maxHp(maxHp),
	m_hp(maxHp)
{
}

HP::~HP()
{
}

void HP::Damage(float damage)
{
	if (damage < 0.0f)
	{
		return;
	}
	m_hp -= damage;
	if (m_hp < 0.0f)
	{
		m_hp = 0.0f;
	}
}

void HP::HpRecover(float recoverNum)
{
	if (recoverNum < 0.0)
	{
		return;
	}
	m_hp += recoverNum;
	if (m_hp > m_maxHp)
	{
		m_hp = m_maxHp;
	}
}

void HP::SetMaxHp(float maxHp)
{
	if (maxHp <= 0.0f)
	{
		return;
	}

	m_maxHp = maxHp;
	if (m_hp < m_maxHp)
	{
		m_hp = m_maxHp;
	}
	
}