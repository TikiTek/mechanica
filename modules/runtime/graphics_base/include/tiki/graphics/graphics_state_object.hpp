#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	class GraphicsStateObject
	{
		TIKI_NONCOPYABLE_CLASS( GraphicsStateObject );
		template<typename T>
		friend class GraphicsStateObjectCollection;

	public:

		virtual bool				isCreated() const = 0;

		TIKI_FORCE_INLINE bool		isCompatible( crc32 hashValue ) const;
		TIKI_FORCE_INLINE crc32		getHashValue() const;

		TIKI_FORCE_INLINE void		addRef();
		TIKI_FORCE_INLINE bool		releaseRef();

	protected:

		TIKI_FORCE_INLINE 			GraphicsStateObject();
		TIKI_FORCE_INLINE virtual	~GraphicsStateObject();

		TIKI_FORCE_INLINE void		create( crc32 hashValue );
		TIKI_FORCE_INLINE void		dispose();

	private:

		size_t		m_refCount;
		crc32		m_hashValue;
	};
}

#include "../../../source/graphics_state_object.inl"
