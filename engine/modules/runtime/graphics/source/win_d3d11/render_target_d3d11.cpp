#include "tiki/graphics/render_target.hpp"

#include "tiki/graphics/graphics_system.hpp"
#include "tiki/graphics/texture_data.hpp"

#include <d3d11.h>

namespace tiki
{
	static bool checkSize( uint16& targetWidth, uint16& targetHeight, uint16 width, uint16 height )
	{
		if ( targetWidth == 0u )
		{
			targetWidth = width;
		}

		if ( targetHeight == 0u )
		{
			targetHeight = height;
		}

		return targetWidth == width && targetHeight == height;
	}

	RenderTarget::RenderTarget()
		: m_width( 0u )
		, m_height( 0u )
		, m_pDepthBuffer( nullptr )
	{
	}

	RenderTarget::~RenderTarget()
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		for (uintreg i = 0u; i < TIKI_COUNT( m_platformData.pColorViews ); ++i)
		{
			TIKI_ASSERT( m_platformData.pColorViews[ i ] == nullptr );
		}

		TIKI_ASSERT( m_platformData.pDepthView == nullptr );
#endif
	}

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, uint16 width, uint16 height, const TextureData** ppColorBuffers, uintreg colorBufferCount, const TextureData* pDepthBuffer )
	{
		TIKI_ASSERT( colorBufferCount <= GraphicsSystemLimits_RenderTargetSlots );

		m_width				= width;
		m_height			= height;

		for (uintreg i = 0u; i < colorBufferCount; ++i)
		{
			TIKI_ASSERT( ppColorBuffers[ i ] != nullptr );
			m_colorBuffers.push( ppColorBuffers[ i ] );

			if( !checkSize( m_width, m_height, ppColorBuffers[ i ]->getWidth(), ppColorBuffers[ i ]->getHeight() ) )
			{
				dispose( graphicsSystem );
				return false;
			}

			D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
			renderTargetDesc.Format				= graphics::getD3dFormat( ppColorBuffers[ i ]->getDescription().format, TextureFlags_RenderTarget, false );
			renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetDesc.Texture2D.MipSlice	= 0;

			const HRESULT result = GraphicsSystemPlatform::getDevice( graphicsSystem )->CreateRenderTargetView( ppColorBuffers[ i ]->m_platformData.pResource, &renderTargetDesc, &m_platformData.pColorViews[ i ] );
			if ( FAILED( result ) || m_platformData.pColorViews[ i ] == nullptr )
			{
				dispose( graphicsSystem );
				return false;
			}
		}

		if ( pDepthBuffer != nullptr )
		{
			m_pDepthBuffer = pDepthBuffer;

			if( !checkSize( m_width, m_height, pDepthBuffer->getWidth(), pDepthBuffer->getHeight() ) )
			{
				return false;
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
			depthDesc.Format				= graphics::getD3dFormat( pDepthBuffer->getDescription().format, TextureFlags_DepthStencil, false );
			depthDesc.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
			depthDesc.Texture2D.MipSlice	= 0u;
			depthDesc.Flags					= 0u;

			const HRESULT result = GraphicsSystemPlatform::getDevice( graphicsSystem )->CreateDepthStencilView( pDepthBuffer->m_platformData.pResource, &depthDesc, &m_platformData.pDepthView );
			if ( FAILED( result ) || m_platformData.pDepthView == nullptr )
			{
				dispose( graphicsSystem );
				return false;
			}
		}
		else
		{
			m_platformData.pDepthView = nullptr;
		}

		return true;
	}

	void RenderTarget::dispose( GraphicsSystem& graphicsSystem )
	{
		for (uintreg i = 0u; i < m_colorBuffers.getCount(); ++i)
		{
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

		m_colorBuffers.clear();

		m_pDepthBuffer		= nullptr;
		m_width				= 0u;
		m_height			= 0u;
	}
}
