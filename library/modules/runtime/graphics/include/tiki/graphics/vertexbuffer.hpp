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

		VertexBuffer()
		{
			m_stride	= 0u;
			m_count		= 0u;
		}

		~VertexBuffer()
		{
			TIKI_ASSERT( m_stride == 0u );
			TIKI_ASSERT( m_count == 0u );
		}

		TIKI_FORCE_INLINE void		create( GraphicsSystem& graphicsSystem, size_t vertexCount, size_t vertexStride, bool dynamic = true, const void* pInitData = nullptr )
		{
			m_stride	= vertexStride;
			m_count		= vertexCount;

			BaseBuffer::create( graphicsSystem, vertexCount * vertexStride, dynamic, GraphicsBufferType_VertexBuffer, pInitData );
		}

		TIKI_FORCE_INLINE void		dispose()
		{
			m_stride	= 0u;
			m_count		= 0u;

			BaseBuffer::dispose();
		}

		TIKI_FORCE_INLINE size_t	getStride() const
		{
			return m_stride;
		}
		
		TIKI_FORCE_INLINE size_t	getCount() const
		{
			return m_count;
		}
			
	private:

		size_t						m_stride;
		size_t						m_count;

	};
}

#endif // TIKI_VERTEXBUFFER_HPP
