
#include "tiki/graphics/rendertarget.hpp"

#include "tiki/graphics/texturedata.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	RenderTarget::RenderTarget()
		: m_width( 0u ), m_height( 0u ), m_colorBufferCount( 0u )
	{		
	}

	RenderTarget::~RenderTarget()
	{
	}

	void RenderTarget::create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		m_colorBufferCount = colorBufferCount;
		TIKI_ASSERT( m_colorBufferCount <= GraphicsSystemLimits_RenderTargetSlots );

		for (size_t i = 0u; i < m_colorBufferCount; ++i)
		{
			m_colorBuffers[ i ] = pColorBuffers[ i ];

			TGRenderTargetDescription renderTargetDesc;
			renderTargetDesc.Format				= graphics::getD3dFormat( (PixelFormat)pColorBuffers[ i ].pDataBuffer->getDesription().format );
			renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice	= 0;

			TIKI_VERIFY( SUCCEEDED( graphics::getDevice( graphicsSystem )->CreateRenderTargetView( pColorBuffers[ i ].pDataBuffer->m_platformData.pResource, &renderTargetDesc, &m_platformData.pColorViews[ i ] ) ) );
			TIKI_ASSERT( m_platformData.pColorViews[ i ] != nullptr );
		}

		for (size_t i = m_colorBufferCount; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].format		= PixelFormat_Invalid;
			m_colorBuffers[ i ].pDataBuffer	= nullptr;

			m_platformData.pColorViews[ i ] = nullptr;
		}

		if ( pDepthBuffer != nullptr )
		{
			m_depthBuffer = *pDepthBuffer;

			TGDepthStencilDescription depthDesc;
			depthDesc.Format				= graphics::getD3dFormat( (PixelFormat)pDepthBuffer->pDataBuffer->getDesription().format );
			depthDesc.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
			depthDesc.Texture2D.MipSlice	= 0u;

			TIKI_VERIFY( SUCCEEDED( graphics::getDevice( graphicsSystem )->CreateDepthStencilView( pDepthBuffer->pDataBuffer->m_platformData.pResource, &depthDesc, &m_platformData.pDepthView ) ) );
			TIKI_ASSERT( m_platformData.pDepthView != nullptr );
		}
		else
		{
			m_depthBuffer.format		= PixelFormat_Invalid;
			m_depthBuffer.pDataBuffer	= nullptr;

			m_platformData.pDepthView	= nullptr;
		}
	}

	void RenderTarget::dispose()
	{
		for (size_t i = 0u; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].format		= PixelFormat_Invalid;
			m_colorBuffers[ i ].pDataBuffer	= nullptr;

			if ( m_platformData.pColorViews[ i ] != nullptr )
			{
				m_platformData.pColorViews[ i ]->Release();
				m_platformData.pColorViews[ i ] = nullptr;
			}
		}

		if ( m_platformData.pDepthView != nullptr )
		{
			m_platformData.pDepthView->Release();
			m_platformData.pDepthView = nullptr;
		}
	}
}
