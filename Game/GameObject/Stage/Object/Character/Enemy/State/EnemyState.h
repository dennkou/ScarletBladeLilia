#pragma once
class EnemyState
{
public:
	EnemyState() = default;
	~EnemyState() = default;
private:
	virtual void Update() = 0;
	virtual void OnDamage(int damage) = 0;
};