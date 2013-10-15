
#include "tiki/base/timer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/base/types.hpp"

#include <windows.h>

TIKI_DEBUGPROP_FLOAT( s_globalTimeScale, "GlobalTimeScale", 1.0f, 0.0001f, 10.0f );
TIKI_DEBUGPROP_FLOAT( s_maxFrameTime, "MaxFrameTime", 1.0f, 0.0001f, 100.0f );

namespace tiki
{
	void Timer::create()
	{
		TIKI_COMPILETIME_ASSERT( sizeof( LARGE_INTEGER ) == sizeof( ::tiki::Timer::TimeStemp ) );

		QueryPerformanceFrequency( (LARGE_INTEGER*)&m_freq );
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_last );
		
		m_elapsedTime	= 0.0;
		m_totalTime		= 0.0;
		m_timeScale		= 1.0;
	}

	void Timer::update()
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_current );

		m_elapsedTime = (double)(m_current.time - m_last.time) / m_freq.time;
		m_elapsedTime = (m_elapsedTime > s_maxFrameTime ? s_maxFrameTime : m_elapsedTime) * m_timeScale * s_globalTimeScale;
		
		m_totalTime += m_elapsedTime;

		m_last = m_current;
	}
}