
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	TGFormat graphics::getD3dFormat( PixelFormat pixelFormat )
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

	static UINT getD3dFlags( TextureFlags flags )
	{
		UINT result = 0u;

		if ( isBitSet( flags, TextureFlags_RenderTarget ) )
		{
			result |= D3D11_BIND_RENDER_TARGET;
		}

		if ( isBitSet( flags, TextureFlags_DepthStencil ) )
		{
			result |= D3D11_BIND_DEPTH_STENCIL;
		}

		if ( isBitSet( flags, TextureFlags_ShaderInput ) )
		{
			result |= D3D11_BIND_SHADER_RESOURCE;
		}

		return result;
	}

	TextureData::TextureData()
	{
	}

	TextureData::~TextureData()
	{
		TIKI_ASSERT( m_platformData.pResource == nullptr );
		TIKI_ASSERT( m_platformData.pShaderView == nullptr );
	}

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pInitData /*= nullptr */ )
	{
		TIKI_ASSERT( m_platformData.pResource == nullptr );
		TIKI_ASSERT( m_platformData.pShaderView == nullptr );
		TIKI_ASSERT( description.type == TextureType_2d ); // currently only 2D textures supported

		m_description = description;

		TIKI_DECLARE_STACKANDZERO( TGTexture2DDesc, desc );
		desc.Format				= graphics::getD3dFormat( (PixelFormat)description.format );
		desc.Width				= description.width;
		desc.Height				= description.height;		
		desc.Usage				= D3D11_USAGE_DEFAULT;
		desc.MipLevels			= 1u;
		desc.ArraySize			= description.arrayCount;
		desc.SampleDesc.Count	= 1u;
		desc.BindFlags			= getD3dFlags( (TextureFlags)description.flags );		

		if ( FAILED( graphics::getDevice( graphicsSystem )->CreateTexture2D( &desc, nullptr, &m_platformData.pTexture2d ) ) )
		{
			TIKI_TRACE_ERROR( "[grpahics] Can't create Texture.\n" );
			return false;
		}

		if ( pInitData != nullptr )
		{
			const size_t rowPitch	= description.width * ( getBitsPerPixel( (PixelFormat)description.format ) / 8u );
			const size_t depthPitch	= rowPitch * description.height;

			graphics::getContext( graphicsSystem )->UpdateSubresource( m_platformData.pResource, 0u, nullptr, pInitData, rowPitch, depthPitch );
		}

		if ( isBitSet( description.flags, TextureFlags_ShaderInput ) )
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;

			if ( FAILED( graphics::getDevice( graphicsSystem )->CreateShaderResourceView( m_platformData.pResource, &srvDesc, &m_platformData.pShaderView ) ) )
			{
				TIKI_TRACE_ERROR( "[grpahics] Can't create ShaderView.\n" );
				return false;
			}
		}

		return true;
	}

	void TextureData::dispose( GraphicsSystem& graphicsSystem )
	{
		if ( m_platformData.pShaderView != nullptr )
		{
			m_platformData.pShaderView->Release();
			m_platformData.pShaderView = nullptr;
		}

		if ( m_platformData.pResource != nullptr )
		{
			m_platformData.pResource->Release();
			m_platformData.pResource = nullptr;
		}
	}
}