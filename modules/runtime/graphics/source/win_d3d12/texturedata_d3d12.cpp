
#include "tiki/graphics/texturedata.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"

#include "graphicssystem_internal_d3d12.hpp"
#include "uploadheap_d3d12.hpp"
#include "descriptorpool_d3d12.hpp"

namespace tiki
{
	static D3D12_RESOURCE_FLAGS getD3dFlags( TextureFlags flags )
	{
		D3D12_RESOURCE_FLAGS result = D3D12_RESOURCE_FLAG_NONE;

		if( isBitSet( flags, TextureFlags_RenderTarget ) )
		{
			result |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}

		if( isBitSet( flags, TextureFlags_DepthStencil ) )
		{
			result |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}

		if( !isBitSet( flags, TextureFlags_ShaderInput ) )
		{
			result |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
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

	bool TextureData::create( GraphicsSystem& graphicsSystem, const TextureDescription& description, const void* pTextureData /* = nullptr */, const char* pDebugName /* = nullptr */ )
	{
		TIKI_ASSERT( m_platformData.pResource == nullptr );
		TIKI_ASSERT( description.type != TextureType_Cube ); // cube textures need to be implemented

		m_description = description;

		ID3D12Device* pDevice = GraphicsSystemPlatform::getDevice( graphicsSystem );

		const PixelFormat format	= (PixelFormat)description.format;
		const TextureFlags flags	= (TextureFlags)description.flags;
		const DXGI_FORMAT dxFormat	= GraphicsSystemPlatform::getD3dPixelFormat( format, isBitSet( flags, TextureFlags_ShaderInput ) );

		D3D12_RESOURCE_DESC resourceDesc;
		switch( description.type )
		{
		case TextureType_1d:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex1D(
				dxFormat,
				description.width,
				description.arrayCount,
				description.mipCount,
				getD3dFlags( flags )
			);
			break;

		case TextureType_2d:
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
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
			resourceDesc = CD3DX12_RESOURCE_DESC::Tex3D(
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

		D3D12_CLEAR_VALUE* pClearValue = nullptr;

		TIKI_DECLARE_STACKANDZERO( D3D12_CLEAR_VALUE, clearValue );
		clearValue.Format = dxFormat;
		if( isBitSet( m_description.flags, TextureFlags_DepthStencil ) )
		{
			clearValue.DepthStencil.Depth = 1.0f;
			pClearValue = &clearValue;
		}
		else if( isBitSet( m_description.flags, TextureFlags_RenderTarget ) )
		{
			pClearValue = &clearValue;
		}

		HRESULT result = pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT ),
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			pClearValue,
			IID_PPV_ARGS( &m_platformData.pResource )
		);

		if( FAILED( result ) )
		{
			dispose( graphicsSystem );
			return false;
		}
		TIKI_SET_DX_OBJECT_NAME( m_platformData.pResource, pDebugName );

		if( pTextureData != nullptr )
		{
			UploadHeapD3d12& uploadHeap = GraphicsSystemPlatform::getUploadHeap( graphicsSystem );

			const uint bytesPerPixel = getBitsPerPixel( (PixelFormat)description.format ) / 8u;

			uint width	= description.width;
			uint height	= description.height;
			uint depth	= TIKI_MAX( description.depth, 1u );

			const uint subResourceCount		= description.arrayCount * description.mipCount;
			const uint requiredUploadSize	= GetRequiredIntermediateSize( m_platformData.pResource, 0u, (UINT)subResourceCount );

			UploadHeapAllocationResult allocation;
			if ( !uploadHeap.allocateData( allocation, requiredUploadSize, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT) )
			{
				dispose( graphicsSystem );
				return false;
			}

			D3D12_SUBRESOURCE_DATA subResources[ 32u ];
			for( uint mipLevel = 0u; mipLevel < description.mipCount; ++mipLevel )
			{
				const uint rowPitch		= width * bytesPerPixel;
				const uint depthPitch	= rowPitch * height;
				
				//memory::copy( allocation.pData, pTextureData, depthPitch );

				//TIKI_DECLARE_STACKANDZERO( D3D12_PLACED_PITCHED_SUBRESOURCE_DESC, viewDesc );
				//viewDesc.Offset				= allocation.offset;
				//viewDesc.Placement.Format	= dxFormat;
				//viewDesc.Placement.Width	= (UINT)width;
				//viewDesc.Placement.Height	= (UINT)height;
				//viewDesc.Placement.Depth	= (UINT)depth;
				//viewDesc.Placement.RowPitch	= (UINT)rowPitch;

				subResources[ mipLevel ].pData		= pTextureData;
				subResources[ mipLevel ].RowPitch	= rowPitch;
				subResources[ mipLevel ].SlicePitch	= depthPitch;

				width		= TIKI_MAX( width / 2u, 1u );
				height		= TIKI_MAX( height / 2u, 1u );
				depth		= TIKI_MAX( depth / 2u, 1u );
			}

			ID3D12GraphicsCommandList* pCommandList = GraphicsSystemPlatform::getCommandList( graphicsSystem );
			GraphicsSystemPlatform::setResourceBarrier( pCommandList, m_platformData.pResource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST );
			UpdateSubresources<32>( pCommandList, m_platformData.pResource, uploadHeap.getBuffer(), allocation.offset, 0u, (UINT)subResourceCount, subResources );
			GraphicsSystemPlatform::setResourceBarrier( pCommandList, m_platformData.pResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE );
		}
	
		if( isBitSet( description.flags, TextureFlags_ShaderInput ) )
		{
			DescriptorPoolD3d12& shaderResourcePool = GraphicsSystemPlatform::getShaderResourcePool( graphicsSystem );
			m_platformData.shaderViewHandle = shaderResourcePool.allocateDescriptor();
			if( m_platformData.shaderViewHandle == InvalidDescriptorHandle )
			{
				dispose( graphicsSystem );
				return false;
			}

			TIKI_DECLARE_STACKANDZERO( D3D12_SHADER_RESOURCE_VIEW_DESC, viewDesc );
			viewDesc.ViewDimension				= getD3dViewDimentions( (TextureType)description.type );
			viewDesc.Format						= dxFormat;
			viewDesc.Shader4ComponentMapping	= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			switch( m_description.type )
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

			pDevice->CreateShaderResourceView( m_platformData.pResource, &viewDesc, shaderResourcePool.getCpuHandle( m_platformData.shaderViewHandle ) );
		}

		return true;
	}

	void TextureData::dispose( GraphicsSystem& graphicsSystem )
	{
		GraphicsSystemPlatform::safeRelease( &m_platformData.pResource );

		if( m_platformData.shaderViewHandle != InvalidDescriptorHandle )
		{
			GraphicsSystemPlatform::getShaderResourcePool( graphicsSystem ).freeDescriptor( m_platformData.shaderViewHandle );
			m_platformData.shaderViewHandle = InvalidDescriptorHandle;
		}
	}
}