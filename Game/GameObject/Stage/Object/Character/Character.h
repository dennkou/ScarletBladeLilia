#pragma once
#include "./../Object.h"
class Character : public Object
{
public:
	Character(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate, std::wstring modelName, int maxHp);
	virtual ~Character() override;

protected:
	virtual void Damage(int damage);
	virtual void HpRecover(int recoverNum);
	virtual bool IsDied() const noexcept { return !m_hp; };
	virtual int GetHp() const noexcept { return m_hp; };
	virtual int GetMaxHp() const noexcept { return m_maxHp; };
	virtual void Move(const DirectX::XMFLOAT3& position);
	virtual void Rall(float angle);
private:
	int m_maxHp;
	int m_hp;
};