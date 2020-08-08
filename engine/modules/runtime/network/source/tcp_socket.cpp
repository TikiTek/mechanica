#include "tiki/network/tcp_socket.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/network/network_types.hpp"

#include <WinSock2.h>
#include <ws2ipdef.h>

namespace tiki
{
	namespace network
	{
		void	fillSockaddrFromEndpoint( sockaddr_storage& target, const IpEndpoint& ipEndpoint );
	}

	bool TcpSocket::create( IpVersion ipVersion, FlagMask8< SocketFlags > socketFlags, FlagMask8< TcpFlags > tcpFlags )
	{
		const int pfInet = ipVersion == IpVersion_4 ? PF_INET : PF_INET6;
		m_socket = socket( pfInet, SOCK_STREAM, IPPROTO_TCP );
		if( m_socket < 0 )
		{
			return false;
		}

		if( socketFlags.isFlagSet( SocketFlags_NonBlocking ) )
		{
			unsigned long mode = 0;
			if( ioctlsocket( m_socket, FIONBIO, &mode ) != 0 )
			{
				closesocket( m_socket );
				return false;
			}
			//fcntl()
		}

		if( socketFlags.isFlagSet( SocketFlags_ReuseAddress ) )
		{
			int mode = 1;
			if( setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&mode, sizeof( mode ) ) != 0 )
			{
				closesocket( m_socket );
				return false;
			}
		}

		if( tcpFlags.isFlagSet( TcpSocketFlags_NoDelay ) )
		{
			//SOL_TCP
			int mode = 1;
			if( setsockopt( m_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&mode, sizeof( mode ) ) != 0 )
			{
				closesocket( m_socket );
				return false;
			}
		}

		return true;
	}

	void TcpSocket::dispose()
	{
		closesocket( m_socket );
	}

	bool TcpSocket::listen( const IpEndpoint& endpoint )
	{
		sockaddr_storage bindAddress;
		network::fillSockaddrFromEndpoint( bindAddress, endpoint );

		if( ::bind( m_socket, (const sockaddr*)&bindAddress, sizeof( bindAddress ) ) != 0 )
		{
			return false;
		}

		return ::listen( m_socket, SOMAXCONN ) == 0;
	}

	bool TcpSocket::connect( const IpEndpoint& endpoint )
	{
		sockaddr_storage connectAddress;
		network::fillSockaddrFromEndpoint( connectAddress, endpoint );

		return ::connect( m_socket, (const sockaddr*)&connectAddress, sizeof( connectAddress ) ) == 0;
	}

	bool TcpSocket::accept( TcpSocket& targetSocket )
	{
		sockaddr_storage acceptAddress;
		int acceptAddressLength = sizeof( acceptAddress );
		int socket = ::accept( m_socket, (sockaddr*)&acceptAddress, &acceptAddressLength );
		if( socket < 0 )
		{
			return false;
		}

		targetSocket.m_socket = socket;
		return true;
	}

	uint TcpSocket::receiveData( void* pBuffer, uint bufferSize )
	{
		const int result = ::recv( m_socket, (char*)pBuffer, (int)bufferSize, 0 );
		if( result > 0 )
		{
			return (uint)result;
		}

		return 0u;
	}

	bool TcpSocket::sendData( const void* pData, uint dataSize )
	{
		int result = ::send( m_socket, (const char*)pData, (int)dataSize, 0 );
		if( result > 0 )
		{
			return true;
		}

		return false;
	}

	bool TcpSocket::getEndpoint( IpEndpoint& endpoint ) const
	{
		return false;
	}

	void network::fillSockaddrFromEndpoint( sockaddr_storage& target, const IpEndpoint& ipEndpoint )
	{
		if( ipEndpoint.address.version == IpVersion_4 )
		{
			sockaddr_in& sockaddr = *(sockaddr_in*)&target;
			sockaddr.sin_family				= AF_INET;
			sockaddr.sin_port				= htons( ipEndpoint.port );
			memory::copy( &sockaddr.sin_addr, ipEndpoint.address.ipv4.addressBytes, sizeof( sockaddr.sin_addr ) );
		}
		else
		{
			sockaddr_in6& sockaddr = *(sockaddr_in6*)&target;
			sockaddr.sin6_family			= AF_INET6;
			sockaddr.sin6_port				= htons( ipEndpoint.port );
			memory::copy( &sockaddr.sin6_addr, ipEndpoint.address.ipv6.addressBytes, sizeof( sockaddr.sin6_addr ) );
		}
	}
}
