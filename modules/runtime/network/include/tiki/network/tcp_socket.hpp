#pragma once
#ifndef TIKI_TCP_SOCKET_HPP_INCLUDED
#define TIKI_TCP_SOCKET_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/base/flagmask.hpp"

namespace tiki
{
	enum IpVersion : uint8;
	enum SocketFlags : uint8;
	struct IpEndpoint;

	enum TcpFlags : uint8
	{
		TcpSocketFlags_NoDelay
	};

	class TcpSocket
	{
		TIKI_NONCOPYABLE_CLASS( TcpSocket );

	public:

		bool		create( IpVersion ipVersion, FlagMask8< SocketFlags > socketFlags, FlagMask8< TcpFlags > tcpFlags );
		void		dispose();

		bool		listen( const IpEndpoint& endpoint );
		bool		connect( const IpEndpoint& endpoint );

		bool		accept( TcpSocket& targetSocket );

		uint		receiveData( void* pBuffer, uint bufferSize );
		bool		sendData( const void* pData, uint dataSize );

		bool		getEndpoint( IpEndpoint& endpoint ) const;

	private:

		int			m_socket;
	};
}

#endif // TIKI_TCP_SOCKET_HPP_INCLUDED
