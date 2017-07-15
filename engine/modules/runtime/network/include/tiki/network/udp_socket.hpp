#ifndef KEEN_UDP_SOCKET_HPP_INCLUDED
#define KEEN_UDP_SOCKET_HPP_INCLUDED

namespace tiki
{
	struct IpEndpoint;

	class UdpSocket
	{
		TIKI_NONCOPYABLE_CLASS( UdpSocket );

	public:

		bool		create();
		void		dispose();

		bool		bind( const IpEndpoint& endpoint );

		uint		receiveDataFrom( IpEndpoint& endpoint, void* pBuffer, uint bufferSize );
		bool		sendDataFrom( const IpEndpoint& endpoint, const void* pData, uint dataSize );

	private:
	};
}

#endif
