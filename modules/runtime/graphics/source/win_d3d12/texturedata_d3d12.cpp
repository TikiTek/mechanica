
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	DXGI_FORMAT graphics::getD3dFormat( PixelFormat pixelFormat, TextureFlags flags )
	{
		TIKI_ASSERT( pixelFormat < PixelFormat_Count );

		static DXGI_FORMAT s_formatLookup[] =
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

	static D3D12_RESOURCE_MISC_FLAG getD3dFlags( TextureFlags flags )
	{
		D3D12_RESOURCE_MISC_FLAG result = D3D12_RESOURCE_MISC_NONE;

		if ( isBitSet( flags, TextureFlags_RenderTarget ) )
		{
			result |= D3D12_RESOURCE_MISC_ALLOW_RENDER_TARGET;
		}

		if ( isBitSet( flags, TextureFlags_DepthStencil ) )
		{
			result |= D3D12_RESOURCE_MISC_ALLOW_DEPTH_STENCIL;
		}

		if ( !isBitSet( flags, TextureFlags_ShaderInput ) )
		{
			result |= D3D12_RESOURCE_MISC_DENY_SHADER_RESOURCE;
		}

		return result;
	}

	static D3D12_SRV_DIMENSION getD3dViewDimentions( TextureType type )
	{
		TIKI_ASSERT( type < TextureType_Count );

		static D3D12_SRV_DIMENSION s_typeLookup[] =
		{
			D3D12_SRV_DIMENSION_TEXTURE1D,		// TextureType_1d
			D3D12_SRV_DIMENSION_TEXTURE2D,		// TextureType_2d
			D3D12_SRV_DIMENSION_TEXTURE3D,		// TextureType_3d
			D3D12_SRV_DIMENSION_TEXTURECUBE,	// TextureType_Cube
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
	}

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pTextureData /*= nullptr*/ )
	{
		TIKI_ASSERT( m_platformData.pResource == nullptr );
		TIKI_ASSERT( description.type != TextureType_Cube ); // cube textures need to be implemented

		m_description = description;

		ID3D12Device* pDevice = graphics::getDevice( graphicsSystem );

		const PixelFormat format	= (PixelFormat)description.format;
		const TextureFlags flags	= (TextureFlags)description.flags;
		const DXGI_FORMAT dxFormat	= graphics::getD3dFormat( format, flags );

		D3D12_RESOURCE_DESC resourceDesc;
		switch( description.type )
		{
		case TextureType_1d:
			resourceDesc = CD3D12_RESOURCE_DESC::Tex1D(
				dxFormat,
				description.width,
				description.arrayCount,
				description.mipCount,
				getD3dFlags( flags )
			);
			break;

		case TextureType_2d:
			resourceDesc = CD3D12_RESOURCE_DESC::Tex2D(
				dxFormat,
				description.width,
				description.height,
				description.arrayCount,
				description.mipCount,
				1u,
				0u,
				getD3dFlags( flags )
			);
			break;

		case TextureType_3d:
			resourceDesc = CD3D12_RESOURCE_DESC::Tex3D(
				dxFormat,
				description.width,
				description.height,
				description.depth,
				description.mipCount,
				getD3dFlags( flags )
			);
			break;

		case TextureType_Cube:
			TIKI_TRACE_ERROR( "[TextureData::create] not implemented\n" );
			break;
		}

		HRESULT result = pDevice->CreateCommittedResource(
			&CD3D12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT ),
			D3D12_HEAP_MISC_NONE,
			&resourceDesc,
			D3D12_RESOURCE_USAGE_INITIAL,
			nullptr,
			IID_PPV_ARGS( &m_platformData.pResource )
		);

		if( FAILED( result ) )
		{
			dispose( graphicsSystem );
			return false;
		}

		if( pTextureData != nullptr )
		{

			UploadHeapD3d12& uploadHeap = graphics::getUploadHeap( graphicsSystem );
			ID3D12GraphicsCommandList* pCommandList = graphics::getCommandList( graphicsSystem );

			graphics::setResourceBarrier( pCommandList, m_platformData.pResource, D3D12_RESOURCE_USAGE_INITIAL, D3D12_RESOURCE_USAGE_COPY_DEST );

			const uint bytesPerPixel = getBitsPerPixel( (PixelFormat)description.format ) / 8u;

			uint width	= description.width;
			uint height	= description.height;
			uint depth	= TIKI_MAX( description.depth, 1u );

			for( uint mipLevel = 0u; mipLevel < description.mipCount; ++mipLevel )
			{
				const uint rowPitch		= width * bytesPerPixel;
				const uint depthPitch	= rowPitch * height;

				UploadHeapAllocationResult allocation;
				if( !uploadHeap.allocateData( allocation, depthPitch, D3D12_TEXTURE_OFFSET_ALIGNMENT ) )
				{
					dispose( graphicsSystem );
					return false;
				}

				memory::copy( allocation.pData, pTextureData, depthPitch );

				TIKI_DECLARE_STACKANDZERO( D3D12_PLACED_PITCHED_SUBRESOURCE_DESC, viewDesc );
				viewDesc.Offset				= allocation.offset;
				viewDesc.Placement.Format	= dxFormat;
				viewDesc.Placement.Width	= (UINT)width;
				viewDesc.Placement.Height	= (UINT)height;
				viewDesc.Placement.Depth	= (UINT)depth;
				viewDesc.Placement.RowPitch	= (UINT)rowPitch;

				TIKI_DECLARE_STACKANDZERO( D3D12_SELECT_SUBRESOURCE, subResource );
				subResource.Subresource = (UINT)mipLevel;

				pCommandList->copy(
					m_platformData.pResource,
					D3D12_SUBRESOURCE_VIEW_SELECT_SUBRESOURCE,
					&subResource,
					0u,
					0u,
					0u,
					uploadHeap.getBuffer(),
					D3D12_SUBRESOURCE_VIEW_PLACED_PITCHED_SUBRESOURCE,
					&viewDesc,
					nullptr,
					D3D12_COPY_NONE
				);
				
				width		= TIKI_MAX( width / 2u, 1u );
				height		= TIKI_MAX( height / 2u, 1u );
				depth		= TIKI_MAX( depth / 2u, 1u );
			}
		}
		
		if( isBitSet( description.flags, TextureFlags_ShaderInput ) )
		{
			// create descriptor heap
			{
				TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, heapDesc );
				heapDesc.NumDescriptors = 1u;
				heapDesc.Type			= D3D12_CBV_SRV_UAV_DESCRIPTOR_HEAP;
				heapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_SHADER_VISIBLE;

				if( FAILED( pDevice->CreateDescriptorHeap( &heapDesc, __uuidof( ID3D12DescriptorHeap ), (void**)&m_platformData.pDescriptorHeap ) ) )
				{
					dispose( graphicsSystem );
					return false;
				}
			}

			// create shader resource view
			{
				TIKI_DECLARE_STACKANDZERO( D3D12_SHADER_RESOURCE_VIEW_DESC, viewDesc );
				viewDesc.ViewDimension = getD3dViewDimentions( (TextureType)description.type );
				viewDesc.Format = dxFormat;

				switch (m_description.type)
				{
				case TextureType_1d:
					viewDesc.Texture1D.MipLevels = description.mipCount;
					break;

				case TextureType_2d:
					viewDesc.Texture2D.MipLevels = description.mipCount;
					break;

				case TextureType_3d:
					viewDesc.Texture3D.MipLevels = description.mipCount;
					break;

				case TextureType_Cube:
					viewDesc.TextureCube.MipLevels = description.mipCount;
					break;

				}

				pDevice->CreateShaderResourceView( m_platformData.pResource, &viewDesc, m_platformData.pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
			}
		}

		return true;
	}

	void TextureData::dispose( GraphicsSystem& graphicsSystem )
	{
		graphics::safeRelease( &m_platformData.pDescriptorHeap );
		graphics::safeRelease( &m_platformData.pResource );
	}
}