#pragma once

//	HPŠÇ—ƒNƒ‰ƒX‚¾‚æ™

class HP
{
public:
	HP(float maxHp);
	~HP();
	
	void Damage(float damage);
	void HpRecover(float recoverNum);
	void SetMaxHp(float maxHp);
	constexpr bool IsDied() const noexcept { return m_hp <= 0.0f; };
	constexpr float GetHp() const noexcept { return m_hp; };
	constexpr float GetMaxHp() const noexcept { return m_maxHp; };
	constexpr float GetHPPercent() const noexcept
	{
		if (GetMaxHp() > 0.0f)
		{
			return GetHp() / GetMaxHp();
		}
		else
		{
			return 0.0f;
		}
	}
private:
	float m_hp;
	float m_maxHp;
};