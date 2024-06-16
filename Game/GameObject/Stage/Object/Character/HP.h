#pragma once

//	HP�Ǘ��N���X���恙

class HP
{
public:
	HP(float maxHp);
	~HP();
	
	void Damage(float damage);
	void HpRecover(float recoverNum);
	void SetMaxHp(float maxHp);
	inline constexpr bool IsDied() const noexcept { return m_hp <= 0.0f; };
	inline constexpr float GetHp() const noexcept { return m_hp; };
	inline constexpr float GetMaxHp() const noexcept { return m_maxHp; };
	inline constexpr float GetHPPercent() const noexcept
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