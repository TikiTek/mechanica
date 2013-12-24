#ifndef TIKI_GRAPHICSSTATEOBJECT_HPP
#define TIKI_GRAPHICSSTATEOBJECT_HPP

#include "tiki/base/assert.hpp"
#include "tiki/base/sizedarray.hpp"

namespace tiki
{
	class GraphicsStateObject
	{
		TIKI_NONCOPYABLE_CLASS( GraphicsStateObject );
		template<typename T>
		friend class GraphicsStateObjectCollection;

	public:

		TIKI_FORCE_INLINE bool	icCreated() const
		{
			return m_refCount != 0u;
		}

		TIKI_FORCE_INLINE bool	isCompatible( crc32 hashValue ) const
		{
			TIKI_ASSERT( m_refCount != 0u );
			return m_hashValue == hashValue;
		}

		TIKI_FORCE_INLINE crc32	getHashValue() const
		{
			TIKI_ASSERT( m_refCount != 0u );
			return m_hashValue;
		}

		TIKI_FORCE_INLINE void	addRef()
		{
			TIKI_ASSERT( m_refCount != 0u );
			m_refCount++;
		}

		TIKI_FORCE_INLINE bool	releaseRef()
		{
			TIKI_ASSERT( m_refCount != 0u );
			return --m_refCount != 0u;
		}

	protected:

		GraphicsStateObject()
		{
			m_refCount = 0u;
			m_hashValue = 0u;
		}

		~GraphicsStateObject()
		{
			TIKI_ASSERT( m_refCount == 0u );
			TIKI_ASSERT( m_hashValue == 0u );
		}

		TIKI_FORCE_INLINE void	create( crc32 hashValue )
		{
			m_refCount = 1u;
			m_hashValue = hashValue;
		}

		TIKI_FORCE_INLINE void	dispose()
		{
			m_refCount = 0u;
			m_hashValue = 0u;
		}

	private:

		size_t		m_refCount;
		crc32		m_hashValue;

	};

	template<typename T>
	class GraphicsStateObjectCollection
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GraphicsStateObjectCollection );

	public:

		TIKI_FORCE_INLINE void	create( size_t count )
		{
			m_data.create( count );
		}

		TIKI_FORCE_INLINE void	dispose()
		{
			m_data.dispose();
		}

		TIKI_FORCE_INLINE T*	findOrAllocate( crc32 hashValue )
		{
			T* pFirst = nullptr;

			for (size_t i = 0u; i < m_data.getCount(); ++i)
			{
				if ( m_data[ i ].icCreated() == false )
				{
					if ( pFirst == nullptr )
					{
						pFirst = &m_data[ i ];
					}

					continue;
				}

				if ( m_data[ i ].isCompatible( hashValue ) )
				{
					m_data[ i ].addRef();
					return &m_data[ i ];
				}
			}
			
			if ( pFirst == nullptr )
			{
				if ( m_data.isFull() )
				{
					return nullptr;
				}

				pFirst = &m_data.push();
			}

			static_cast< GraphicsStateObject* >( pFirst )->create( hashValue );
			return pFirst;
		}

	private:

		SizedArray< T >			m_data;
		
	};
}

#endif // TIKI_GRAPHICSSTATEOBJECT_HPP
