#pragma once

#include "tiki/graphics/base_buffer.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	enum IndexType : uint8
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

		TIKI_FORCE_INLINE bool		create( GraphicsSystem& graphicsSystem, uintreg indexCount, IndexType type, bool dynamic = true, const void* pInitData = nullptr, const char* pDebugName = nullptr );
		TIKI_FORCE_INLINE void		dispose( GraphicsSystem& graphicsSystem );

		TIKI_FORCE_INLINE IndexType	getIndexType() const	{ return m_indexType; }
		TIKI_FORCE_INLINE uintreg	getCount() const		{ return m_count; }

	private:

		IndexType					m_indexType;
		uintreg						m_count;
	};
}

#include "../../../source/global/index_buffer.inl"
