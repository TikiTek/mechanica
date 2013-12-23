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

		TIKI_FORCE_INLINE			ConstantBuffer();
		TIKI_FORCE_INLINE			~ConstantBuffer();

		TIKI_FORCE_INLINE void		create( GraphicsSystem& graphicsSystem, size_t bufferSize );
		TIKI_FORCE_INLINE void		dispose( GraphicsSystem& graphicsSystem );

		TIKI_FORCE_INLINE size_t	getSize() const { return m_size; }

	private:

		size_t						m_size;

	};
}

#include "../../../source/global/constantbuffer.inl"

#endif // TIKI_CONSTANTBUFFER_HPP
