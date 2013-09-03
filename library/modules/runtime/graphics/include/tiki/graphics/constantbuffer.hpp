#ifndef TIKI_CONSTANTBUFFER_HPP
#define TIKI_CONSTANTBUFFER_HPP

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/basebuffer.hpp"

namespace tiki
{
	class ConstantBuffer : public BaseBuffer
	{
		TIKI_NONCOPYABLE_CLASS( ConstantBuffer );
		friend class GraphicsContext;

	public:

		ConstantBuffer()
		{
			m_size = 0u;
		}

		~ConstantBuffer()
		{
			TIKI_ASSERT( m_size == 0u );
		}

		TIKI_FORCE_INLINE void		create( GraphicsSystem& graphicsSystem, size_t bufferSize )
		{
			m_size = bufferSize;

			BaseBuffer::create( graphicsSystem, bufferSize, true, GraphicsBufferType_ConstantBuffer );
		}

		TIKI_FORCE_INLINE void		dispose()
		{
			m_size = 0u;

			BaseBuffer::dispose();
		}

		TIKI_FORCE_INLINE size_t	getSize() const
		{
			return m_size;
		}

	private:

		size_t						m_size;

	};
}

#endif // TIKI_CONSTANTBUFFER_HPP
