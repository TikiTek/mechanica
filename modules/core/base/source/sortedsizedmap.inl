#pragma once
#ifndef __TIKI_SORTEDSIZEDMAP_INL_INCLUDED__
#define __TIKI_SORTEDSIZEDMAP_INL_INCLUDED__

#include "tiki/base/assert.hpp"

namespace tiki
{
	template<typename TKey, typename TValue>
	SortedSizedMap<TKey, TValue>::SortedSizedMap()
	{
		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename TKey, typename TValue>
	SortedSizedMap<TKey, TValue>::~SortedSizedMap()
	{
		TIKI_ASSERT( m_pData	== nullptr );
		TIKI_ASSERT( m_count	== 0u );
		TIKI_ASSERT( m_capacity	== 0u );
	}

	template<typename TKey, typename TValue>
	void SortedSizedMap<TKey, TValue>::create( uint size, uint alignment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_pData		= static_cast< Pair* >( memory::newArray< Pair >( size ) );
		m_count		= 0u;
		m_capacity	= size;
	}

	template<typename TKey, typename TValue>
	void SortedSizedMap<TKey, TValue>::dispose()
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
	uint SortedSizedMap<TKey, TValue>::getCount() const
	{
		return m_count;
	}

	template<typename TKey, typename TValue>
	uint SortedSizedMap<TKey, TValue>::getCapacity() const
	{
		return m_capacity;
	}

	template<typename TKey, typename TValue>
	TValue& SortedSizedMap<TKey, TValue>::getValueAt( uint index )
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	const TValue& SortedSizedMap<TKey, TValue>::getValueAt( uint index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	KeyValuePair<TKey, TValue>& SortedSizedMap<TKey, TValue>::getPairAt( uint index )
	{
		TIKI_ASSERT( index < m_capacity );
		if ( index >= m_count )
		{
			m_count = index + 1u;
		}
		return m_pData[ index ];
	}

	template<typename TKey, typename TValue>
	const KeyValuePair<TKey, TValue>& SortedSizedMap<TKey, TValue>::getPairAt( uint index ) const
	{
		TIKI_ASSERT( index < m_capacity );
		return m_pData[ index ];
	}

	template<typename TKey, typename TValue>
	bool SortedSizedMap<TKey, TValue>::findValue( TValue* pTargetValue, TKey key ) const
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
	void SortedSizedMap<TKey, TValue>::set( const TKey& key, const TValue& value )
	{
		TIKI_ASSERT( m_count < m_capacity );

		uint pos = 0u;
		for (; pos < m_count; ++pos)
		{
			if ( m_pData[ pos ].key > key )
			{
				break;
			}
		} 

		for (uint i = m_count; i > pos; --i)
		{
			m_pData[ i ] = m_pData[ i - 1 ];
		}

		m_pData[ pos ].key		= key;
		m_pData[ pos ].value	= value;
		m_count++;
	}
	
	template<typename TKey, typename TValue>
	bool SortedSizedMap<TKey, TValue>::remove( const TKey& key )
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
	uint SortedSizedMap<TKey, TValue>::findIndex( const TKey& key ) const
	{
		if ( m_count == 0u )
		{
			return TIKI_SIZE_T_MAX;
		}

		int imin = 0u;
		int imax = m_count;
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
			// determine which subarray to search
			else if (m_pData[ imid ].key < key)
			{
				// change min index to search upper subarray
				imin = imid + 1;
			}
			else
			{
				// change max index to search lower subarray
				imax = imid - 1;
			}
		}
		// key was not found
		return TIKI_SIZE_T_MAX;

		//while ( max > min && pos == TIKI_SIZE_T_MAX )
		//{
		//	const uint mid = ( max + min ) / 2u;

		//	if ( m_pData[ mid ].key == key )
		//	{
		//		pos = mid;
		//	}
		//	else if( m_pData[ mid ].key < key )
		//	{
		//		min = mid;
		//	}
		//	else if( m_pData[ mid ].key > key )
		//	{
		//		max = mid;
		//	}
		//}

		//return pos;
	}
}

#endif // __TIKI_SORTEDSIZEDMAP_INL_INCLUDED__
