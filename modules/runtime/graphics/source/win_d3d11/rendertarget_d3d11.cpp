
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

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		m_colorBufferCount = colorBufferCount;
		TIKI_ASSERT( m_colorBufferCount <= GraphicsSystemLimits_RenderTargetSlots );

		for (size_t i = 0u; i < m_colorBufferCount; ++i)
		{
			m_colorBuffers[ i ] = pColorBuffers[ i ];

			TGRenderTargetDescription renderTargetDesc;
			renderTargetDesc.Format				= graphics::getD3dFormat( (PixelFormat)pColorBuffers[ i ].pDataBuffer->getDesription().format, TextureFlags_RenderTarget );
			renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice	= 0;

			const HRESULT result = graphics::getDevice( graphicsSystem )->CreateRenderTargetView( pColorBuffers[ i ].pDataBuffer->m_platformData.pResource, &renderTargetDesc, &m_platformData.pColorViews[ i ] );
			if ( FAILED( result ) || m_platformData.pColorViews[ i ] == nullptr )
			{
				dispose();
				return false;
			}
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
			depthDesc.Format				= graphics::getD3dFormat( (PixelFormat)pDepthBuffer->pDataBuffer->getDesription().format, TextureFlags_DepthStencil );
			depthDesc.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
			depthDesc.Texture2D.MipSlice	= 0u;
			depthDesc.Flags					= 0u;

			const HRESULT result = graphics::getDevice( graphicsSystem )->CreateDepthStencilView( pDepthBuffer->pDataBuffer->m_platformData.pResource, &depthDesc, &m_platformData.pDepthView );
			if ( FAILED( result ) || m_platformData.pDepthView == nullptr )
			{
				dispose();
				return false;
			}
		}
		else
		{
			m_depthBuffer.format		= PixelFormat_Invalid;
			m_depthBuffer.pDataBuffer	= nullptr;

			m_platformData.pDepthView	= nullptr;
		}

		return true;
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
