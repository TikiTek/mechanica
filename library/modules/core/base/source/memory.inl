#ifndef TIKI_MEMORY_INL
#define TIKI_MEMORY_INL

#include <new>

namespace tiki
{
#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
	template<typename T>
	TIKI_FORCE_INLINE T* memory::newAlign( size_t alignment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		return TIKI_NEW T();
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::deleteAlign( T* p )
	{
		TIKI_ASSERT( p );
		TIKI_DEL p;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* memory::newArray( size_t count, size_t alignment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		return TIKI_NEW T[ count ];
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::deleteArray( T* pArray, size_t arraySize )
	{
		TIKI_DELARR pArray;
	}
#else
	template<typename T>
	TIKI_FORCE_INLINE T* memory::newAlign( size_t alignment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		void* pNew = allocAlign( sizeof( T ), alignment );
		return ::new( pNew ) T();
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::deleteAlign( T* p )
	{
		TIKI_ASSERT( p );
		p->~T();
		freeAlign( p );
	}

	template<typename T>
	TIKI_FORCE_INLINE T* memory::newArray( size_t count, size_t alignment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		if ( alignment == TIKI_DEFAULT_ALIGNMENT )
		{
			alignment = TIKI_ALIGNOF( T );
		}

		void* pNew = allocAlign( sizeof( T ) * count, alignment );
		TIKI_ASSERT( (size_t)pNew % alignment == 0u );

		T* pArray = (T*)pNew;

		for (size_t i = 0u; i < count; ++i)
		{
			T* pItem = new( &pArray[ i ] ) T;
			TIKI_ASSERT( (size_t)pItem == (size_t)&pArray[ i ] );
		} 

		return pArray;
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::deleteArray( T* pArray, size_t arraySize )
	{
		for (size_t i = 0u; i < arraySize; ++i)
		{
			pArray[ i ].~T();
		}
		freeAlign( pArray );
	}
#endif

	template<typename T>
	TIKI_FORCE_INLINE void memory::set( T* p, uint8 value )
	{
		TIKI_ASSERT( p );
		set( p, sizeof( T ), value );
	}

	template<typename T>
	TIKI_FORCE_INLINE void memory::zero( T& p )
	{
		zero( &p, sizeof( p ) );
	}

	TIKI_FORCE_INLINE void memory::zero( void* p, size_t size )
	{
		set( p, size, 0u );
	}
}

#endif // TIKI_MEMORY_INL