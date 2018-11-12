#pragma  once

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphics_system_limits.hpp"
#include "tiki/graphics/pixel_format.hpp"

#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
#	include "../../../source/win_d3d11/render_target_d3d11.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
#	include "../../../source/win_d3d12/render_target_d3d12.hpp"
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
#	include "../../../source/global_vulkan/render_target_vulkan.hpp"
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
			clear();
		}

		RenderTargetBuffer( const TextureData& dataBuffer, PixelFormat pixelFormat = PixelFormat_Invalid )
		{
			format		= pixelFormat;
			pDataBuffer	= &dataBuffer;
		}

		PixelFormat			format;
		const TextureData*	pDataBuffer;

		TIKI_FORCE_INLINE void clear()
		{
			format		= PixelFormat_Invalid;
			pDataBuffer	= nullptr;
		}
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
		void						dispose( GraphicsSystem& graphicsSystem );

		uint						getWidth() const	{ return m_width; }
		uint						getHeight() const	{ return m_height; }

		const TextureData*			getColorTextureData( uint index ) const		{ TIKI_ASSERT( index < m_colorBufferCount ); return m_colorBuffers[ index ].pDataBuffer; }
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
