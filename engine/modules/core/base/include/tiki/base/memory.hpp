#pragma once
#ifndef TIKI_MEMORY_HPP
#define TIKI_MEMORY_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/assert.hpp"

#define TIKI_MINIMUM_ALIGNMENT TIKI_SIZE_T_BYTES

#if TIKI_ENABLED( TIKI_BUILD_MSVC ) && 0
#	define TIKI_DECLARE_STACKANDZERO( type, name ) type name = { }
#else
#	define TIKI_DECLARE_STACKANDZERO( type, name ) type name; memory::zero( name )
#endif

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )

#	define TIKI_ALLOC( size )											::tiki::memory::allocateAlignedMemory( size, __FILE__, __LINE__, TIKI_MINIMUM_ALIGNMENT, false )
#	define TIKI_ALLOC_ALIGNED( size, alignment )						::tiki::memory::allocateAlignedMemory( size, __FILE__, __LINE__, alignment, false )

#	define TIKI_ALLOC_ZERO( size )										::tiki::memory::allocateAlignedMemory( size, __FILE__, __LINE__, TIKI_DEFAULT_ALIGNMENT, true ) ) type
#	define TIKI_ALLOC_ZERO_ALIGNED( size, alignment )					::tiki::memory::allocateAlignedMemory( size, __FILE__, __LINE__, true ) ) type

#	define TIKI_NEW( type )												::new ( ::tiki::memory::allocateAlignedObject< type >( __FILE__, __LINE__, TIKI_DEFAULT_ALIGNMENT, false ) ) type
#	define TIKI_NEW_ALIGNED( type, alignment )							::new ( ::tiki::memory::allocateAlignedObject< type >( __FILE__, __LINE__, alignment, false ) ) type

#	define TIKI_NEW_ZERO( type )										::new ( ::tiki::memory::allocateAlignedObject< type >( __FILE__, __LINE__, TIKI_DEFAULT_ALIGNMENT, true ) ) type
#	define TIKI_NEW_ZERO_ALIGNED( type, alignment )						::new ( ::tiki::memory::allocateAlignedObject< type >( __FILE__, __LINE__, alignment, true ) ) type

#	define TIKI_NEW_ARRAY( type, count, construct )						::tiki::memory::newArrayAligned< type >( count, __FILE__, __LINE__, TIKI_DEFAULT_ALIGNMENT, construct  )
#	define TIKI_NEW_ARRAY_ALIGNED( type, count, alignment, construct )	::tiki::memory::newArrayAligned< type >( count, __FILE__, __LINE__, alignment, construct )

#else

#	define TIKI_ALLOC( size )											::tiki::memory::allocateAlignedMemory( size, TIKI_MINIMUM_ALIGNMENT, false )
#	define TIKI_ALLOC_ALIGNED( size, alignment )						::tiki::memory::allocateAlignedMemory( size, alignment, true )

#	define TIKI_ALLOC_ZERO( size )										::tiki::memory::allocateAlignedMemory( size, TIKI_DEFAULT_ALIGNMENT, true ) ) type
#	define TIKI_ALLOC_ZERO_ALIGNED( size, alignment )					::tiki::memory::allocateAlignedMemory( size, true ) ) type

#	define TIKI_NEW( type )												::new ( ::tiki::memory::allocateAlignedObject< type >( TIKI_DEFAULT_ALIGNMENT, false ) ) type
#	define TIKI_NEW_ALIGNED( type, alignment )							::new ( ::tiki::memory::allocateAlignedObject< type >( alignment, false ) ) type

#	define TIKI_NEW_ZERO( type )										::new ( ::tiki::memory::allocateAlignedObject< type >( TIKI_DEFAULT_ALIGNMENT, true ) ) type
#	define TIKI_NEW_ZERO_ALIGNED( type, alignment )						::new ( ::tiki::memory::allocateAlignedObject< type >( alignment, true ) ) type

#	define TIKI_NEW_ARRAY( type, count, construct )						::tiki::memory::newArrayAligned< type >( count, TIKI_DEFAULT_ALIGNMENT, construct )
#	define TIKI_NEW_ARRAY_ALIGNED( type, count, alignment, construct )	::tiki::memory::newArrayAligned< type >( count, alignment, construct )

#endif

#define TIKI_FREE( ptr )												::tiki::memory::freeAligned( ptr )
#define TIKI_DELETE( ptr )												::tiki::memory::deleteObjectAligned( ptr )
#define TIKI_DELETE_ARRAY( ptr, count )									::tiki::memory::deleteArrayAligned( ptr, count )

namespace tiki
{
	namespace memory
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		void*					allocateAlignedMemory( uint size, const char* pFileName, int lineNumber, uint alignment, bool zeroMemory );

		template<typename T>
		TIKI_FORCE_INLINE T*	allocateAlignedObject( const char* pFileName, int lineNumber, uint alignment, bool zeroMemory );

		template<typename T>
		TIKI_FORCE_INLINE T*	newArrayAligned( uint count, const char* pFileName, int lineNumber, uint alignment, bool constructElements);
#else
		void*					allocateAlignedMemory( uint size, uint alignment, bool zeroMemory );

		template<typename T>
		TIKI_FORCE_INLINE T*	allocateAlignedObject( uint alignment, bool zeroMemory );

		template<typename T>
		TIKI_FORCE_INLINE T*	newArrayAligned( uint count, uint alignment, bool constructElements );
#endif

		void					freeAligned( void* pPtr );

		template<typename T>
		TIKI_FORCE_INLINE void	deleteObjectAligned( T* pPtr );

		template<typename T>
		TIKI_FORCE_INLINE void	deleteArrayAligned( T* pArray, uint count );

		template<typename T>
		TIKI_FORCE_INLINE void	callDefaultConstructor( T* pObject );

		template<typename T>
		void					callDestructor( T* pObject );

		int						compare( const void* pData1, const void* pData2, uint sizeInBytes );
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
