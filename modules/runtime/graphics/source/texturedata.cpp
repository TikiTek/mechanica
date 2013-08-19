
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/bits.hpp"

#include "graphicshandles.hpp"

#include <d3d11.h>

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

	static UINT getD3dFlags( tiki::TextureFlags flags )
	{
		UINT result = 0u;

		if ( tiki::isBitSet( flags, TextureFlags_RenderTarget ) )
		{
			result |= D3D11_BIND_RENDER_TARGET;
		}

		if ( tiki::isBitSet( flags, TextureFlags_DepthStencil ) )
		{
			result |= D3D11_BIND_DEPTH_STENCIL;
		}

		if ( tiki::isBitSet( flags, TextureFlags_ShaderInput ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		return result;
	}

	TextureData::TextureData()
	{
		m_pResource		= nullptr;
		m_pShaderView	= nullptr;
	}

	TextureData::~TextureData()
	{
		TIKI_ASSERT( m_pResource == nullptr );
		TIKI_ASSERT( m_pShaderView == nullptr );
	}

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pInitData /*= nullptr */ )
	{
		TIKI_ASSERT( m_pResource == nullptr );
		TIKI_ASSERT( description.type == TextureType_2d ); // currently only 2D textures supported

		m_description = description;

		TIKI_DECLARE_STACKANDZERO( TGTexture2DDesc, desc );
		desc.Format				= getD3dFormat( (PixelFormat)description.format );
		desc.Width				= description.width;
		desc.Height				= description.height;
		desc.Usage				= D3D11_USAGE_DEFAULT;
		desc.MipLevels			= 1u;
		desc.ArraySize			= description.arrayCount;
		desc.SampleDesc.Count	= 1u;
		desc.BindFlags			= getD3dFlags( (TextureFlags)description.flags );		

		if ( FAILED( getHandles( graphicsSystem )->pDevice->CreateTexture2D( &desc, nullptr, &m_pTexture2d ) ) )
		{
			TIKI_TRACE_ERROR( "[grpahics] Can't create Texture.\n" );
			return false;
		}

		if ( pInitData != nullptr )
		{
			const size_t rowPitch	= description.width * ( getBitsPerPixel( (PixelFormat)description.format ) / 8u );
			const size_t depthPitch	= rowPitch * description.height;

			getHandles( graphicsSystem )->pContext->UpdateSubresource( m_pResource, 0u, nullptr, pInitData, rowPitch, depthPitch );
		}

		if ( isBitSet( description.flags, TextureFlags_ShaderInput ) )
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;

			if ( FAILED( getHandles( graphicsSystem )->pDevice->CreateShaderResourceView( m_pResource, &srvDesc, &m_pShaderView ) ) )
			{
				TIKI_TRACE_ERROR( "[grpahics] Can't create ShaderView.\n" );
				return false;
			}
		}

		return true;
	}

	void TextureData::dispose()
	{
		if ( m_pShaderView != nullptr )
		{
			m_pShaderView->Release();
			m_pShaderView = nullptr;
		}

		if ( m_pResource != nullptr )
		{
			m_pResource->Release();
			m_pResource = nullptr;
		}
	}
}