#pragma once
#ifndef TIKI_SOCKET_HPP_INCLUDED__
#define TIKI_SOCKET_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#if TIKI_ENABLED(TIKI_PLATFORM_WIN)
#	include "../../../source/win/platformdata_win.hpp"
#elif TIKI_ENABLED(TIKI_PLATFORM_LINUX)
#	include "../../../source/posix/platformdata_posix.hpp"
#endif

namespace tiki
{
	class Socket
	{
		TIKI_NONCOPYABLE_CLASS( Socket );

	public:

							Socket();
							~Socket();

		uint				readData( void* pTarget, uint length );

		uint				writeData( const void* pData, uint length );

	private:

		SocketPlatformData	m_platformData;

	};
}

#endif // TIKI_SOCKET_HPP_INCLUDED__
