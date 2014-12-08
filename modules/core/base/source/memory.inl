#ifndef TIKI_MEMORY_INL
#define TIKI_MEMORY_INL

#include <new>

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE T* memory::newAlign( uint alignment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = TIKI_ALIGNOF( T );
		}

		void* pNew = TIKI_MEMORY_ALLOCALIGN( sizeof( T ), alignment );
		return ::new( pNew ) T();
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::deleteAlign( T* pPtr )
	{
		TIKI_ASSERT( pPtr != nullptr );

		pPtr->~T();
		freeAlign( pPtr );
	}

	template<typename T>
	TIKI_FORCE_INLINE T* memory::newArray( uint count, uint alignment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = TIKI_ALIGNOF( T );
		}

		void* pNew = TIKI_MEMORY_ALLOCALIGN( sizeof( T ) * count, alignment );
		TIKI_ASSERT( (uint)pNew % alignment == 0u );

		T* pArray = (T*)pNew;

		for (uint i = 0u; i < count; ++i)
		{
			T* pItem = new( &pArray[ i ] ) T;
			TIKI_ASSERT( (uint)pItem == (uint)&pArray[ i ] );
		} 

		return pArray;
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::deleteArray( T* pArray, uint count )
	{
		for (uint i = 0u; i < count; ++i)
		{
			pArray[ i ].~T();
		}
		freeAlign( pArray );
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::zero( T& p )
	{
		zero( &p, sizeof( p ) );
	}

	TIKI_FORCE_INLINE void memory::zero( void* pTargetData, uint sizeInBytes )
	{
		uint8* pData = static_cast< uint8* >( pTargetData );

		const uint uint64Count = sizeInBytes / 8u;
		set64( pData, uint64Count, 0ull );

		const uint uint8Count = sizeInBytes % 8u;
		const uint offset		= uint64Count * 8u;
		set8( pData + offset, uint8Count, 0u );
	}
}

#endif // TIKI_MEMORY_INL