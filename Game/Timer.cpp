#include "Timer.h"

Timer::Timer()
	:
	m_time(0.0f),
	m_enemyTimeRate(1.0f),
	m_worldTimeRate(1.0f),
	m_oldTimePoint(std::chrono::system_clock::now()),
	m_startPoint(std::chrono::system_clock::now()),
	m_startFlag(false)
{

}

Timer::~Timer()
{
}

void Timer::Update()
{
	if (!m_startFlag)
	{
		m_oldTimePoint = std::chrono::system_clock::now();
		m_startFlag = true;
	}
	else
	{
		m_time = static_cast<float>(duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_oldTimePoint).count());
	}

	//	1 / 1@•bˆÈã‚ÍØ‚èÌ‚Ä‚é‚æ™
	m_time = std::min(1.0f / 1.0f * 1000.0f, m_time);

	m_oldTimePoint = std::chrono::system_clock::now();
}