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

		IndexType_UInt16	= 2u,
		IndexType_UInt32	= 4u,

		IndexType_MaxSize	= IndexType_UInt32
	};

	class IndexBuffer : public BaseBuffer
	{
		TIKI_NONCOPYABLE_CLASS( IndexBuffer );
		friend class GraphicsContext;

	public:

		TIKI_FORCE_INLINE			IndexBuffer();
		TIKI_FORCE_INLINE			~IndexBuffer();

		TIKI_FORCE_INLINE void		create( GraphicsSystem& graphicsSystem, size_t indexCount, IndexType type, bool dynamic = true, const void* pInitData = nullptr );
		TIKI_FORCE_INLINE void		dispose( GraphicsSystem& graphicsSystem );

		TIKI_FORCE_INLINE IndexType	getIndexType() const	{ return m_indexType; }

		TIKI_FORCE_INLINE size_t	getCount() const		{ return m_count; }

	private:

		IndexType					m_indexType;
		size_t						m_count;

	};
}

#include "../../../source/global/indexbuffer.inl"

#endif // TIKI_INDEXBUFFER_HPP
