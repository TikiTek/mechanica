
#include "tiki/base/endianness.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	Endianness getHostEndianness()
	{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		return Endianness_Little;
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
		return Endianness_Little;
#else
#	error Platform not supported
#endif
	}
	
	Endianness getPlatformEndianness( PlatformType platform )
	{
		switch ( platform )
		{
		case PlatformType_Apple:
		case PlatformType_Linux:
		case PlatformType_Win:
			return Endianness_Little;

		default:
			TIKI_BREAK( "Platform not spported" );
			break;
		}

		return Endianness_Invalid;
	}

	void changeEndianness( void* pData, uint size )
	{
		TIKI_BREAK( "todo" );
	}

	void checkAndChangeEndianness( void* pData, uint size, PlatformType targetPlatform )
	{
		if ( getHostEndianness() != getPlatformEndianness( targetPlatform ) )
		{
			changeEndianness( pData, size );
		}
	}

}