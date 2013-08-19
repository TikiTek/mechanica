#pragma once
#ifndef TIKI_TIMESYSTEM_HPP
#define TIKI_TIMESYSTEM_HPP

#include <Windows.h>

namespace tiki
{
	class TimeSystem
	{
	public:
								TimeSystem();
								~TimeSystem();

		void					initzilize();
		void					update();

		double					getElapsedTime() const { return m_elapsedTime; };
		double					getTotalTime() const { return m_totalTime; };

		void					setTimeScale( const double timeScale ) { m_timeScale = timeScale; }

	private:
		LARGE_INTEGER			m_freq;
		LARGE_INTEGER			m_last;
		LARGE_INTEGER			m_current;

		double					m_elapsedTime;
		double					m_totalTime;
		double					m_timeScale;

	};
}

#endif