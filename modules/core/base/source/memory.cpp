
#include "tiki/base/memory.hpp"

#include <malloc.h>

#if TIKI_ENABLED( TIKI_BUILD_MINGW )
#	include <string.h>
#endif

namespace tiki
{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
	void* memory::allocAlign( size_t size, const char* pFileName, int lineNumber, size_t alignment )
#else
	void* memory::allocAlign( size_t size, size_t alignment )
#endif
	{
		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = 4u;
		}

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		return _aligned_malloc_dbg( size, alignment, pFileName, lineNumber );
#else
		return _aligned_malloc( size, alignment );
#endif
	}

	void memory::freeAlign( void* pPtr )
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		_aligned_free( pPtr );
#else
		_aligned_free_dbg( pPtr );
#endif
	}

#elif TIKI_ENABLED( TIKI_BUILD_MINGW )

#	warning "TODO: findout how align malloc works on linux"

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
	void* memory::allocAlign( size_t size, const char* pFileName, int lineNumber, size_t alignment )
#else
	void* memory::allocAlign( size_t size, size_t alignment )
#endif
	{
		return malloc( size );
	}

	void memory::freeAlign( void* pPtr )
	{
		free( pPtr );
	}

#else

#   error Platform not supported

#endif

	int	memory::compare( const void* pData1, const void* pData2, uint sizeInBytes )
	{
		return memcmp( pData1, pData2, sizeInBytes );
	}

	//void memory::set( void* p, size_t size, uint8 value )
	//{
	//	memset( p, value, size );
	//}

	void memory::copy( void* pTargetData, const void* pSourceData, uint sizeInBytes )
	{
		memcpy( pTargetData, pSourceData, sizeInBytes );
	}

	void memory::set8( void* pTargetData, uint size, uint8 value )
	{
		register uint count = size;
		register uint8* pData = static_cast< uint8* >( pTargetData );
		while ( count-- > 0u )
		{
			*pData++ = value;
		}
	}

	void memory::set16( void* pTargetData, uint size, uint16 value )
	{
		register uint count = size;
		register uint16* pData = static_cast< uint16* >( pTargetData );
		while ( count-- > 0u )
		{
			*pData++ = value;
		}
	}

	void memory::set32( void* pTargetData, uint size, uint32 value )
	{
		register uint count = size;
		register uint32* pData = static_cast< uint32* >( pTargetData );
		while ( count-- > 0u )
		{
			*pData++ = value;
		}
	}

	void memory::set64( void* pTargetData, uint size, uint64 value )
	{
		register uint count = size;
		register uint64* pData = static_cast< uint64* >( pTargetData );
		while ( count-- > 0u )
		{
			*pData++ = value;
		}
	}
}
