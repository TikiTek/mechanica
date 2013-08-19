
#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/functions.hpp"

namespace tiki
{
	template<typename T>
	SizedArray<T>::SizedArray()
		: m_pData( nullptr ), m_count( 0u ), m_capacity( 0u )
	{
	}

	template<typename T>
	SizedArray<T>::~SizedArray()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}

	template<typename T>
	TIKI_FORCE_INLINE void SizedArray<T>::create( size_t capacity )
	{
		TIKI_ASSERT( capacity );
		TIKI_ASSERT( m_pData == 0u );
		TIKI_ASSERT( m_count == 0u && m_capacity == 0u );

		m_capacity	= capacity;
		m_pData		= memory::newArray< T >( capacity );
	}

	template<typename T>
	TIKI_FORCE_INLINE void SizedArray<T>::dispose()
	{
		TIKI_ASSERT( m_pData );

		memory::deleteArray( m_pData, m_capacity );

		m_pData		= nullptr;
		m_count		= 0u;
		m_capacity	= 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE void SizedArray<T>::clear()
	{
		TIKI_ASSERT( m_pData );

		m_count = 0u;
	}

	template<typename T>
	TIKI_FORCE_INLINE size_t SizedArray<T>::getCount() const
	{
		return m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE size_t SizedArray<T>::getCapacity() const
	{
		return m_capacity;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* SizedArray<T>::getData()
	{
		return m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* SizedArray<T>::getData() const
	{
		return m_pData;
	}

	template<typename T>
	TIKI_FORCE_INLINE T* SizedArray<T>::getEnd()
	{
		return m_pData + m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE const T* SizedArray<T>::getEnd() const
	{
		return m_pData + m_count;
	}

	template<typename T>
	TIKI_FORCE_INLINE T& SizedArray<T>::getTop()
	{
		TIKI_ASSERT( m_count > 0u );
		return m_pData[ m_count - 1u ];
	}

	template<typename T>
	TIKI_FORCE_INLINE const T& SizedArray<T>::getTop() const
	{
		TIKI_ASSERT( m_count > 0u );
		return m_pData[ m_count - 1u ];
	}

	template<typename T>
	TIKI_FORCE_INLINE T& SizedArray<T>::pop()
	{
		TIKI_ASSERT( m_count > 0u );
		return m_pData[ --m_count ];
	}

	template<typename T>
	TIKI_FORCE_INLINE T& SizedArray<T>::push()
	{
		TIKI_ASSERT( m_count < m_capacity );
		return m_pData[ m_count++ ];
	}

	template<typename T>
	TIKI_FORCE_INLINE T& SizedArray<T>::push( const T& value )
	{
		TIKI_ASSERT( m_count < m_capacity );
		return m_pData[ m_count++ ] = value;		
	}

	template<typename T>
	TIKI_FORCE_INLINE T* tiki::SizedArray<T>::pushRange( const uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( m_count + count <= m_capacity );
		
		const size_t currentCount = m_count;
		m_count += count;

		return &m_pData[ currentCount ];
	}
	
	template<typename T>
	TIKI_FORCE_INLINE void SizedArray<T>::pushRange( const T* pData, const uint count )
	{
		TIKI_ASSERT( count >= 1u );
		TIKI_ASSERT( m_count + count <= m_capacity );

		for (size_t i = 0u; i < count; ++i)
		{
			m_pData[ m_count++ ] = pData[ i ];
		}
	}

	template<typename T>
	TIKI_FORCE_INLINE T& SizedArray<T>::operator[]( size_t index )
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}

	template<typename T>
	TIKI_FORCE_INLINE const T& SizedArray<T>::operator[]( size_t index ) const
	{
		TIKI_ASSERT( index < m_count );
		return m_pData[ index ];
	}
}