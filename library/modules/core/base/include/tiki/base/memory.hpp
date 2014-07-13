#pragma once
#ifndef TIKI_MEMORY_HPP
#define TIKI_MEMORY_HPP

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"
#include "tiki/base/assert.hpp"

#define TIKI_DECLARE_STACKANDZERO( type, name ) type name; \
	memory::zero( &name, sizeof( type ) )

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
#	define TIKI_MEMORY_ALLOC( size ) ::tiki::memory::allocAlign( size, __FILE__, __LINE__, TIKI_DEFAULT_ALIGNMENT )
#	define TIKI_MEMORY_ALLOCALIGN( size, alignment ) ::tiki::memory::allocAlign( size, __FILE__, __LINE__, alignment )
#else
#	define TIKI_MEMORY_ALLOC( size ) ::tiki::memory::allocAlign( size, TIKI_DEFAULT_ALIGNMENT )
#	define TIKI_MEMORY_ALLOCALIGN( size, alignment ) ::tiki::memory::allocAlign( size, alignment )
#endif

#define TIKI_MEMORY_FREE( pData ) ::tiki::memory::freeAlign( pData )

namespace tiki
{
	namespace memory
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		void*					allocAlign( uint size, const char* pFileName, int lineNumber, uint alignment = TIKI_DEFAULT_ALIGNMENT );
#else
		void*					allocAlign( uint size, uint alignment = TIKI_DEFAULT_ALIGNMENT );
#endif

		void					freeAlign( void* pPtr );

		int						compare ( const void* pData1, const void* pData2, uint sizeInBytes );
		void					copy( void* pTargetData, const void* pSourceData, uint sizeInBytes );
		void					set8( void* pTargetData, uint size, uint8 value );
		void					set16( void* pTargetData, uint size, uint16 value );
		void					set32( void* pTargetData, uint size, uint32 value );
		void					set64( void* pTargetData, uint size, uint64 value );
		
		template<typename T>
		TIKI_FORCE_INLINE T*	newAlign( uint alignment = TIKI_DEFAULT_ALIGNMENT );

		template<typename T>
		TIKI_FORCE_INLINE void	deleteAlign( T* pPtr );

		template<typename T>
		TIKI_FORCE_INLINE T*	newArray( uint count, uint alignment = TIKI_DEFAULT_ALIGNMENT );

		template<typename T>
		TIKI_FORCE_INLINE void	deleteArray( T* pArray, uint count );

		template<typename T>
		TIKI_FORCE_INLINE void	zero( T& pTargetData );

		TIKI_FORCE_INLINE void	zero( void* pTargetData, uint sizeInBytes );
	}
}

#include "../../../source/memory.inl"

#endif // TIKI_MEMORY_HPP
