#pragma once
#ifndef TIKI_CLIENT_HPP_INCLUDED__
#define TIKI_CLIENT_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/net/socket.hpp"

namespace tiki
{
	class Client
	{
		TIKI_NONCOPYABLE_CLASS( Client );

	public:

		bool				create( const char* pHostName, uint16 port );
		void				dispose();

		void				waitForConnection();

		Socket&				getSocket();

	private:

		ClientPlatformData	m_platformData;

		Socket&				m_socket;

	};
}

#endif // TIKI_CLIENT_HPP_INCLUDED__
