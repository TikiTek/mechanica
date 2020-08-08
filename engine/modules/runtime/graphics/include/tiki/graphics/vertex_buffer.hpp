#pragma once

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/base_buffer.hpp"

namespace tiki
{
	class VertexBuffer : public BaseBuffer
	{
		TIKI_NONCOPYABLE_CLASS( VertexBuffer );
		friend class GraphicsContext;

	public:

		TIKI_FORCE_INLINE			VertexBuffer();
		TIKI_FORCE_INLINE			~VertexBuffer();

		TIKI_FORCE_INLINE bool		create( GraphicsSystem& graphicsSystem, uintreg vertexCount, uintreg vertexStride, bool dynamic = true, const void* pInitData = nullptr, const char* pDebugName = nullptr );
		TIKI_FORCE_INLINE void		dispose( GraphicsSystem& graphicsSystem );

		TIKI_FORCE_INLINE uintreg	getStride() const	{ return m_stride; }

		TIKI_FORCE_INLINE uintreg	getCount() const	{ return m_count; }

	private:

		uintreg						m_stride;
		uintreg						m_count;

	};
}

#include "../../../source/global/vertex_buffer.inl"
