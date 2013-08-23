#include "tiki/graphics/rendertarget.hpp"

#include "graphicshandles.hpp"
#include "tiki/framework/framework.hpp"

namespace tiki
{
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
			renderTargetDesc.Format				= getD3dFormat( pColorBuffers[ i ].pDataBuffer->getDesription().format );
			renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice	= 0;

			TIKI_VERIFY( SUCCEEDED( getHandles( graphicsSystem )->pDevice->CreateRenderTargetView( m_textureData.m_pTexture2d, &renderTargetDesc, &m_pColorViews[ i ] ) ) );
			TIKI_ASSERT( m_pColorViews[ i ] != nullptr );
		}

		for (size_t i = m_colorBufferCount; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].format		= PixelFormat_Invalid;
			m_colorBuffers[ i ].pDataBuffer	= nullptr;

			m_pColorViews[ i ] = nullptr;
		}

		if ( pDepthBuffer != nullptr )
		{
			m_depthBuffer = pDepthBuffer;

			TGRenderTargetDescription renderTargetDesc;
			renderTargetDesc.Format				= getD3dFormat( pDepthBuffer->pDataBuffer->getDesription().format );
			renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice	= 0;

			TIKI_VERIFY( SUCCEEDED( getHandles( graphicsSystem )->pDevice->CreateRenderTargetView( m_textureData.m_pTexture2d, &renderTargetDesc, &m_pDepthView ) ) );
		}
		else
		{
			m_depthBuffer.format		= PixelFormat_Invalid;
			m_depthBuffer.pDataBuffer	= nullptr;

			m_pDepthView = nullptr;
		}
	}

	void RenderTarget::dispose()
	{
		for (size_t i = 0u; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].format		= PixelFormat_Invalid;
			m_colorBuffers[ i ].pDataBuffer	= nullptr;

			if ( m_pColorViews[ i ] != nullptr )
			{
				m_pColorViews[ i ]->Release();
				m_pColorViews[ i ] = nullptr;
			}
		}

		if ( m_pDepthView != nullptr )
		{
			m_pDepthView->Release();
			m_pDepthView = nullptr;
		}
	}
}
