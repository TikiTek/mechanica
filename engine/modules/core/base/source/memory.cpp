
#include "tiki/base/memory.hpp"

#include <malloc.h>

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#	include <memory.h>
#elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )
#	include <string.h>
#endif

#if TIKI_ENABLED( TIKI_BUILD_MSVC ) && TIKI_ENABLED( TIKI_BUILD_DEBUG )
#	include <crtdbg.h>
#endif

namespace tiki
{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
	void* memory::allocateAlignedMemory( uintreg size, const char* pFileName, int lineNumber, uintreg alignment, bool zeroMemory )
#else
	void* memory::allocateAlignedMemory( uintreg size, uintreg alignment, bool zeroMemory )
#endif
	{
		TIKI_ASSERT( alignment != TIKI_DEFAULT_ALIGNMENT );

		void* pMemory = nullptr;

#if TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )
		pMemory = malloc( size );
#elif TIKI_ENABLED( TIKI_BUILD_MSVC )
#	if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		pMemory = _aligned_malloc_dbg( size, alignment, pFileName, lineNumber );
#	else
		pMemory = _aligned_malloc( size, alignment );
#	endif
#endif

		if( zeroMemory )
		{
			memory::zero( pMemory, size );
		}

		TIKI_ASSERT( (uintreg)pMemory % alignment == 0u );
		return pMemory;
	}

	void memory::freeAligned( void* pPtr )
	{
#if TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )
		return free( pPtr );
#elif TIKI_ENABLED( TIKI_BUILD_MSVC )
#	if TIKI_ENABLED( TIKI_BUILD_DEBUG )		
		_aligned_free_dbg( pPtr );
#	else
		_aligned_free( pPtr );
#	endif
#endif
	}

	int	memory::compare( const void* pData1, const void* pData2, uintreg sizeInBytes )
	{
		return memcmp( pData1, pData2, sizeInBytes );
	}

	void memory::copy( void* pTargetData, const void* pSourceData, uintreg sizeInBytes )
	{
		memcpy( pTargetData, pSourceData, sizeInBytes );
	}
}
