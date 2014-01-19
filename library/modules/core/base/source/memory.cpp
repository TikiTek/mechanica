
#include "tiki/base/memory.hpp"

#include <memory>
#include <malloc.h>

#if TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include <string.h>
#endif

namespace tiki
{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
	void* memory::allocAlign( size_t size, size_t alignment )
	{
		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = 4u;
		}

		return _aligned_malloc( size, alignment );
	}

	void memory::freeAlign( void* pPtr )
	{
		_aligned_free( pPtr );
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