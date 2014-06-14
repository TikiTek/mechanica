#pragma once
#ifndef __TIKI_GRAPHICSSTATEOBJECTCOLLECTION_INL_INCLUDED__
#define __TIKI_GRAPHICSSTATEOBJECTCOLLECTION_INL_INCLUDED__

#include "tiki/base/assert.hpp"

namespace tiki
{
	template<typename T>
	TIKI_FORCE_INLINE void GraphicsStateObjectCollection::create( size_t count )
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_maxCount = 0u;
#endif

		m_data.create( count );
	}

	template<typename T>
	TIKI_FORCE_INLINE void GraphicsStateObjectCollection::dispose()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		TIKI_TRACE_INFO( "[GraphicsStateObjectCollection] Max usage (%u / %u)\n", m_maxCount, m_data.getCapacity() );
#endif

		m_data.dispose();
	}

	template<typename T>
	TIKI_FORCE_INLINE T* GraphicsStateObjectCollection::findOrAllocate( crc32 hashValue )
	{
		T* pFirst = nullptr;

		for (size_t i = 0u; i < m_data.getCount(); ++i)
		{
			if ( m_data[ i ].isCreated() == false )
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

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_maxCount = TIKI_MAX( m_maxCount, m_data.getCount() );
#endif

		static_cast< GraphicsStateObject* >( pFirst )->create( hashValue );
		return pFirst;
	}
}

#endif // __TIKI_GRAPHICSSTATEOBJECTCOLLECTION_INL_INCLUDED__
