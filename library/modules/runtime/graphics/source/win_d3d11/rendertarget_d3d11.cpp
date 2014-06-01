
#include "tiki/graphics/rendertarget.hpp"

#include "tiki/graphics/texturedata.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	static void checkSize( uint& targetWidth, uint& targetHeight, uint width, uint height )
	{
		if ( targetWidth == 0u )
		{
			targetWidth = width;
		}

		if ( targetHeight == 0u )
		{
			targetHeight = height;
		}

		TIKI_ASSERT( targetWidth == width );
		TIKI_ASSERT( targetHeight == height );
	}

	RenderTarget::RenderTarget()
		: m_width( 0u ), m_height( 0u ), m_colorBufferCount( 0u )
	{
		for (uint i = 0u; i < TIKI_COUNT( m_platformData.pColorViews ); ++i)
		{
			m_platformData.pColorViews[ i ] = nullptr;
		} 

		m_platformData.pDepthView = nullptr;
	}

	RenderTarget::~RenderTarget()
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		for (uint i = 0u; i < TIKI_COUNT( m_platformData.pColorViews ); ++i)
		{
			TIKI_ASSERT( m_platformData.pColorViews[ i ] == nullptr );
		} 

		TIKI_ASSERT( m_platformData.pDepthView == nullptr );
#endif
	}

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		TIKI_ASSERT( colorBufferCount <= GraphicsSystemLimits_RenderTargetSlots );

		m_colorBufferCount	= colorBufferCount;
		m_width				= width;
		m_height			= height;

		TIKI_ASSERT( pColorBuffers != nullptr ||colorBufferCount == 0u );
		for (size_t i = 0u; i < m_colorBufferCount; ++i)
		{
			m_colorBuffers[ i ] = pColorBuffers[ i ];
			TIKI_ASSERT( pColorBuffers[ i ].pDataBuffer != nullptr );

			checkSize( m_width, m_height, pColorBuffers[ i ].pDataBuffer->getWidth(), pColorBuffers[ i ].pDataBuffer->getHeight() );

			TGRenderTargetDescription renderTargetDesc;
			renderTargetDesc.Format				= graphics::getD3dFormat( (PixelFormat)pColorBuffers[ i ].pDataBuffer->getDescription().format, TextureFlags_RenderTarget );
			renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice	= 0;

			const HRESULT result = graphics::getDevice( graphicsSystem )->CreateRenderTargetView( pColorBuffers[ i ].pDataBuffer->m_platformData.pResource, &renderTargetDesc, &m_platformData.pColorViews[ i ] );
			if ( FAILED( result ) || m_platformData.pColorViews[ i ] == nullptr )
			{
				dispose( graphicsSystem );
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

			checkSize( m_width, m_height, pDepthBuffer->pDataBuffer->getWidth(), pDepthBuffer->pDataBuffer->getHeight() );

			TGDepthStencilDescription depthDesc;
			depthDesc.Format				= graphics::getD3dFormat( (PixelFormat)pDepthBuffer->pDataBuffer->getDescription().format, TextureFlags_DepthStencil );
			depthDesc.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
			depthDesc.Texture2D.MipSlice	= 0u;
			depthDesc.Flags					= 0u;

			const HRESULT result = graphics::getDevice( graphicsSystem )->CreateDepthStencilView( pDepthBuffer->pDataBuffer->m_platformData.pResource, &depthDesc, &m_platformData.pDepthView );
			if ( FAILED( result ) || m_platformData.pDepthView == nullptr )
			{
				dispose( graphicsSystem );
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

	void RenderTarget::dispose( GraphicsSystem& graphicsSystem )
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

		m_colorBufferCount	= 0u;
		m_width				= 0u;
		m_height			= 0u;
	}
}
