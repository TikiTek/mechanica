#include "tiki/gamecore/timesystem.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/base/types.hpp"

#include <time.h>

TIKI_DEBUGPROP_FLOAT( s_globalTimeScale, "GlobalTimeScale", 1.0f, 0.0001f, 10.0f );
TIKI_DEBUGPROP_FLOAT( s_maxFrameTime, "MaxFrameTime", 1.0f, 0.0001f, 100.0f );

namespace tiki
{
	TimeSystem::TimeSystem()
	{
	};

	TimeSystem::~TimeSystem()
	{
	};

	void TimeSystem::initzilize()
	{
		QueryPerformanceFrequency(&m_freq);
		QueryPerformanceCounter(&m_last);
		
		srand( (unsigned int)time( nullptr ) );

		m_totalTime = 0.0;
		m_timeScale	= 1.0;
	}

	void TimeSystem::update()
	{
		QueryPerformanceCounter(&m_current);
		m_elapsedTime = (double)(m_current.QuadPart - m_last.QuadPart) / m_freq.QuadPart;
		m_elapsedTime = (m_elapsedTime > s_maxFrameTime ? s_maxFrameTime : m_elapsedTime) * m_timeScale * s_globalTimeScale;
		
		m_totalTime += m_elapsedTime;

		m_last = m_current;
	}
}