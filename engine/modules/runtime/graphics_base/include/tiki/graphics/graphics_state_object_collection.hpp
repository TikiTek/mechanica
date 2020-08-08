#pragma once

#include "tiki/container/sized_array.hpp"

namespace tiki
{
	template<typename T>
	class GraphicsStateObjectCollection
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GraphicsStateObjectCollection );

	public:

		TIKI_FORCE_INLINE void	create( uintreg count );
		TIKI_FORCE_INLINE void	dispose();

		TIKI_FORCE_INLINE T*	findOrAllocate( crc32 hashValue );

	private:

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		uintreg					m_maxCount;
#endif

		SizedArray< T >			m_data;
	};
}

#include "../../../source/graphics_state_object_collection.inl"
