#pragma  once
#ifndef TIKI_RENDERTARGET_HPP__
#define TIKI_RENDERTARGET_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphicsbase/pixelformat.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "win_d3d11/rendertarget_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
#	include "global_opengl4/rendertarget_opengl4.hpp"
#endif

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class TextureData;

	struct RenderTargetBuffer
	{
		PixelFormat			format;
		const TextureData*	pDataBuffer;
	};

	class RenderTarget
	{
		TIKI_NONCOPYABLE_CLASS( RenderTarget );
		friend class GraphicsContext;
		friend class GraphicsSystem;

	public:

		RenderTarget();
		~RenderTarget();

		void						create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer );
		void						dispose();

		const TextureData*			getColorTextureData( size_t index ) const	{ TIKI_ASSERT( index < m_colorBufferCount ); return m_colorBuffers[ index ].pDataBuffer; }
		size_t						getColorBufferCount() const					{ return m_colorBufferCount; }

		const TextureData*			getDepthTextureData() const					{ return m_depthBuffer.pDataBuffer; }
		
	private:

		size_t						m_width;
		size_t						m_height;

		RenderTargetBuffer			m_colorBuffers[ GraphicsSystemLimits_RenderTargetSlots ];
		size_t						m_colorBufferCount;

		RenderTargetBuffer			m_depthBuffer;

		RenderTargetPlatformData	m_platformData;

	};
}

#endif // TIKI_RENDERTARGET_HPP__
