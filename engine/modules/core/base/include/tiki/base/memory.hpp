#pragma once
#ifndef TIKI_MEMORY_HPP
#define TIKI_MEMORY_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/assert.hpp"

#define TIKI_MINIMUM_ALIGNMENT TIKI_SIZE_T_BYTES

#if TIKI_ENABLED( TIKI_BUILD_MSVC ) && 0
#	define TIKI_DECLARE_STACKANDZERO( type, name ) type name = { }
#else
#	define TIKI_DECLARE_STACKANDZERO( type, name ) type name; memory::zero( &name, sizeof( type ) )
#endif

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )

#	define TIKI_MEMORY_ALLOC( size )											::tiki::memory::allocAligned( size, __FILE__, __LINE__, TIKI_MINIMUM_ALIGNMENT )
#	define TIKI_MEMORY_ALLOC_ALIGNED( size, alignment )							::tiki::memory::allocAligned( size, __FILE__, __LINE__, alignment )

#	define TIKI_MEMORY_NEW_OBJECT( type )										::new ( ::tiki::memory::newObjectAligned< type >( __FILE__, __LINE__, TIKI_DEFAULT_ALIGNMENT ) ) type
#	define TIKI_MEMORY_NEW_OBJECT_ALIGNED( type, alignment )					::new ( ::tiki::memory::newObjectAligned< type >( __FILE__, __LINE__, alignment ) ) type

#	define TIKI_MEMORY_NEW_ARRAY( type, count, construct )						::tiki::memory::newArrayAligned< type >( count, __FILE__, __LINE__, TIKI_DEFAULT_ALIGNMENT, construct  )
#	define TIKI_MEMORY_NEW_ARRAY_ALIGNED( type, count, alignment, construct )	::tiki::memory::newArrayAligned< type >( count, __FILE__, __LINE__, alignment, construct )

#else

#	define TIKI_MEMORY_ALLOC( size )											::tiki::memory::allocAligned( size, TIKI_MINIMUM_ALIGNMENT )
#	define TIKI_MEMORY_ALLOC_ALIGNED( size, alignment )							::tiki::memory::allocAligned( size, alignment )

#	define TIKI_MEMORY_NEW_OBJECT( type )										::new ( ::tiki::memory::newObjectAligned< type >( TIKI_DEFAULT_ALIGNMENT ) ) type
#	define TIKI_MEMORY_NEW_OBJECT_ALIGNED( type, alignment )					::new ( ::tiki::memory::newObjectAligned< type >( alignment ) ) type

#	define TIKI_MEMORY_NEW_ARRAY( type, count, construct )						::tiki::memory::newArrayAligned< type >( count, TIKI_DEFAULT_ALIGNMENT, construct )
#	define TIKI_MEMORY_NEW_ARRAY_ALIGNED( type, count, alignment, construct )	::tiki::memory::newArrayAligned< type >( count, alignment, construct )

#endif

#define TIKI_MEMORY_FREE( pData )				::tiki::memory::freeAligned( pData )
#define TIKI_MEMORY_DELETE_OBJECT( ptr )		::tiki::memory::deleteObjectAligned( ptr )
#define TIKI_MEMORY_DELETE_ARRAY( ptr, count )	::tiki::memory::deleteArrayAligned( ptr, count )

namespace tiki
{
	namespace memory
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		void*					allocAligned( uint size, const char* pFileName, int lineNumber, uint alignment );

		template<typename T>
		TIKI_FORCE_INLINE T*	newObjectAligned( const char* pFileName, int lineNumber, uint alignment );

		template<typename T>
		TIKI_FORCE_INLINE T*	newArrayAligned( uint count, const char* pFileName, int lineNumber, uint alignment, bool constructElements);

#else
		void*					allocAligned( uint size, uint alignment );

		template<typename T>
		TIKI_FORCE_INLINE T*	newObjectAligned( uint alignment );

		template<typename T>
		TIKI_FORCE_INLINE T*	newArrayAligned( uint count, uint alignment, bool constructElements );
#endif

		void					freeAligned( void* pPtr );

		template<typename T>
		void					deleteObjectAligned( T* pPtr );

		template<typename T>
		void					deleteArrayAligned( T* pArray, uint count );

		int						compare ( const void* pData1, const void* pData2, uint sizeInBytes );
		void					copy( void* pTargetData, const void* pSourceData, uint sizeInBytes );
		
		TIKI_FORCE_INLINE void	set8( void* pTargetData, uint count, uint8 value );
		TIKI_FORCE_INLINE void	set16( void* pTargetData, uint count, uint16 value );
		TIKI_FORCE_INLINE void	set32( void* pTargetData, uint count, uint32 value );
		TIKI_FORCE_INLINE void	set64( void* pTargetData, uint count, uint64 value );
		TIKI_FORCE_INLINE void	zero( void* pTargetData, uint sizeInBytes );
		
		template<typename T>
		TIKI_FORCE_INLINE void	zero( T& pTargetData );
	}
}

#include "../../../source/memory.inl"

#endif // TIKI_MEMORY_HPP
