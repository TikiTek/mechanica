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

	TIKI_FORCE_INLINE bool IndexBuffer::create( GraphicsSystem& graphicsSystem, uintreg indexCount, IndexType type, bool dynamic /*= true*/, const void* pInitData /*= nullptr*/, const char* pDebugName /*= nullptr*/ )
	{
		m_indexType	= type;
		m_count		= indexCount;

		if( !BaseBuffer::create( graphicsSystem, indexCount * (uintreg)type, dynamic, BaseBufferTypes_IndexBuffer, pInitData, pDebugName ) )
		{
			dispose( graphicsSystem );
			return false;
		}

		return true;
	}

	TIKI_FORCE_INLINE void IndexBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		m_indexType	= IndexType_Invalid;
		m_count		= 0u;

		BaseBuffer::dispose( graphicsSystem );
	}
}

#endif // __TIKI_INDEXBUFFER_INL_INCLUDED__
