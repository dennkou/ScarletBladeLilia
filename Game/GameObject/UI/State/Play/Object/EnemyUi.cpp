#include "EnemyUi.h"

EnemyUi::EnemyUi(int maxHp)
	:
	m_maxHp(maxHp)
{

}

EnemyUi::~EnemyUi()
{

}

void EnemyUi::Render(ID3D12GraphicsCommandList* commandList)
{

}

void EnemyUi::SetMaxHp(int maxHp)
{
	m_maxHp = maxHp;
}

void EnemyUi::SetHp(int hp)
{

}