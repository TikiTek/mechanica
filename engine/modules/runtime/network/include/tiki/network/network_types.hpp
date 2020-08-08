#pragma once
#ifndef TIKI_NETWORK_TYPES_HPP_INCLUDED
#define TIKI_NETWORK_TYPES_HPP_INCLUDED

#include "tiki/base/types.hpp"

namespace tiki
{
	enum IpVersion : uint8
	{
		IpVersion_4,
		IpVersion_6
	};

	union IpAddress4
	{
		uint8	addressBytes[ 4u ];
		uint32	address;
	};

	struct IpAddress6
	{
		uint8	addressBytes[ 16u ];
		uint16	address[ 8u ];
	};

	struct IpAddress
	{
		IpVersion		version;
		union
		{
			IpAddress4	ipv4;
			IpAddress6	ipv6;
		};
	};

	struct IpEndpoint
	{
		IpAddress	address;
		uint16		port;
	};

	enum SocketFlags : uint8
	{
		SocketFlags_NonBlocking,
		SocketFlags_ReuseAddress
	};

	namespace network
	{
		IpAddress4	getAnyIpAddress4();
		IpAddress4	getBroadcastIpAddress4();
		IpAddress6	getAnyIpAddress6();
		IpAddress6	getBroadcastIpAddress6();

		void		formatIpAddress( char* pBuffer, uint bufferSize, const IpAddress& ipAddress );
		void		formatIpEndpoint( char* pBuffer, uint bufferSize, const IpEndpoint& ipAddress );
	}
}

#endif // TIKI_NETWORK_TYPES_HPP_INCLUDED
