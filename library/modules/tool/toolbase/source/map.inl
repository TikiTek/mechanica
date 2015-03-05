#pragma once
#ifndef TIKI_MAP_INL_INCLUDED__
#define TIKI_MAP_INL_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE Map<TKey, TValue>::Map()
	{
		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE tiki::Map<TKey, TValue>::Map( const Map< TKey, TValue >& copy )
	{
		m_pData = nullptr;
		m_count = 0;

		*this = copy;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE Map<TKey, TValue>::~Map()
	{
		dispose();
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map<TKey, TValue>::clear()
	{
		m_count = 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map<TKey, TValue>::dispose()
	{
		if ( m_pData != nullptr )
		{
			memory::deleteArray( m_pData, m_capacity );
		}

		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE uint Map<TKey, TValue>::getCount() const
	{
		return m_count;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE bool tiki::Map<TKey, TValue>::isEmpty() const
	{
		return m_count == 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE TValue& Map<TKey, TValue>::getValueAt( uint index )
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE const TValue& Map<TKey, TValue>::getValueAt( uint index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE KeyValuePair<TKey, TValue>& Map<TKey, TValue>::getPairAt( uint index )
	{
		TIKI_ASSERT( index < m_capacity );
		if ( index >= m_count )
		{
			m_count = index + 1u;
		}
		return m_pData[ index ];
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE const KeyValuePair<TKey, TValue>& Map<TKey, TValue>::getPairAt( uint index ) const
	{
		TIKI_ASSERT( index < m_capacity );
		return m_pData[ index ];
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE bool Map<TKey, TValue>::findValue( TValue* pTargetValue, TKey key ) const
	{
		TIKI_ASSERT( pTargetValue != nullptr );

		const uint index = findIndex( key );
		if ( index == TIKI_SIZE_T_MAX )
		{
			return false;
		}

		*pTargetValue = m_pData[ index ].value;
		return true;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map<TKey, TValue>::set( const TKey& key, const TValue& value )
	{
		uint pos = 0u;
		for (; pos < m_count; ++pos)
		{
			Pair& pair = m_pData[ pos ];

			if ( pair.key == key )
			{
				pair.value = value;
				return;
			}
			else if ( pair.key > key )
			{
				break;
			}
		}

		checkArraySize( m_count + 1 );

		for (uint i = m_count; i > pos; --i)
		{
			m_pData[ i ] = m_pData[ i - 1 ];
		}

		m_pData[ pos ].key		= key;
		m_pData[ pos ].value	= value;
		m_count++;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE bool Map<TKey, TValue>::remove( const TKey& key )
	{
		const uint index = findIndex( key );

		if ( index == TIKI_SIZE_T_MAX )
		{
			return false;
		}

		m_count--;
		for (uint i = index; i < m_count; ++i)
		{
			m_pData[ i ] = m_pData[ i + 1u ];
		}

		return true;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map<TKey, TValue>::operator=( const Map<TKey, TValue>& copy )
	{
		memory::deleteArray( m_pData, m_capacity );

		m_capacity		= copy.m_capacity;
		m_count			= copy.m_count;

		if ( m_count > 0u )
		{
			m_pData = memory::newArray< Pair >( m_capacity );
		}
		else
		{
			m_pData		= nullptr;
			m_capacity	= 0u;
		}

		for (uint i = 0u; i < m_count; ++i)
		{
			m_pData[ i ] = copy.m_pData[ i ];
		}
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE uint Map<TKey, TValue>::findIndex( const TKey& key ) const
	{
		if ( m_count == 0u )
		{
			return TIKI_SIZE_T_MAX;
		}

		int imin = 0u;
		int imax = int( m_count );
		// continue searching while [imin,imax] is not empty
		while (imax >= imin)
		{
			// calculate the midpoint for roughly equal partition
			const int imid =  ( imax + imin ) / 2u; //midpoint(imin, imax);
			if(m_pData[ imid ].key == key)
			{
				// key found at index imid
				return imid;
			}
			// determine which sub array to search
			else if (m_pData[ imid ].key < key)
			{
				// change min index to search upper sub array
				imin = imid + 1;
			}
			else
			{
				// change max index to search lower sub array
				imax = imid - 1;
			}
		}
		// key was not found
		return TIKI_SIZE_T_MAX;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE uint tiki::Map<TKey, TValue>::getNextSize( uint targetSize )
	{
		uint capacity = TIKI_MAX( 2u, m_capacity );

		while ( capacity <= targetSize )
		{
			capacity *= 2;
		}

		return capacity;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void tiki::Map<TKey, TValue>::checkArraySize( uint neddedSize )
	{
		if ( m_capacity < neddedSize )
		{
			const uint capacity = getNextSize( neddedSize );
			Pair* pNewData = memory::newArray< Pair >( capacity );

			for (uint i = 0u; i < m_count; ++i)
			{
				pNewData[ i ] = m_pData[ i ];
			}

			if ( m_pData != nullptr )
			{
				memory::deleteArray( m_pData, m_capacity );
			}

			m_pData		= pNewData;
			m_capacity	= capacity;
		}
	}
}

#endif // TIKI_MAP_INL_INCLUDED__