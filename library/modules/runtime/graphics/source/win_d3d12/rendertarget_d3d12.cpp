
#include "tiki/graphics/rendertarget.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/texturedata.hpp"

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
		m_platformData.pColorHeap = nullptr;
		m_platformData.pDepthHeap = nullptr;
	}

	RenderTarget::~RenderTarget()
	{
		TIKI_ASSERT( m_platformData.pColorHeap == nullptr );
		TIKI_ASSERT( m_platformData.pDepthHeap == nullptr );
	}

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		TIKI_ASSERT( colorBufferCount <= GraphicsSystemLimits_RenderTargetSlots );

		m_colorBufferCount	= colorBufferCount;
		m_width				= width;
		m_height			= height;

		ID3D12Device* pDevice = graphics::getDevice( graphicsSystem );

		// create descriptor heaps
		{
			TIKI_DECLARE_STACKANDZERO( D3D12_DESCRIPTOR_HEAP_DESC, heapDesc );

			if( colorBufferCount > 0u )
			{
				heapDesc.NumDescriptors	= (UINT)colorBufferCount;
				heapDesc.Type			= D3D12_RTV_DESCRIPTOR_HEAP;
				if( FAILED( pDevice->CreateDescriptorHeap( &heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_platformData.pColorHeap ) ) )
				{
					dispose( graphicsSystem );
					return false;
				}
			}

			if( pDepthBuffer != nullptr )
			{
				heapDesc.NumDescriptors	= 1u;
				heapDesc.Type			= D3D12_DSV_DESCRIPTOR_HEAP;
				if( FAILED( pDevice->CreateDescriptorHeap( &heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_platformData.pDepthHeap ) ) )
				{
					dispose( graphicsSystem );
					return false;
				}
			}
		}

		TIKI_ASSERT( pColorBuffers != nullptr ||colorBufferCount == 0u );
		for (size_t i = 0u; i < m_colorBufferCount; ++i)
		{
			m_colorBuffers[ i ] = pColorBuffers[ i ];
			TIKI_ASSERT( pColorBuffers[ i ].pDataBuffer != nullptr );

			checkSize( m_width, m_height, pColorBuffers[ i ].pDataBuffer->getWidth(), pColorBuffers[ i ].pDataBuffer->getHeight() );

			TIKI_DECLARE_STACKANDZERO( D3D12_RENDER_TARGET_VIEW_DESC, viewDesc );
			viewDesc.Format						= graphics::getD3dFormat( (PixelFormat)pColorBuffers[ i ].pDataBuffer->getDescription().format, TextureFlags_RenderTarget );
			viewDesc.ViewDimension				= D3D12_RTV_DIMENSION_TEXTURE2D;

			pDevice->CreateRenderTargetView( pColorBuffers[ i ].pDataBuffer->m_platformData.pResource, &viewDesc, m_platformData.pColorHeap->GetCPUDescriptorHandleForHeapStart() );
		}

		for (size_t i = m_colorBufferCount; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].clear();
		}

		if ( pDepthBuffer != nullptr )
		{
			m_depthBuffer = *pDepthBuffer;
			TIKI_ASSERT( pDepthBuffer->pDataBuffer != nullptr );

			checkSize( m_width, m_height, pDepthBuffer->pDataBuffer->getWidth(), pDepthBuffer->pDataBuffer->getHeight() );

			TIKI_DECLARE_STACKANDZERO( D3D12_DEPTH_STENCIL_VIEW_DESC, viewDesc );
			viewDesc.Format				= graphics::getD3dFormat( (PixelFormat)pDepthBuffer->pDataBuffer->getDescription( ).format, TextureFlags_DepthStencil );
			viewDesc.ViewDimension		= D3D12_DSV_DIMENSION_TEXTURE2D;

			pDevice->CreateDepthStencilView( pDepthBuffer->pDataBuffer->m_platformData.pResource, &viewDesc, m_platformData.pDepthHeap->GetCPUDescriptorHandleForHeapStart() );
		}
		else
		{
			m_depthBuffer.format		= PixelFormat_Invalid;
			m_depthBuffer.pDataBuffer	= nullptr;

			m_platformData.pDepthHeap	= nullptr;
		}

		return true;
	}

	void RenderTarget::dispose( GraphicsSystem& graphicsSystem )
	{
		graphics::safeRelease( &m_platformData.pColorHeap );
		graphics::safeRelease( &m_platformData.pDepthHeap );

		for( size_t i = 0u; i < TIKI_COUNT( m_colorBuffers ); ++i )
		{
			m_colorBuffers[ i ].clear();
		}

		m_depthBuffer.clear();

		m_colorBufferCount	= 0u;
		m_width				= 0u;
		m_height			= 0u;
	}
}
