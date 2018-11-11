#pragma once
#ifndef __TIKI_GRAPHICSSTATEOBJECTCOLLECTION_HPP_INCLUDED__
#define __TIKI_GRAPHICSSTATEOBJECTCOLLECTION_HPP_INCLUDED__

#include "tiki/container/sizedarray.hpp"

namespace tiki
{
	template<typename T>
	class GraphicsStateObjectCollection
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GraphicsStateObjectCollection );

	public:

		TIKI_FORCE_INLINE void	create( size_t count );
		TIKI_FORCE_INLINE void	dispose();

		TIKI_FORCE_INLINE T*	findOrAllocate( crc32 hashValue );

	private:
		
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		uint					m_maxCount;
#endif

		SizedArray< T >			m_data;
		
	};
}

#include "../../../source/graphicsstateobjectcollection.inl"

#endif // __TIKI_GRAPHICSSTATEOBJECTCOLLECTION_HPP_INCLUDED__
