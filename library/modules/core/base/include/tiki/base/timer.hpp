#pragma once
#ifndef TIKI_TIMESYSTEM_HPP
#define TIKI_TIMESYSTEM_HPP

#include "tiki/base/types.hpp"

namespace tiki
{
	class Timer
	{
	public:

		void		create();

		void		update();

		double		getElapsedTime() const	{ return m_elapsedTime; };
		double		getTotalTime() const	{ return m_totalTime; };

		void		setTimeScale( const double timeScale ) { m_timeScale = timeScale; }

	private:

		struct TimeStemp
		{
			uint64 time;
		};

		TimeStemp	m_freq;
		TimeStemp	m_last;
		TimeStemp	m_current;

		double		m_elapsedTime;
		double		m_totalTime;
		double		m_timeScale;

	};
}

#endif