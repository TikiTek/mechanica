
#include "tiki/base/memory.hpp"

#include <memory>
#include <malloc.h>

#if TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include <string.h>
#endif

namespace tiki
{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
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
#else
#	warning "TODO: findout how align malloc works on linux"

	void* memory::allocAlign( size_t size, size_t alignment )
	{
		return malloc( size );
	}
	
	void memory::freeAlign( void* pPtr )
	{
		free( pPtr );
	}
#endif

	int	memory::cmp( void* ptr1, void* ptr2, size_t bytes )
	{
		return memcmp( ptr1, ptr2, bytes );
	}

	void memory::set( void* p, size_t size, uint8 value )
	{
		memset( p, value, size );
	}

	void memory::copy( void* pTarget, const void* pSource, size_t length )
	{
		memcpy( pTarget, pSource, length );
	}
}