
#include "tiki/graphics/basebuffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	BaseBuffer::BaseBuffer()
	{
		m_bufferId = 0u;
	}

	BaseBuffer::~BaseBuffer()
	{
		TIKI_ASSERT( m_bufferId == 0u );
	}

	bool BaseBuffer::create( GraphicsSystem& graphicsSystem, size_t size, bool dynamic, GraphicsBufferType binding, const void* pInitData /*= nullptr*/ )
	{
		TIKI_ASSERT( m_bufferId == 0u );

		m_dynamic = dynamic;

		return true;
	}

	void BaseBuffer::dispose( GraphicsSystem& graphicsSystem )
	{
	}
}