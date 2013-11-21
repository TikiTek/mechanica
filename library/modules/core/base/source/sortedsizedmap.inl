#pragma once
#ifndef __TIKI_SORTEDSIZEDMAP_INL_INCLUDED__
#define __TIKI_SORTEDSIZEDMAP_INL_INCLUDED__

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

		m_pData		= static_cast< Pair* >( memory::allocAlign( sizeof( Pair ) * size ) );
		m_count		= 0u;
		m_capacity	= size;
	}

	template<typename TKey, typename TValue>
	void SortedSizedMap<TKey, TValue>::dispose()
	{
		if ( m_pData != nullptr )
		{
			memory::freeAlign( m_pData );
		}

		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename TKey, typename TValue>
	uint SortedSizedMap<TKey, TValue>::getCount()
	{
		return m_count;
	}

	template<typename TKey, typename TValue>
	uint SortedSizedMap<TKey, TValue>::getCapacity()
	{
		return m_capacity;
	}

	template<typename TKey, typename TValue>
	void SortedSizedMap<TKey, TValue>::getValueAt( TValue* pTargetValue, uint index )
	{
		TIKI_ASSERT( index < m_count );
		*pTargetValue = m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	bool SortedSizedMap<TKey, TValue>::findValue( TValue* pTargetValue, TKey key )
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

		uint min = 0u;
		uint max = m_count;
		uint pos = TIKI_SIZE_T_MAX;
		while ( max > min && pos == TIKI_SIZE_T_MAX )
		{
			const uint mid = ( max + min ) / 2u;

			if ( m_pData[ mid ].key == key )
			{
				pos = mid;
			}
			else if( m_pData[ mid ].key < key )
			{
				min = mid;
			}
			else if( m_pData[ mid ].key > key )
			{
				max = mid;
			}
		}
		
		if ( pos == TIKI_SIZE_T_MAX )
		{
			pos = max;
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
		uint min = 0u;
		uint max = m_count;
		uint pos = TIKI_SIZE_T_MAX;
		while ( max > min && pos == TIKI_SIZE_T_MAX )
		{
			const uint mid = ( max + min ) / 2u;

			if ( m_pData[ mid ].key == key )
			{
				pos = mid;
			}
			else if( m_pData[ mid ].key < key )
			{
				min = mid;
			}
			else if( m_pData[ mid ].key > key )
			{
				max = mid;
			}
		}

		return pos;
	}
}

#endif // __TIKI_SORTEDSIZEDMAP_INL_INCLUDED__
