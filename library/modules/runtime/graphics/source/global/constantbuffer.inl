#pragma once
#ifndef __TIKI_CONSTANTBUFFER_INL_INCLUDED__
#define __TIKI_CONSTANTBUFFER_INL_INCLUDED__

namespace tiki
{
	TIKI_FORCE_INLINE ConstantBuffer::ConstantBuffer()
	{
		m_size = 0u;
	}

	TIKI_FORCE_INLINE ConstantBuffer::~ConstantBuffer()
	{
		TIKI_ASSERT( m_size == 0u );
	}

	TIKI_FORCE_INLINE bool ConstantBuffer::create( GraphicsSystem& graphicsSystem, uint bufferSize )
	{
		m_size = bufferSize;

		if( !BaseBuffer::create( graphicsSystem, bufferSize, true, GraphicsBufferType_ConstantBuffer ) )
		{
			dispose( graphicsSystem );
			return false;
		}

		return true;
	}

	TIKI_FORCE_INLINE void ConstantBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
		m_size = 0u;

		BaseBuffer::dispose( graphicsSystem );
	}
}

#endif // __TIKI_CONSTANTBUFFER_INL_INCLUDED__
