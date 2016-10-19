#ifndef TIKI_MEMORY_INL
#define TIKI_MEMORY_INL

#include <new>

namespace tiki
{
	TIKI_FORCE_INLINE void memory::set8( void* pTargetData, uint count, uint8 value )
	{
		register uint remainingCount = count;
		register uint8* pData = static_cast< uint8* >( pTargetData );
		while ( remainingCount-- > 0u )
		{
			*pData++ = value;
		}
	}

	TIKI_FORCE_INLINE void memory::set16( void* pTargetData, uint count, uint16 value )
	{
		register uint remainingCount = count;
		register uint16* pData = static_cast< uint16* >( pTargetData );
		while ( remainingCount-- > 0u )
		{
			*pData++ = value;
		}
	}

	TIKI_FORCE_INLINE void memory::set32( void* pTargetData, uint count, uint32 value )
	{
		register uint remainingCount = count;
		register uint32* pData = static_cast< uint32* >( pTargetData );
		while ( remainingCount-- > 0u )
		{
			*pData++ = value;
		}
	}

	TIKI_FORCE_INLINE void memory::set64( void* pTargetData, uint count, uint64 value )
	{
		register uint remainingCount = count;
		register uint64* pData = static_cast< uint64* >( pTargetData );
		while ( remainingCount-- > 0u )
		{
			*pData++ = value;
		}
	}

	TIKI_FORCE_INLINE void memory::zero( void* pTargetData, uint sizeInBytes )
	{
		uint8* pData = static_cast< uint8* >( pTargetData );

		const uint uint64Count	= sizeInBytes / 8u;
		const uint uint8Count	= sizeInBytes % 8u;
		const uint offset		= uint64Count * 8u;

		set64( pData, uint64Count, 0ull );
		set8( pData + offset, uint8Count, 0u );
	}

	template<typename T>
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
	TIKI_FORCE_INLINE T* memory::allocateAlignedObject( const char* pFileName, int lineNumber, uint alignment, bool zeroMemory )
#else
	TIKI_FORCE_INLINE T* memory::allocateAlignedObject( uint alignment, bool zeroMemory )
#endif
	{
		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = TIKI_ALIGNOF( T );
		}

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		void* pNew = memory::allocateAlignedMemory( sizeof( T ), pFileName, lineNumber, alignment, zeroMemory );
#else
		void* pNew = memory::allocateAlignedMemory( sizeof( T ), alignment, zeroMemory );
#endif

		return static_cast< T*>( pNew );
	}

	template<typename T>
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
	TIKI_FORCE_INLINE T* memory::newArrayAligned( uint count, const char* pFileName, int lineNumber, uint alignment, bool constructElements )
#else
	TIKI_FORCE_INLINE T* memory::newArrayAligned( uint count, uint alignment, bool constructElements )
#endif
	{
		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = TIKI_ALIGNOF( T );
		}

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		void* pNew = memory::allocateAlignedMemory( sizeof( T ) * count, pFileName, lineNumber, alignment, false );
#else
		void* pNew = memory::allocateAlignedMemory( sizeof( T ) * count, alignment, false );
#endif

		T* pArray = static_cast<T*>(pNew);
		if( constructElements )
		{
			for( uint i = 0u; i < count; ++i )
			{
				T* pItem = new(&pArray[ i ]) T;
				TIKI_ASSERT( pItem == &pArray[ i ] );
			}
		}

		return pArray;
	}

	template<typename T>
	void memory::deleteObjectAligned( T* pPtr )
	{
		TIKI_ASSERT( pPtr != nullptr );

		pPtr->~T();
		memory::freeAligned( pPtr );
	}

	template<typename T>
	void memory::deleteArrayAligned( T* pArray, uint count )
	{
		TIKI_ASSERT( pArray != nullptr );

		for (uint i = 0u; i < count; ++i)
		{
			pArray[ i ].~T();
		}

		memory::freeAligned( pArray );
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::zero( T& p )
	{
		zero( &p, sizeof( p ) );
	}
}

#endif // TIKI_MEMORY_INL