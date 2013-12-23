#pragma once
#ifndef __TIKI_INDEXBUFFER_INL_INCLUDED__
#define __TIKI_INDEXBUFFER_INL_INCLUDED__

namespace tiki
{
	TIKI_FORCE_INLINE IndexBuffer::IndexBuffer()
	{
		m_indexType = IndexType_Invalid;
		m_count		= 0u;
	}

	TIKI_FORCE_INLINE IndexBuffer::~IndexBuffer()
	{
		TIKI_ASSERT( m_count == 0u );
		TIKI_ASSERT( m_indexType == IndexType_Invalid );
	}

	TIKI_FORCE_INLINE void IndexBuffer::create( GraphicsSystem& graphicsSystem, size_t indexCount, IndexType type, bool dynamic /*= true*/, const void* pInitData /*= nullptr*/ )
	{
		m_indexType	= type;
		m_count		= indexCount;

		BaseBuffer::create( graphicsSystem, indexCount * (size_t)type, dynamic, GraphicsBufferType_IndexBuffer, pInitData );
	}

	TIKI_FORCE_INLINE void IndexBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		m_indexType	= IndexType_Invalid;
		m_count		= 0u;

		BaseBuffer::dispose( graphicsSystem );
	}
}

#endif // __TIKI_INDEXBUFFER_INL_INCLUDED__
