#ifndef TIKI_VERTEXBUFFER_HPP
#define TIKI_VERTEXBUFFER_HPP

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/basebuffer.hpp"

namespace tiki
{
	class VertexBuffer : public BaseBuffer
	{
		TIKI_NONCOPYABLE_CLASS( VertexBuffer );
		friend class GraphicsContext;

	public:

		TIKI_FORCE_INLINE			VertexBuffer();
		TIKI_FORCE_INLINE			~VertexBuffer();

		TIKI_FORCE_INLINE bool		create( GraphicsSystem& graphicsSystem, size_t vertexCount, size_t vertexStride, bool dynamic = true, const void* pInitData = nullptr, const char* pDebugName = nullptr );
		TIKI_FORCE_INLINE void		dispose( GraphicsSystem& graphicsSystem );

		TIKI_FORCE_INLINE size_t	getStride() const	{ return m_stride; }
		
		TIKI_FORCE_INLINE size_t	getCount() const	{ return m_count; }
			
	private:

		size_t						m_stride;
		size_t						m_count;

	};
}

#include "../../../source/global/vertexbuffer.inl"

#endif // TIKI_VERTEXBUFFER_HPP
