
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	TGFormat graphics::getD3dFormat( PixelFormat pixelFormat, TextureFlags flags )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count );

		static TGFormat s_formatLookup[] =
		{
			DXGI_FORMAT_R8_UNORM,				// PixelFormat_R8,
			DXGI_FORMAT_R8G8B8A8_UNORM,			// PixelFormat_R8G8B8A8
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,	// PixelFormat_R8G8B8A8_Gamma
			DXGI_FORMAT_R16G16B16A16_FLOAT,		// PixelFormat_R16G16B16A16_Float
			DXGI_FORMAT_R32_FLOAT,				// PixelFormat_R32_Float
			DXGI_FORMAT_R32G32B32_FLOAT,		// PixelFormat_R32G32B32_Float
			DXGI_FORMAT_R32G32B32A32_FLOAT,		// PixelFormat_R32G32B32A32_Float
			DXGI_FORMAT_D24_UNORM_S8_UINT		// PixelFormat_Depth24Stencil8
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_formatLookup ) == PixelFormat_Count );

		if ( pixelFormat == PixelFormat_Depth24Stencil8 && isBitSet( flags, TextureFlags_ShaderInput ) )
		{
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}

		return s_formatLookup[ pixelFormat ];
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

	static D3D11_SRV_DIMENSION getViewDimentions( TextureType type )
	{
		TIKI_ASSERT( type < TextureType_Count );

		static D3D11_SRV_DIMENSION s_typeLookup[] =
		{
			D3D11_SRV_DIMENSION_TEXTURE1D,		// TextureType_1d
			D3D11_SRV_DIMENSION_TEXTURE2D,		// TextureType_2d
			D3D11_SRV_DIMENSION_TEXTURE3D,		// TextureType_3d
			D3D11_SRV_DIMENSION_TEXTURECUBE,	// TextureType_Cube
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_typeLookup ) == TextureType_Count );

		return s_typeLookup[ type ];
	}

	TextureData::TextureData()
	{
	}

	TextureData::~TextureData()
	{
		TIKI_ASSERT( m_platformData.pResource == nullptr );
		TIKI_ASSERT( m_platformData.pShaderView == nullptr );
	}

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pTextureData /*= nullptr*/ )
	{
		TIKI_ASSERT( m_platformData.pResource == nullptr );
		TIKI_ASSERT( m_platformData.pShaderView == nullptr );
		TIKI_ASSERT( description.type != TextureType_Cube ); // cube textures need to be implemented

		m_description = description;

		D3D11_SUBRESOURCE_DATA initData[ 32u ];
		memory::zero( initData, sizeof( initData ) );

		const D3D11_SUBRESOURCE_DATA* pD3dInitData = nullptr;
		if ( pTextureData != nullptr )
		{
			const uint bytesPerPixel = getBitsPerPixel( (PixelFormat)description.format ) / 8u;

			uint width	= description.width;
			uint height	= description.height;
			uint depth	= TIKI_MAX( description.depth, 1u );
			const uint8* pLevelData	= static_cast< const uint8* >( pTextureData );

			for (uint mipLevel = 0u; mipLevel <= description.mipCount; ++mipLevel)
			{
				const uint rowPitch		= width * bytesPerPixel;
				const uint depthPitch	= rowPitch * height;

				initData[ mipLevel ].pSysMem			= pLevelData;
				initData[ mipLevel ].SysMemPitch		= rowPitch;
				initData[ mipLevel ].SysMemSlicePitch	= depthPitch;

				pLevelData	+= depthPitch * depth;
				width		/= 2u;
				height		/= 2u;
				depth		= TIKI_MAX( depth / 2u, 1u );
			} 

			pD3dInitData = initData;
		}

		const DXGI_FORMAT dxFormat = graphics::getD3dFormat( (PixelFormat)description.format, (TextureFlags)description.flags );
		ID3D11Device* pDevice = graphics::getDevice( graphicsSystem );
		HRESULT result = S_FALSE;
		switch ( m_description.type )
		{
		case TextureType_1d:
			{
				TIKI_DECLARE_STACKANDZERO( TGTexture1DDesc, desc );
				desc.Format				= dxFormat;
				desc.Width				= description.width;
				desc.Usage				= D3D11_USAGE_DEFAULT;
				desc.MipLevels			= description.mipCount + 1u;
				desc.BindFlags			= getD3dFlags( (TextureFlags)description.flags );

				result = pDevice->CreateTexture1D( &desc, pD3dInitData, &m_platformData.pTexture1d );
			}
			break;

		case TextureType_2d:
			{
				TIKI_DECLARE_STACKANDZERO( TGTexture2DDesc, desc );
				desc.Format				= dxFormat;
				desc.Width				= description.width;
				desc.Height				= description.height;
				desc.Usage				= D3D11_USAGE_DEFAULT;
				desc.MipLevels			= description.mipCount + 1u;
				desc.ArraySize			= description.arrayCount;
				desc.SampleDesc.Count	= 1u;
				desc.BindFlags			= getD3dFlags( (TextureFlags)description.flags );

				result = pDevice->CreateTexture2D( &desc, pD3dInitData, &m_platformData.pTexture2d );
			}
			break;

		case TextureType_3d:
			{
				TIKI_DECLARE_STACKANDZERO( TGTexture3DDesc, desc );
				desc.Format				= dxFormat;
				desc.Width				= description.width;
				desc.Height				= description.height;
				desc.Depth				= description.depth;
				desc.Usage				= D3D11_USAGE_DEFAULT;
				desc.MipLevels			= description.mipCount + 1u;
				desc.BindFlags			= getD3dFlags( (TextureFlags)description.flags );

				result = pDevice->CreateTexture3D( &desc, pD3dInitData, &m_platformData.pTexture3d );
			}
			break;

		case TextureType_Cube:
			break;

		}
		if ( FAILED( result ) )
		{
			TIKI_TRACE_ERROR( "[grpahics] Can't create Texture.\n" );
			return false;
		}

		if ( isBitSet( description.flags, TextureFlags_ShaderInput ) )
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format						= dxFormat;
			srvDesc.ViewDimension				= getViewDimentions( (TextureType)description.type );
			srvDesc.Texture2D.MipLevels			= description.mipCount + 1u;
			srvDesc.Texture2D.MostDetailedMip	= 0u;

			if ( FAILED( pDevice->CreateShaderResourceView( m_platformData.pResource, &srvDesc, &m_platformData.pShaderView ) ) )
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