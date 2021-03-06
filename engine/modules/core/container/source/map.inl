#pragma once

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE Map< TKey, TValue >::Map()
	{
		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE Map< TKey, TValue >::Map( const Map< TKey, TValue >& copy )
	{
		m_pData = nullptr;
		m_count = 0;

		*this = copy;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE Map< TKey, TValue >::~Map()
	{
		dispose();
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map< TKey, TValue >::dispose()
	{
		if ( m_pData != nullptr )
		{
			TIKI_DELETE_ARRAY( m_pData, m_capacity );
		}

		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map< TKey, TValue >::clear()
	{
		m_count = 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE uintreg Map< TKey, TValue >::getCount() const
	{
		return m_count;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE bool Map< TKey, TValue >::isEmpty() const
	{
		return m_count == 0u;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE bool Map< TKey, TValue >::hasKey( const TKey& key ) const
	{
		return findIndex( key ) != TIKI_SIZE_T_MAX;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE TValue& Map< TKey, TValue >::getValueAt( uintreg index )
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE const TValue& Map< TKey, TValue >::getValueAt( uintreg index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE KeyValuePair< TKey, TValue >& Map< TKey, TValue >::getPairAt( uintreg index )
	{
		TIKI_ASSERT( index < m_capacity );
		if ( index >= m_count )
		{
			m_count = index + 1u;
		}
		return m_pData[ index ];
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE const KeyValuePair< TKey, TValue >& Map< TKey, TValue >::getPairAt( uintreg index ) const
	{
		TIKI_ASSERT( index < m_capacity );
		return m_pData[ index ];
	}

	template< typename TKey, typename TValue >
	TIKI_FORCE_INLINE TValue* tiki::Map< TKey, TValue >::find( const TKey& key )
	{
		const uintreg index = findIndex( key );
		if( index == TIKI_SIZE_T_MAX )
		{
			return nullptr;
		}

		return &m_pData[ index ].value;
	}

	template< typename TKey, typename TValue >
	TIKI_FORCE_INLINE const TValue* tiki::Map< TKey, TValue >::find( const TKey& key ) const
	{
		const uintreg index = findIndex( key );
		if( index == TIKI_SIZE_T_MAX )
		{
			return nullptr;
		}

		return &m_pData[ index ].value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE bool Map< TKey, TValue >::findValue( TValue* pTargetValue, const TKey& key ) const
	{
		TIKI_ASSERT( pTargetValue != nullptr );

		const uintreg index = findIndex( key );
		if ( index == TIKI_SIZE_T_MAX )
		{
			return false;
		}

		*pTargetValue = m_pData[ index ].value;
		return true;
	}

	template< typename TKey, typename TValue >
	TIKI_FORCE_INLINE typename Map< TKey, TValue >::InsertResult Map< TKey, TValue >::insertKey( const TKey& key )
	{
		const uintreg pos = findPositionIndex( key );

		Pair& pair1 = m_pData[ pos ];
		if( pos < m_count && pair1.key == key )
		{
			InsertResult result;
			result.isNew	= false;
			result.pValue	= &pair1.value;
			return result;
		}

		checkArraySize( m_count + 1 );

		for( uintreg i = m_count; i > pos; --i )
		{
			m_pData[ i ] = m_pData[ i - 1 ];
		}

		Pair& pair2 = m_pData[ pos ];
		pair2.key	= key;
		pair2.value	= TValue();
		m_count++;

		InsertResult result;
		result.isNew	= true;
		result.pValue	= &pair2.value;
		return result;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE TValue& Map< TKey, TValue >::set( const TKey& key, const TValue& value )
	{
		const uintreg pos = findPositionIndex( key );

		Pair& pair1 = m_pData[ pos ];
		if( pos < m_count && pair1.key == key )
		{
			pair1.value = value;
			return pair1.value;
		}

		checkArraySize( m_count + 1 );

		for( uintreg i = m_count; i > pos; --i )
		{
			m_pData[ i ] = m_pData[ i - 1 ];
		}

		Pair& pair2 = m_pData[ pos ];
		pair2.key	= key;
		pair2.value	= value;
		m_count++;

		return pair2.value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE bool Map< TKey, TValue >::remove( const TKey& key )
	{
		const uintreg index = findIndex( key );
		if ( index == TIKI_SIZE_T_MAX )
		{
			return false;
		}

		m_count--;
		for (uintreg i = index; i < m_count; ++i)
		{
			m_pData[ i ] = m_pData[ i + 1u ];
		}

		return true;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE TValue& Map< TKey, TValue >::operator[]( const TKey& key )
	{
		const uintreg index = findIndex( key );
		if ( index == TIKI_SIZE_T_MAX )
		{
			return set( key, TValue() );
		}

		return m_pData[index].value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE const TValue& Map< TKey, TValue >::operator[]( const TKey& key ) const
	{
		const uintreg index = findIndex( key );
		TIKI_ASSERT( index != TIKI_SIZE_T_MAX );

		return m_pData[index].value;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map< TKey, TValue >::operator=( const Map< TKey, TValue >& copy )
	{
		if( m_pData != nullptr )
		{
			TIKI_DELETE_ARRAY( m_pData, m_capacity );
		}

		m_capacity		= copy.m_capacity;
		m_count			= copy.m_count;

		if( m_count > 0u )
		{
			m_pData = TIKI_NEW_ARRAY( Pair, m_capacity, true );
		}
		else
		{
			m_pData		= nullptr;
			m_capacity	= 0u;
		}

		for( uintreg i = 0u; i < m_count; ++i )
		{
			m_pData[ i ] = copy.m_pData[ i ];
		}
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE KeyValuePair< TKey, TValue >* Map< TKey, TValue >::getBegin()
	{
		return m_pData;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE const KeyValuePair< TKey, TValue >* Map< TKey, TValue >::getBegin() const
	{
		return m_pData;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE KeyValuePair< TKey, TValue >* Map< TKey, TValue >::getEnd()
	{
		return m_pData + m_count;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE const KeyValuePair< TKey, TValue >* Map< TKey, TValue >::getEnd() const
	{
		return m_pData + m_count;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE uintreg Map< TKey, TValue >::findIndex( const TKey& key ) const
	{
		if ( m_count == 0u )
		{
			return TIKI_SIZE_T_MAX;
		}

		int imin = 0u;
		int imax = int( m_count );
		while (imax >= imin)
		{
			const int imid =  ( imax + imin ) / 2u;
			if ( imid < 0 || imid >= (int)m_count )
			{
				return TIKI_SIZE_T_MAX;
			}

			if ( m_pData[ imid ].key == key )
			{
				return imid;
			}
			else if ( m_pData[ imid ].key < key )
			{
				imin = imid + 1;
			}
			else
			{
				imax = imid - 1;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE uintreg Map< TKey, TValue >::findPositionIndex( const TKey& key ) const
	{
		if( m_count == 0u )
		{
			return m_count;
		}

		int rangeStart = 0;
		int rangeEnd = int( m_count );
		while( rangeEnd >= rangeStart && rangeStart < int( m_count ) )
		{
			const int rangeMid =  rangeStart + ((rangeEnd - rangeStart) / 2);
			const Pair& kvp = m_pData[ rangeMid ];

			if( kvp.key == key )
			{
				return rangeMid;
			}
			else if( key < kvp.key )
			{
				rangeEnd = rangeMid - 1;
			}
			else
			{
				rangeStart = rangeMid + 1;
			}
		}

		return uintreg( rangeStart );
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE uintreg Map< TKey, TValue >::getNextSize( uintreg targetSize )
	{
		uintreg capacity = TIKI_MAX( 2u, m_capacity );

		while ( capacity <= targetSize )
		{
			capacity *= 2;
		}

		return capacity;
	}

	template<typename TKey, typename TValue>
	TIKI_FORCE_INLINE void Map< TKey, TValue >::checkArraySize( uintreg neddedSize )
	{
		if ( m_capacity < neddedSize )
		{
			const uintreg capacity = getNextSize( neddedSize );
			Pair* pNewData = TIKI_NEW_ARRAY( Pair, capacity, true );

			for (uintreg i = 0u; i < m_count; ++i)
			{
				pNewData[ i ] = m_pData[ i ];
			}

			if ( m_pData != nullptr )
			{
				TIKI_DELETE_ARRAY( m_pData, m_capacity );
			}

			m_pData		= pNewData;
			m_capacity	= capacity;
		}
	}
}
