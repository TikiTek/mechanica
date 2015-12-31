
#include "tiki/base/timer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/base/types.hpp"

#include <time.h>

TIKI_DEBUGPROP_FLOAT( s_globalTimeScale, "GlobalTimeScale", 1.0f, 0.0001f, 10.0f );
TIKI_DEBUGPROP_FLOAT( s_maxFrameTime, "MaxFrameTime", 1.0f, 0.0001f, 100.0f );

namespace tiki
{
	void Timer::create()
	{
		TIKI_COMPILETIME_ASSERT( sizeof( timespec ) == sizeof( ::tiki::Timer::TimeStamp ) );

		clock_gettime( CLOCK_REALTIME, (timespec*)&m_last );
		
		m_elapsedTime	= 0.0;
		m_totalTime	= 0.0;
		m_timeScale	= 1.0;
	}

	void Timer::update()
	{
		clock_gettime( CLOCK_REALTIME, (timespec*)&m_current );

		m_elapsedTime = ( m_current.timeSec - m_last.timeSec ) + ( m_current.timeMS - m_last.timeMS ) / 1000000L;
		//m_elapsedTime = (double)(m_current.time - m_last.time) / m_freq.time;
		m_elapsedTime = (m_elapsedTime > s_maxFrameTime ? s_maxFrameTime : m_elapsedTime) * m_timeScale * s_globalTimeScale;
		
		m_totalTime += m_elapsedTime;

		m_last = m_current;
	}
}