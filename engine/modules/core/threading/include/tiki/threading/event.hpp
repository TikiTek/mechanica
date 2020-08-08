#pragma once
#ifndef __TIKI_EVENT_HPP_INCLUDED__
#define __TIKI_EVENT_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/platformdata_win.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include "../../../source/posix/platformdata_posix.hpp"
#else
#	error not supported
#endif

namespace tiki
{
	class Event
	{
		TIKI_NONCOPYABLE_CLASS( Event );

	public:

		Event();
		~Event();

		bool	create( bool initialState = false, bool manualReset = false, const char* pName = nullptr );
		void	dispose();

		void	signal();
		void	reset();

		bool	waitForSignal( timems timeOut = TIKI_TIME_OUT_INFINITY );

	private:

		EventPlatformData	m_platformData;

	};
}

#endif // __TIKI_EVENT_HPP_INCLUDED__
