#include "tiki/graphics/rendertarget.hpp"

#include "graphicshandles.hpp"
#include "tiki/framework/framework.hpp"

namespace tiki
{
	RenderTarget::RenderTarget() 
		: m_pRenderTarget( nullptr )
	{
	}

	RenderTarget::~RenderTarget()
	{
	}

	void RenderTarget::create( GraphicsSystem& graphicsSystem, PixelFormat format, size_t width /* = 0u */, size_t height /* = 0u */ )
	{
		const uint2 backBufferSize = graphicsSystem.getBackBufferSize();

		width	= width  == 0u ? backBufferSize.x : width;
		height	= height == 0u ? backBufferSize.y : height;

		TextureDescription desc;
		desc.width	= width;
		desc.height	= height;
		desc.depth	= 1u;
		desc.arrayCount	= 1u;
		
		desc.format		= format;
		desc.type		= TextureType_2d;
		desc.flags		= TextureFlags_RenderTarget | TextureFlags_ShaderInput;

		TIKI_VERIFY( m_textureData.create( graphicsSystem, desc, nullptr ) );

		TGTexture2DDesc dxDesc;
		m_textureData.m_pTexture2d->GetDesc( &dxDesc );

		TGRenderTargetDescription renderTargetDesc;
		renderTargetDesc.Format				= dxDesc.Format;
		renderTargetDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D.MipSlice	= 0;

		TIKI_VERIFY( SUCCEEDED( getHandles( graphicsSystem )->pDevice->CreateRenderTargetView( m_textureData.m_pTexture2d, &renderTargetDesc, &m_pRenderTarget ) ) );
	}

	void RenderTarget::dispose()
	{
		m_textureData.dispose();

		if ( m_pRenderTarget != nullptr ) 
		{
			m_pRenderTarget->Release();
			m_pRenderTarget = nullptr;
		}
	}
}
