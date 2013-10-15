#include "tiki/graphics/rendertarget.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/texturedata.hpp"

#include "graphicshandles.hpp"

namespace tiki
{
	static DXGI_FORMAT getD3dFormat( tiki::PixelFormat pixelFormat )
	{
		switch ( pixelFormat )
		{
		case PixelFormat_R8:
			return DXGI_FORMAT_R8_UNORM;
		case PixelFormat_R8G8B8A8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PixelFormat_R8G8B8A8_Gamma:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case PixelFormat_R32_Float:
			return DXGI_FORMAT_R32_FLOAT;
		case PixelFormat_R32G32B32_Float:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case PixelFormat_R32G32B32A32_Float:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case PixelFormat_Depth24Stencil8:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	RenderTarget::RenderTarget()
		: m_width( 0u ), m_height( 0u ), m_colorBufferCount( 0u ), m_pDepthView( nullptr )
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
			renderTargetDesc.Format				= getD3dFormat( (PixelFormat)pColorBuffers[ i ].pDataBuffer->getDesription().format );
			renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice	= 0;

			TIKI_VERIFY( SUCCEEDED( getHandles( graphicsSystem )->pDevice->CreateRenderTargetView( pColorBuffers[ i ].pDataBuffer->m_pResource, &renderTargetDesc, &m_pColorViews[ i ] ) ) );
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
			m_depthBuffer = *pDepthBuffer;

			TGDepthStencilDescription depthDesc;
			depthDesc.Format				= getD3dFormat( (PixelFormat)pDepthBuffer->pDataBuffer->getDesription().format );
			depthDesc.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
			depthDesc.Texture2D.MipSlice	= 0u;

			TIKI_VERIFY( SUCCEEDED( getHandles( graphicsSystem )->pDevice->CreateDepthStencilView( pDepthBuffer->pDataBuffer->m_pResource, &depthDesc, &m_pDepthView ) ) );
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
