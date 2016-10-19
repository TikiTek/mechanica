
#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/functions.hpp"

namespace tiki
{
	template< typename T >
	TIKI_FORCE_INLINE SizedArray< T >::SizedArray()
		: m_pData( nullptr ), m_count( 0u ), m_capacity( 0u )
	{
	}

	template< typename T >
	TIKI_FORCE_INLINE SizedArray< T >::~SizedArray()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template< typename T >
	TIKI_FORCE_INLINE bool SizedArray< T >::create( uint capacity, size_t aligment /* = TIKI_DEFAULT_ALIGNMENT */, bool constructElements /* = true */ )
	{
		TIKI_ASSERT( capacity > 0u );
		TIKI_ASSERT( m_pData == nullptr );
		TIKI_ASSERT( m_count == 0u );
		TIKI_ASSERT( m_capacity == 0u );

		m_capacity	= capacity;
		m_pData		= TIKI_NEW_ARRAY_ALIGNED( T, capacity, aligment, constructElements );
		if ( m_pData == nullptr )
		{
			dispose();
			return false;				 
		}

		return true;
	}

	template< typename T >
	TIKI_FORCE_INLINE void SizedArray< T >::dispose()
	{
		if ( m_pData != nullptr )
		{
			TIKI_MEMORY_DELETE_ARRAY( m_pData, m_capacity );
		}

		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template< typename T >
	TIKI_FORCE_INLINE void SizedArray< T >::clear()
	{
		TIKI_ASSERT( m_pData != nullptr );

		m_count = 0u;
	}

	template< typename T >
	TIKI_FORCE_INLINE T& SizedArray< T >::push()
	{
		TIKI_ASSERT( m_count < m_capacity );
		return m_pData[ m_count++ ];
	}

	template< typename T >
	TIKI_FORCE_INLINE T& SizedArray< T >::push( const T& value )
	{
		TIKI_ASSERT( m_count < m_capacity );
		return m_pData[ m_count++ ] = value;		
	}

	template< typename T >
	TIKI_FORCE_INLINE T* SizedArray< T >::pushRange( uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( m_count + count <= m_capacity );

		const uint currentCount = m_count;
		m_count += count;

		return &m_pData[ currentCount ];
	}

	template< typename T >
	TIKI_FORCE_INLINE T* SizedArray< T >::pushRange( const T* pData, uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( m_count + count <= m_capacity );

		const uint currentCount = m_count;
		for (uint i = 0u; i < count; ++i)
		{
			m_pData[ m_count++ ] = pData[ i ];
		}

		return &m_pData[ currentCount ];
	}

	template< typename T >
	TIKI_FORCE_INLINE bool SizedArray< T >::pop( T& value )
	{
		if ( isEmpty() )
		{
			return false;
		}

		value = m_pData[ --m_count ];
		return true;
	}

	template< typename T >
	TIKI_FORCE_INLINE void SizedArray< T >::removeUnsortedByIndex( uint index )
	{
		TIKI_ASSERT( index < m_count );
		m_pData[ index ] = m_pData[ --m_count ];
	}

	template< typename T >
	TIKI_FORCE_INLINE bool SizedArray< T >::removeUnsortedByValue( const T& value )
	{
		for (uint i = 0u; i < m_count; ++i)
		{
			if ( m_pData[ i ] == value )
			{
				removeUnsortedByIndex( i );
				return true;
			}
		}

		return false;
	}

	template< typename T >
	TIKI_FORCE_INLINE uint SizedArray< T >::getIndexOf( const T* pValue ) const
	{
		TIKI_ASSERT( pValue >= m_pData );
		TIKI_ASSERT( pValue < getEnd() );

		return pValue - m_pData;
	}

	template< typename T >
	TIKI_FORCE_INLINE uint SizedArray< T >::getIndexOf( const T& value ) const
	{
		for (uint i = 0u; i < m_count; ++i)
		{
			if ( m_pData[ i ] == value )
			{
				return i;
			}
		}

		return TIKI_SIZE_T_MAX;
	}

	template< typename T >
	TIKI_FORCE_INLINE T& SizedArray< T >::operator[]( uint index )
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}

	template< typename T >
	TIKI_FORCE_INLINE const T& SizedArray< T >::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}
}