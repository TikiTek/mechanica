#ifndef TIKI_RENDERTARGET_HPP__
#define TIKI_RENDERTARGET_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicsystem.hpp"
#include "tiki/graphicsbase/pixelformat.hpp"

namespace tiki
{
	class GraphicsContext;

	struct RenderTargetBuffer
	{
		PixelFormat			format;
		const TextureData*	pDataBuffer;
	};

	class RenderTarget
	{
		TIKI_NONCOPYABLE_CLASS( RenderTarget );
		friend class GraphicsContext;

	public:

									~RenderTarget();

		void						create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer );
		void						dispose();

		const TextureData*			getColorTextureData( size_t index ) const { return m_colorBuffers[ index ].pDataBuffer; }
		size_t						getColorBufferCount() const { return m_colorBufferCount; }

		const TextureData*			getDepthTextureData() const { return m_depthBuffer.pDataBuffer; }

	private:

		RenderTargetBuffer			m_colorBuffers[ GraphicsSystemLimits_RenderTargetSlots ];
		TGRenderTarget*				m_pColorViews[ GraphicsSystemLimits_RenderTargetSlots ];
		size_t						m_colorBufferCount;

		RenderTargetBuffer			m_depthBuffer;
		TGRenderTarget*				m_pDepthView;

	};
}

#endif // TIKI_RENDERTARGET_HPP__
