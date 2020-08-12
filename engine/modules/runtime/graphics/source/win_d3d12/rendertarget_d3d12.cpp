
#include "tiki/graphics/rendertarget.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/texturedata.hpp"

#include "descriptorpool_d3d12.hpp"
#include "graphicssystem_internal_d3d12.hpp"

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
		TIKI_ASSERT( TIKI_COUNT( m_colorBuffers ) == TIKI_COUNT( m_platformData.colorHandles ) );
	}

	RenderTarget::~RenderTarget()
	{
	}

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		TIKI_ASSERT( colorBufferCount <= GraphicsSystemLimits_RenderTargetSlots );

		m_colorBufferCount	= colorBufferCount;
		m_width				= width;
		m_height			= height;

		ID3D12Device* pDevice = GraphicsSystemPlatform::getDevice( graphicsSystem );

		// create descriptor heaps
		{
			TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, heapDesc );

			for( size_t i = 0u; i < colorBufferCount; ++i )
			{
			}

			if( pDepthBuffer != nullptr )
			{
			}
		}

		TIKI_ASSERT( pColorBuffers != nullptr || colorBufferCount == 0u );

		DescriptorPoolD3d12& renderTargetPool = GraphicsSystemPlatform::getRenderTargetPool( graphicsSystem );
		for (size_t i = 0u; i < m_colorBufferCount; ++i)
		{
			m_colorBuffers[ i ] = pColorBuffers[ i ];
			TIKI_ASSERT( pColorBuffers[ i ].pDataBuffer != nullptr );

			m_platformData.colorHandles[ i ] = renderTargetPool.allocateDescriptor();
			if( m_platformData.colorHandles[ i ] == InvalidDescriptorHandle )
			{
				dispose( graphicsSystem );
				return false;
			}

			checkSize( m_width, m_height, pColorBuffers[ i ].pDataBuffer->getWidth(), pColorBuffers[ i ].pDataBuffer->getHeight() );

			TIKI_DECLARE_STACKANDZERO( D3D12_RENDER_TARGET_VIEW_DESC, viewDesc );
			viewDesc.Format						= GraphicsSystemPlatform::getD3dPixelFormat( (PixelFormat)pColorBuffers[ i ].pDataBuffer->getDescription().format, false );
			viewDesc.ViewDimension				= D3D12_RTV_DIMENSION_TEXTURE2D;

			pDevice->CreateRenderTargetView( pColorBuffers[ i ].pDataBuffer->m_platformData.pResource, &viewDesc, renderTargetPool.getCpuHandle( m_platformData.colorHandles[ i ] ) );
		}

		for (size_t i = m_colorBufferCount; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].clear();
		}

		if ( pDepthBuffer != nullptr )
		{
			m_depthBuffer = *pDepthBuffer;
			TIKI_ASSERT( pDepthBuffer->pDataBuffer != nullptr );

			DescriptorPoolD3d12& depthStencilPool = GraphicsSystemPlatform::getDepthStencilPool( graphicsSystem );
			m_platformData.depthHandle = depthStencilPool.allocateDescriptor();
			if( m_platformData.depthHandle == InvalidDescriptorHandle )
			{
				dispose( graphicsSystem );
				return false;
			}

			checkSize( m_width, m_height, pDepthBuffer->pDataBuffer->getWidth(), pDepthBuffer->pDataBuffer->getHeight() );

			TIKI_DECLARE_STACKANDZERO( D3D12_DEPTH_STENCIL_VIEW_DESC, viewDesc );
			viewDesc.Format				= GraphicsSystemPlatform::getD3dPixelFormat( (PixelFormat)pDepthBuffer->pDataBuffer->getDescription( ).format, false );
			viewDesc.ViewDimension		= D3D12_DSV_DIMENSION_TEXTURE2D;

			pDevice->CreateDepthStencilView( pDepthBuffer->pDataBuffer->m_platformData.pResource, &viewDesc, depthStencilPool.getCpuHandle( m_platformData.depthHandle ) );
		}
		else
		{
			m_depthBuffer.format		= PixelFormat_Invalid;
			m_depthBuffer.pDataBuffer	= nullptr;

			m_platformData.depthHandle	= InvalidDescriptorHandle;
		}

		return true;
	}

	void RenderTarget::dispose( GraphicsSystem& graphicsSystem )
	{
		DescriptorPoolD3d12& renderTargetPool = GraphicsSystemPlatform::getRenderTargetPool( graphicsSystem );
		for( size_t i = 0u; i < TIKI_COUNT( m_colorBuffers ); ++i )
		{
			if( m_platformData.colorHandles[ i ] != InvalidDescriptorHandle )
			{
				renderTargetPool.freeDescriptor( m_platformData.colorHandles[ i ] );
				m_platformData.colorHandles[ i ] = InvalidDescriptorHandle;
			}

			m_colorBuffers[ i ].clear();
		}

		if( m_platformData.depthHandle != InvalidDescriptorHandle )
		{
			GraphicsSystemPlatform::getDepthStencilPool( graphicsSystem ).freeDescriptor( m_platformData.depthHandle );
			m_platformData.depthHandle = InvalidDescriptorHandle;
		}

		m_depthBuffer.clear();

		m_colorBufferCount	= 0u;
		m_width				= 0u;
		m_height			= 0u;
	}
}
