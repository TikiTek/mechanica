
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
		TIKI_ASSERT( description.type == TextureType_2d ); // currently only 2D textures supported

		m_description = description;

		TIKI_DECLARE_STACKANDZERO( TGTexture2DDesc, desc );
		desc.Format				= graphics::getD3dFormat( (PixelFormat)description.format, (TextureFlags)description.flags );
		desc.Width				= description.width;
		desc.Height				= description.height;		
		desc.Usage				= D3D11_USAGE_DEFAULT;
		desc.MipLevels			= description.mipCount + 1u;
		desc.ArraySize			= description.arrayCount;
		desc.SampleDesc.Count	= 1u;
		desc.BindFlags			= getD3dFlags( (TextureFlags)description.flags );

		D3D11_SUBRESOURCE_DATA initData[ 32u ];
		memory::zero( initData, sizeof( initData ) );

		const D3D11_SUBRESOURCE_DATA* pD3dInitData = nullptr;
		if ( pTextureData != nullptr )
		{
			const uint bytesPerPixel = getBitsPerPixel( (PixelFormat)description.format ) / 8u;

			uint width	= description.width;
			uint height	= description.height;
			const uint8* pLevelData	= static_cast< const uint8* >( pTextureData );

			for (uint mipLevel = 0u; mipLevel <= description.mipCount; ++mipLevel)
			{
				const uint rowPitch		= width * bytesPerPixel;
				const uint depthPitch	= rowPitch * height;

				initData[ mipLevel ].pSysMem			= pLevelData;
				initData[ mipLevel ].SysMemPitch		= rowPitch;
				initData[ mipLevel ].SysMemSlicePitch	= depthPitch;

				width		/= 2u;
				height		/= 2u;
				pLevelData	+= depthPitch;
			} 

			pD3dInitData = initData;
		}

		ID3D11Device* pDevice = graphics::getDevice( graphicsSystem );
		if ( FAILED( pDevice->CreateTexture2D( &desc, pD3dInitData, &m_platformData.pTexture2d ) ) )
		{
			TIKI_TRACE_ERROR( "[grpahics] Can't create Texture.\n" );
			return false;
		}

		if ( isBitSet( description.flags, TextureFlags_ShaderInput ) )
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format						= desc.Format;
			srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels			= desc.MipLevels;
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