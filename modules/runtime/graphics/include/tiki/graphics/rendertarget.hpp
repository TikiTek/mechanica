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
		RenderTargetBuffer()
		{
			format		= PixelFormat_Invalid;
			pDataBuffer	= nullptr;
		}

		RenderTargetBuffer( const TextureData& dataBuffer, PixelFormat pixelFormat = PixelFormat_Invalid )
		{
			format		= pixelFormat;
			pDataBuffer	= &dataBuffer;
		}

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

		bool						create( GraphicsSystem& graphicsSystem, uint width, uint height, const RenderTargetBuffer* pColorBuffers, uint colorBufferCount, const RenderTargetBuffer* pDepthBuffer );
		void						dispose();

		uint						getWidth() const	{ return m_width; }
		uint						getHeight() const	{ return m_height; }

		const TextureData*			getColorTextureData( uint index ) const	{ TIKI_ASSERT( index < m_colorBufferCount ); return m_colorBuffers[ index ].pDataBuffer; }
		uint						getColorBufferCount() const					{ return m_colorBufferCount; }

		const TextureData*			getDepthTextureData() const					{ return m_depthBuffer.pDataBuffer; }
		
	private:

		uint						m_width;
		uint						m_height;

		RenderTargetBuffer			m_colorBuffers[ GraphicsSystemLimits_RenderTargetSlots ];
		size_t						m_colorBufferCount;

		RenderTargetBuffer			m_depthBuffer;

		RenderTargetPlatformData	m_platformData;

	};
}

#endif // TIKI_RENDERTARGET_HPP__
