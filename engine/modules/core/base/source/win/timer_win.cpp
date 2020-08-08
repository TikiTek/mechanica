#include "tiki/base/timer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/debug_property.hpp"
#include "tiki/base/types.hpp"

#include <windows.h>

namespace tiki
{
	TIKI_DEBUGPROP_FLOAT( s_globalTimeScale, "GlobalTimeScale", 1.0f, 0.0001f, 10.0f );
	TIKI_DEBUGPROP_FLOAT( s_maxFrameTime, "MaxFrameTime", 1.0f, 0.0001f, 100.0f );

	void Timer::create()
	{
		TIKI_COMPILETIME_ASSERT( sizeof( LARGE_INTEGER ) == sizeof( ::tiki::Timer::TimeStamp ) );

		QueryPerformanceFrequency( (LARGE_INTEGER*)&m_freq );
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_last );

		m_time.elapsedTime	= 0.0;
		m_time.totalTime	= 0.0;
		m_timeScale			= 1.0;
	}

	void Timer::update()
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_current );

		m_time.elapsedTime = (double)(m_current.time - m_last.time) / m_freq.time;
		m_time.elapsedTime = (m_time.elapsedTime > s_maxFrameTime ? s_maxFrameTime : m_time.elapsedTime) * m_timeScale * s_globalTimeScale;

		m_time.totalTime += m_time.elapsedTime;

		m_last = m_current;
	}
}