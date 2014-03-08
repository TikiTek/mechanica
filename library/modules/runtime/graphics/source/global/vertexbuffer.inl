#pragma once
#ifndef __TIKI_VERTEXBUFFER_INL_INCLUDED__
#define __TIKI_VERTEXBUFFER_INL_INCLUDED__

namespace tiki
{
	TIKI_FORCE_INLINE VertexBuffer::VertexBuffer()
	{
		m_stride	= 0u;
		m_count		= 0u;
	}

	TIKI_FORCE_INLINE VertexBuffer::~VertexBuffer()
	{
		TIKI_ASSERT( m_stride == 0u );
		TIKI_ASSERT( m_count == 0u );
	}

	TIKI_FORCE_INLINE bool VertexBuffer::create( GraphicsSystem& graphicsSystem, size_t vertexCount, size_t vertexStride, bool dynamic /*= true*/, const void* pInitData /*= nullptr*/ )
	{
		if ( BaseBuffer::create( graphicsSystem, vertexCount * vertexStride, dynamic, GraphicsBufferType_VertexBuffer, pInitData ) )
		{
			m_stride	= vertexStride;
			m_count		= vertexCount;

			return true;
		}

		return false;
	}

	TIKI_FORCE_INLINE void VertexBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		m_stride	= 0u;
		m_count		= 0u;

		BaseBuffer::dispose( graphicsSystem );
	}
}

#endif // __TIKI_VERTEXBUFFER_INL_INCLUDED__
