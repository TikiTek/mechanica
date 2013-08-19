#ifndef TIKI_STATICBUFFER_HPP__
#define TIKI_STATICBUFFER_HPP__

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/types.hpp"

#include "graphicstypes.hpp"
#include "graphicshandles.hpp"

namespace tiki
{
	template<uint TBinding>
	class StaticBuffer
	{
		friend class GpuContext;
		TIKI_NONCOPYABLE_CLASS( StaticBuffer );

	public:

						StaticBuffer();
		virtual			~StaticBuffer();

		void			create( const void* pData, size_t elementStride, size_t elementCount );
		void			dispose();

		void			apply() const;

		size_t			getType() const { return TBinding; }
		size_t			getStride() const { return m_elementSize; }
		
	private:

		TGBuffer*		m_pBuffer;

		size_t			m_elementSize;
		size_t			m_elementCount;

	};
}

#include "staticbuffer.inl"

namespace tiki
{
	class SVertexBuffer		: public StaticBuffer< 1u > { };
	class SIndexBuffer		: public StaticBuffer< 2u > { };
	class SConstantBuffer	: public StaticBuffer< 4u > { };
}

#endif // TIKI_STATICBUFFER_HPP__
