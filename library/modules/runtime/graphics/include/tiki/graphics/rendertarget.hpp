#ifndef TIKI_RENDERTARGET_HPP__
#define TIKI_RENDERTARGET_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphicsbase/pixelformat.hpp"

#include "graphicstypes.hpp"

namespace tiki
{
	class RenderTarget
	{
		TIKI_NONCOPYABLE_CLASS( RenderTarget );

	public:
									RenderTarget();
									~RenderTarget();

		void						create( GraphicsSystem& graphicsSystem, PixelFormat format, size_t width = 0u, size_t height = 0u );
		void						dispose();

		const TextureData&			getTextureData() const { return m_textureData; }
		const TGRenderTarget*		getRenderTarget() const { return m_pRenderTarget; }

	private:

		TextureData					m_textureData;
		TGRenderTarget*				m_pRenderTarget;

	};
}

#endif // TIKI_RENDERTARGET_HPP__
