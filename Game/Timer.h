#pragma once
#include <chrono>
#include <memory>
class Timer
{
public:
	Timer();
	~Timer();

	void Update();

	inline float GetSystemTime() const noexcept { return m_time;  }
	inline float GetEnemyTime() const noexcept { return m_time * m_enemyTimeRate * m_worldTimeRate; }
	inline float GetWorldTime() const noexcept { return m_time * m_worldTimeRate; }
	inline void SetEnemyTimeRate(float enemyTimeRate) noexcept { m_enemyTimeRate = enemyTimeRate; }
	inline void SetWorldTimeRate(float worldTimeRate) noexcept { m_worldTimeRate = worldTimeRate; }
	inline long long GetTotalTime() const { return static_cast<long long>(duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_startPoint).count()); }
private:
	std::chrono::system_clock::time_point m_oldTimePoint;	//	前回の更新時間だよ☆
	std::chrono::system_clock::time_point m_startPoint;		//	ゲームの開始時間だよ☆
	bool m_startFlag;
	float m_time;											//	更新にかかった時間だよ☆
	float m_enemyTimeRate;									//	主に敵キャラクターが使用する時間のずれだよ☆
	float m_worldTimeRate;									//	ゲーム全体の時間のずれだよ☆
};