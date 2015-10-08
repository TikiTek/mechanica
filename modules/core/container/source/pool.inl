#pragma once
#ifndef TIKI_POOL_INL_INCLUDED__
#define TIKI_POOL_INL_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE tiki::Pool<T>::Pool()
	{
		m_pData			= nullptr;
		m_pFirstFree	= nullptr;
		m_count			= 0u;
		m_capacity		= 0u;
	}	

	template<typename T>
	TIKI_FORCE_INLINE tiki::Pool<T>::~Pool()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template<typename T>
	TIKI_FORCE_INLINE void tiki::Pool<T>::create( uint capacity )
	{
		TIKI_COMPILETIME_ASSERT( sizeof( T ) >= sizeof( FreeElement ) );

	}

	template<typename T>
	TIKI_FORCE_INLINE void tiki::Pool<T>::dispose()
	{
		TIKI_MEMORY_FREE( m_pData );

		m_pData			= nullptr;
		m_pFirstFree	= nullptr;
		m_count			= 0u;
		m_capacity		= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE void tiki::Pool<T>::clear()
	{

	}

	template<typename T>
	TIKI_FORCE_INLINE uint tiki::Pool<T>::getCount() const
	{

	}

	template<typename T>
	TIKI_FORCE_INLINE bool tiki::Pool<T>::isEmpty() const
	{

	}

	template<typename T>
	TIKI_FORCE_INLINE bool tiki::Pool<T>::contains( const T& item ) const
	{

	}

	template<typename T>
	TIKI_FORCE_INLINE T& tiki::Pool<T>::push()
	{

	}

	template<typename T>
	TIKI_FORCE_INLINE T& tiki::Pool<T>::push( const T& item )
	{

	}

	template<typename T>
	TIKI_FORCE_INLINE bool tiki::Pool<T>::removeUnsortedByValue( const T& item )
	{

	}
}

#endif // TIKI_POOL_INL_INCLUDED__
