#ifndef TIKI_INDEXBUFFER_HPP
#define TIKI_INDEXBUFFER_HPP

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/basebuffer.hpp"

namespace tiki
{
	enum IndexType
	{		
		IndexType_Invalid	= 0u,
		IndexType_Uint16	= 2u,
		IndexType_Uint32	= 4u
	};

	class IndexBuffer : public BaseBuffer
	{
		TIKI_NONCOPYABLE_CLASS( IndexBuffer );
		friend class GraphicsContext;

	public:

		IndexBuffer()
		{
			m_indexType = IndexType_Invalid;
			m_count		= 0u;
		}

		~IndexBuffer()
		{
			TIKI_ASSERT( m_count == 0u );
			TIKI_ASSERT( m_indexType == IndexType_Invalid );
		}

		TIKI_FORCE_INLINE void		create( GraphicsSystem& graphicsSystem, size_t indexCount, IndexType type, bool dynamic = true, const void* pInitData = nullptr )
		{
			m_indexType	= type;
			m_count		= indexCount;

			BaseBuffer::create( graphicsSystem, indexCount * (size_t)type, dynamic, GraphicsBufferType_IndexBuffer, pInitData );
		}

		TIKI_FORCE_INLINE void		dispose()
		{
			m_indexType	= IndexType_Invalid;
			m_count		= 0u;

			BaseBuffer::dispose();
		}

		TIKI_FORCE_INLINE IndexType	getIndexType() const
		{
			return m_indexType;
		}

		TIKI_FORCE_INLINE size_t	getCount() const
		{
			return m_count;
		}

	private:

		IndexType					m_indexType;
		size_t						m_count;

	};
}

#endif // TIKI_INDEXBUFFER_HPP
