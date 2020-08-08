#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	struct GameTime
	{
		double	elapsedTime;
		double	totalTime;
	};

	class Timer
	{
	public:

		void		create();

		void		update();

		GameTime	getTime() const			{ return m_time; }
		double		getElapsedTime() const	{ return m_time.elapsedTime; };
		double		getTotalTime() const	{ return m_time.totalTime; };

		void		setTimeScale( const double timeScale ) { m_timeScale = timeScale; }

	private:

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		struct TimeStamp
		{
			uint64 time;
		};
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
		struct TimeStamp
		{
			long int timeSec;
			long int timeMS;
		};
#else
#	error "Platform not supported"
#endif

		TimeStamp	m_freq;
		TimeStamp	m_last;
		TimeStamp	m_current;

		GameTime	m_time;
		double		m_timeScale;
	};
}
