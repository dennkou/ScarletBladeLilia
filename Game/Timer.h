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
	std::chrono::system_clock::time_point m_oldTimePoint;	//	�O��̍X�V���Ԃ��恙
	std::chrono::system_clock::time_point m_startPoint;		//	�Q�[���̊J�n���Ԃ��恙
	bool m_startFlag;
	float m_time;											//	�X�V�ɂ����������Ԃ��恙
	float m_enemyTimeRate;									//	��ɓG�L�����N�^�[���g�p���鎞�Ԃ̂��ꂾ�恙
	float m_worldTimeRate;									//	�Q�[���S�̂̎��Ԃ̂��ꂾ�恙
};