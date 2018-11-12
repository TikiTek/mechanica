#pragma once

#include "tiki/graphics/base_buffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class ConstantBuffer : public BaseBuffer
	{
		TIKI_NONCOPYABLE_CLASS( ConstantBuffer );
		friend class GraphicsContext;

	public:

		TIKI_FORCE_INLINE			ConstantBuffer();
		TIKI_FORCE_INLINE			~ConstantBuffer();

		TIKI_FORCE_INLINE bool		create( GraphicsSystem& graphicsSystem, uint bufferSize, const char* pDebugName );
		TIKI_FORCE_INLINE void		dispose( GraphicsSystem& graphicsSystem );

		TIKI_FORCE_INLINE uint		getSize() const { return m_size; }

	private:

		uint						m_size;
	};
}

#include "../../../source/global/constant_buffer.inl"
