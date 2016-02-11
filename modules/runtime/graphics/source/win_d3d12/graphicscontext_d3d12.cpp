
#include "tiki/graphics/graphicscontext.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/crc32.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/graphics/blendstate.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/depthstencilstate.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/indexbuffer.hpp"
#include "tiki/graphics/rasterizerstate.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/shadertype.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexinputbinding.hpp"
#include "tiki/graphics/viewport.hpp"
#include "tiki/math/rectangle.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	struct ViewHandleDataD3d12
	{
		ViewHandleDataD3d12()
		{
		}

		ViewHandleDataD3d12( UINT _slot, D3D12_CPU_DESCRIPTOR_HANDLE _handle )
		{
			slot	= _slot;
			handle	= _handle;
		}

		UINT						slot;
		D3D12_CPU_DESCRIPTOR_HANDLE	handle;
	};

	GraphicsContext::GraphicsContext()
	{
		m_pGraphicsSystem = nullptr;
	}

	GraphicsContext::~GraphicsContext()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_platformData.pCommandList == nullptr );
	}

	bool GraphicsContext::create( GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_platformData.pCommandList == nullptr );

		m_pGraphicsSystem				= &graphicsSystem;
		m_platformData.pDevice			= GraphicsSystemPlatform::getDevice( graphicsSystem );
		m_platformData.pCommandList		= GraphicsSystemPlatform::getCommandList( graphicsSystem );
		m_platformData.pRootSignature	= GraphicsSystemPlatform::getRootSignature( graphicsSystem );

		m_pRenderTarget = nullptr;

		for (uint i = 0u; i < TIKI_COUNT( m_apRenderPassesStack ); ++i)
		{
			m_apRenderPassesStack[ i ] = nullptr;
		}
		m_currentRenderPassDepth = 0u;

		m_immediateVertexData.create( graphicsSystem, MaxImmediateGeometrySize / 4u, 4u, true );
		m_immediateVertexCount	= 0u;
		m_immediateVertexStride	= 0u;

		invalidateState();

		return true;
	}

	void GraphicsContext::dispose( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem				= nullptr;
		m_platformData.pCommandList		= nullptr;

		m_immediateVertexData.dispose( graphicsSystem );
	}

	void GraphicsContext::clear( const RenderTarget& renderTarget, Color color /* = TIKI_COLOR_BLACK */, float depthValue /* = 1.0f */, uint8 stencilValue /* = 0u */, ClearMask clearMask /* = ClearMask_All */ )
	{
		D3D12_CLEAR_FLAGS depthClearFlags = (D3D12_CLEAR_FLAGS)0u;
		if ( isBitSet( clearMask, ClearMask_Depth ) )
		{
			depthClearFlags |= D3D12_CLEAR_FLAG_DEPTH;
		}

		if ( isBitSet( clearMask, ClearMask_Stencil ) )
		{
			depthClearFlags |= D3D12_CLEAR_FLAG_STENCIL;
		}

		if ( depthClearFlags != 0u && renderTarget.m_platformData.depthHandle != InvalidDescriptorHandle )
		{
			DescriptorPoolD3d12& depthStencilPool = GraphicsSystemPlatform::getDepthStencilPool( *m_pGraphicsSystem );
			m_platformData.pCommandList->ClearDepthStencilView( depthStencilPool.getCpuHandle( renderTarget.m_platformData.depthHandle ), depthClearFlags, depthValue, stencilValue, 0u, nullptr );
		}

		DescriptorPoolD3d12& renderTargetPool = GraphicsSystemPlatform::getRenderTargetPool( *m_pGraphicsSystem );

		float4 floatColor;
		color::toFloat4( floatColor, color );
		for( size_t i = 0u; i < renderTarget.m_colorBufferCount; ++i )
		{
			TIKI_ASSERT( renderTarget.m_platformData.colorHandles[ i ] != InvalidDescriptorHandle );
			m_platformData.pCommandList->ClearRenderTargetView( renderTargetPool.getCpuHandle( renderTarget.m_platformData.colorHandles[ i ] ), &floatColor.x, 0u, nullptr );
		}
	}

	void GraphicsContext::copyTextureData( const TextureData& sourceData, const TextureData& targetData )
	{
		m_platformData.pCommandList->CopyResource(
			targetData.m_platformData.pResource,
			sourceData.m_platformData.pResource
		);
	}

	void GraphicsContext::beginRenderPass( const RenderTarget& renderTarget, const Viewport* pViewport /* = nullptr*/ )
	{
		TIKI_ASSERT( m_currentRenderPassDepth < GraphicsSystemLimits_RenderPassStackDepth );

		m_apRenderPassesStack[ m_currentRenderPassDepth ] = &renderTarget;
		m_currentRenderPassDepth++;
		
		DescriptorPoolD3d12& renderTargetPool = GraphicsSystemPlatform::getRenderTargetPool( *m_pGraphicsSystem );

		D3D12_CPU_DESCRIPTOR_HANDLE aD3dHandles[ GraphicsSystemLimits_MaxRenderTargetViews ];
		for( size_t i = 0u; i < renderTarget.m_colorBufferCount; ++i )
		{
			aD3dHandles[ i ] = renderTargetPool.getCpuHandle( renderTarget.m_platformData.colorHandles[ i ] );
		}

		D3D12_CPU_DESCRIPTOR_HANDLE depthHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE* pDepthHandle = nullptr;
		if( renderTarget.m_platformData.depthHandle != InvalidDescriptorHandle )
		{
			depthHandle = GraphicsSystemPlatform::getDepthStencilPool( *m_pGraphicsSystem ).getCpuHandle( renderTarget.m_platformData.depthHandle );
			pDepthHandle = &depthHandle;
		}

		m_platformData.pCommandList->OMSetRenderTargets(
			(UINT)renderTarget.m_colorBufferCount,
			(renderTarget.m_colorBufferCount == 0u ? nullptr : aD3dHandles),
			FALSE,
			pDepthHandle
		);

		D3D12_VIEWPORT viewPort;
		D3D12_RECT scissorRect;
		if ( pViewport == nullptr )
		{
			viewPort.TopLeftX	= 0.0f;
			viewPort.TopLeftY	= 0.0f;
			viewPort.Width		= (float)renderTarget.getWidth();
			viewPort.Height		= (float)renderTarget.getHeight();
			viewPort.MinDepth	= 0.0f;
			viewPort.MaxDepth	= 1.0f;

			scissorRect.left	= 0u;
			scissorRect.top		= 0u;
			scissorRect.right	= LONG( renderTarget.getWidth() );
			scissorRect.bottom	= LONG( renderTarget.getHeight() );
		}
		else
		{
			viewPort.TopLeftX	= pViewport->x;
			viewPort.TopLeftY	= pViewport->y;
			viewPort.Width		= pViewport->width;
			viewPort.Height		= pViewport->height;
			viewPort.MinDepth	= pViewport->minDepth;
			viewPort.MaxDepth	= pViewport->maxDepth;

			scissorRect.left	= LONG( pViewport->x );
			scissorRect.top		= LONG( pViewport->y );
			scissorRect.right	= LONG( pViewport->x + pViewport->width );
			scissorRect.bottom	= LONG( pViewport->y + pViewport->height );
		}
		m_platformData.pCommandList->RSSetViewports( 1u, &viewPort );
		m_platformData.pCommandList->RSSetScissorRects( 1u, &scissorRect );

		m_pRenderTarget = &renderTarget;

		invalidateState();
	}

	void GraphicsContext::endRenderPass()
	{
		TIKI_ASSERT( m_currentRenderPassDepth != 0u );

		m_currentRenderPassDepth--;
		m_apRenderPassesStack[ m_currentRenderPassDepth ] = nullptr;

		if ( m_currentRenderPassDepth != 0u )
		{
			const RenderTarget& renderTarget = *m_apRenderPassesStack[ m_currentRenderPassDepth - 1u ];

			DescriptorPoolD3d12& renderTargetPool = GraphicsSystemPlatform::getRenderTargetPool( *m_pGraphicsSystem );

			D3D12_CPU_DESCRIPTOR_HANDLE aD3dHandles[ GraphicsSystemLimits_MaxRenderTargetViews ];
			for( size_t i = 0u; i < renderTarget.m_colorBufferCount; ++i )
			{
				aD3dHandles[ i ] = renderTargetPool.getCpuHandle( renderTarget.m_platformData.colorHandles[ i ] );
			}

			D3D12_CPU_DESCRIPTOR_HANDLE depthHandle;
			D3D12_CPU_DESCRIPTOR_HANDLE* pDepthHandle = nullptr;
			if( renderTarget.m_platformData.depthHandle != InvalidDescriptorHandle )
			{
				depthHandle = GraphicsSystemPlatform::getDepthStencilPool( *m_pGraphicsSystem ).getCpuHandle( renderTarget.m_platformData.depthHandle );
				pDepthHandle = &depthHandle;
			}

			m_platformData.pCommandList->OMSetRenderTargets(
				(UINT)renderTarget.m_colorBufferCount,
				(renderTarget.m_colorBufferCount == 0u ? nullptr : aD3dHandles),
				FALSE,
				pDepthHandle
			);

			m_pRenderTarget = &renderTarget;
		}
		else
		{
			m_pRenderTarget = nullptr;
			m_platformData.pCommandList->OMSetRenderTargets( 0u, nullptr, FALSE, nullptr );
		}

		invalidateState();
	}


	void GraphicsContext::setBlendState( const BlendState* pBlendState )
	{
		TIKI_ASSERT( pBlendState != nullptr );

		if ( m_pBlendState != pBlendState )
		{
			m_pBlendState = pBlendState;
		}		
	}

	void GraphicsContext::setDepthStencilState( const DepthStencilState* pDepthStencilState )
	{
		TIKI_ASSERT( pDepthStencilState != nullptr );
		m_pDepthStencilState = pDepthStencilState;
	}

	void GraphicsContext::setRasterizerState( const RasterizerState* pRasterizerState )
	{
		TIKI_ASSERT( pRasterizerState != nullptr );
		m_pRasterizerState = pRasterizerState;
	}

	void GraphicsContext::setPrimitiveTopology( PrimitiveTopology topology )
	{
		m_primitiveTopology = topology;
	}

	void GraphicsContext::setVertexShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_VertexShader );
		m_pVertexShader = pShader;
	}

	void GraphicsContext::setVertexInputBinding( const VertexInputBinding* pVertexInputBinding )
	{
		TIKI_ASSERT( pVertexInputBinding != nullptr );
		m_pVertexInputBinding = pVertexInputBinding;
	}

	void GraphicsContext::setVertexShaderSamplerState( uint slot, const SamplerState* pSampler )
	{
		if ( m_apVertexSamplerStates[ slot ] != pSampler )
		{
			m_apVertexSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setVertexShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_apVertexTextures[ slot ] != pTextureData )
		{
			m_apVertexTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setVertexShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_apVertexConstants[ slot ] != &buffer )
		{
			m_apVertexConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setPixelShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_PixelShader );
		m_pPixelShader = pShader;
	}

	void GraphicsContext::setPixelShaderSamplerState( uint slot, const SamplerState* pSampler )
	{
		if ( m_apPixelSamplerStates[ slot ] != pSampler )
		{
			m_apPixelSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setPixelShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_apPixelTextures[ slot ] != pTextureData )
		{
			m_apPixelTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setPixelShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_apPixelConstants[ slot ] != &buffer)
		{
			m_apPixelConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setIndexBuffer( const IndexBuffer& indexBuffer )
	{
		D3D12_INDEX_BUFFER_VIEW bufferView;
		bufferView.BufferLocation	= indexBuffer.m_pBuffer->GetGPUVirtualAddress();
		bufferView.SizeInBytes		= UINT( indexBuffer.getCount() * indexBuffer.getIndexType() );
		bufferView.Format			= GraphicsSystemPlatform::getD3dIndexFormat( indexBuffer.getIndexType() );

		m_platformData.pCommandList->IASetIndexBuffer( &bufferView );
	}

	void GraphicsContext::setVertexBuffer( uint slot, const VertexBuffer& vertexBuffer )
	{
		D3D12_VERTEX_BUFFER_VIEW bufferView;
		bufferView.BufferLocation	= vertexBuffer.m_pBuffer->GetGPUVirtualAddress();
		bufferView.SizeInBytes		= UINT( vertexBuffer.getCount() * vertexBuffer.getStride() );
		bufferView.StrideInBytes	= UINT( vertexBuffer.getStride() );

		m_platformData.pCommandList->IASetVertexBuffers( (UINT)slot, 1u, &bufferView );
	}

	void* GraphicsContext::beginImmediateGeometry( uint vertexStride, uint vertexCount )
	{
		TIKI_ASSERT( vertexStride * vertexCount <= MaxImmediateGeometrySize );

		m_immediateVertexStride	= vertexStride;
		m_immediateVertexCount	= vertexCount;

		return mapBuffer( m_immediateVertexData );
	}

	void GraphicsContext::endImmediateGeometry()
	{
		unmapBuffer( m_immediateVertexData );
		TIKI_ASSERT( validateDrawCall() );

		D3D12_VERTEX_BUFFER_VIEW bufferView;
		bufferView.BufferLocation	= m_immediateVertexData.m_pBuffer->GetGPUVirtualAddress();
		bufferView.SizeInBytes		= UINT( m_immediateVertexCount * m_immediateVertexStride );
		bufferView.StrideInBytes	= UINT( m_immediateVertexStride );
		m_platformData.pCommandList->IASetVertexBuffers( 0u, 1u, &bufferView );

		prepareDrawCall();
		m_platformData.pCommandList->DrawInstanced( (UINT)m_immediateVertexCount, 1u, 0u, 0u );		
	}

	void GraphicsContext::drawGeometry( uint vertexCount, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );

		prepareDrawCall();
		m_platformData.pCommandList->DrawInstanced( (UINT)vertexCount, 1u, (UINT)baseVertexOffset, 0u );
	}

	void GraphicsContext::drawIndexedGeometry( uint indexCount, uint baseIndexOffset /*= 0u*/, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );

		prepareDrawCall();
		m_platformData.pCommandList->DrawIndexedInstanced( (UINT)indexCount, 1u, (UINT)baseIndexOffset, (UINT)baseVertexOffset, 0u );
	}

	void* GraphicsContext::mapBuffer( const BaseBuffer& buffer )
	{
		TIKI_ASSERT( buffer.m_pBuffer != nullptr );

		void* pData;
		buffer.m_pBuffer->Map( 0u, nullptr, &pData );

		return pData;
	}

	void GraphicsContext::unmapBuffer( const BaseBuffer& buffer )
	{
		buffer.m_pBuffer->Unmap( 0u, nullptr );
	}

	const RenderTarget& GraphicsContext::getBackBuffer() const
	{
		return m_pGraphicsSystem->getBackBuffer();
	}

	void GraphicsContext::prepareDrawCall()
	{
		// pipeline state
		{
			const uint64 aRelevantStates[] =
			{
				(uint64)m_pBlendState,
				(uint64)m_pDepthStencilState,
				(uint64)m_pVertexInputBinding,
				(uint64)m_pRenderTarget,
				(uint64)m_primitiveTopology,
				(uint64)m_platformData.pRootSignature,
				(uint64)m_pPixelShader,
				(uint64)m_pRasterizerState,
				(uint64)m_pVertexShader
			};
			const crc32 crc = crcBytes( aRelevantStates, sizeof( aRelevantStates ) );

			ID3D12PipelineState* pPipelineState = nullptr;
			for( uint i = 0u; i < m_platformData.pipelineStates.getCount(); ++i )
			{
				const GraphicsContextPipelineState& pipelineState = m_platformData.pipelineStates[ i ];

				if( pipelineState.crc == crc )
				{
					pPipelineState = pipelineState.pPpielineState;
					break;
				}
			}

			if( pPipelineState == nullptr )
			{
				static const D3D12_PRIMITIVE_TOPOLOGY_TYPE s_aTopologieTypeMapping[] =
				{
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,		// PrimitiveTopology_PointList
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,			// PrimitiveTopology_LineList
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,			// PrimitiveTopology_LineStrip
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,		// PrimitiveTopology_TriangleList
					D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE		// PrimitiveTopology_TriangleStrip
				};
				TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aTopologieTypeMapping ) == PrimitiveTopology_Count );

				PixelFormat depthFormat = m_pRenderTarget->m_depthBuffer.format;
				if( depthFormat == PixelFormat_Invalid && m_pRenderTarget->m_depthBuffer.pDataBuffer != nullptr )
				{
					depthFormat = m_pRenderTarget->m_depthBuffer.pDataBuffer->m_description.format;
				}

				TIKI_DECLARE_STACKANDZERO( D3D12_GRAPHICS_PIPELINE_STATE_DESC, pipelineDesc );
				pipelineDesc.BlendState				= m_pBlendState->m_platformData.blendDesc;
				pipelineDesc.DepthStencilState		= m_pDepthStencilState->m_platformData.depthStencilDesc;
				pipelineDesc.DSVFormat				= GraphicsSystemPlatform::getD3dPixelFormat( depthFormat, false );
				pipelineDesc.InputLayout			= m_pVertexInputBinding->m_platformData.inputLayoutDesc;
				pipelineDesc.NumRenderTargets		= (UINT)m_pRenderTarget->m_colorBufferCount;
				pipelineDesc.PrimitiveTopologyType	= s_aTopologieTypeMapping[ m_primitiveTopology ];
				pipelineDesc.pRootSignature			= m_platformData.pRootSignature;
				pipelineDesc.PS						= m_pPixelShader->m_platformData.shaderCode;
				pipelineDesc.RasterizerState		= m_pRasterizerState->m_platformData.rasterizerDesc;
				pipelineDesc.VS						= m_pVertexShader->m_platformData.shaderCode;
				pipelineDesc.SampleDesc.Count		= 1u;

				for( uint i = 0u; i < m_pRenderTarget->m_colorBufferCount; ++i )
				{
					PixelFormat colorFormat = m_pRenderTarget->m_colorBuffers[ i ].format;
					if( colorFormat == PixelFormat_Invalid )
					{
						colorFormat = m_pRenderTarget->m_colorBuffers[ i ].pDataBuffer->m_description.format;
					}

					pipelineDesc.RTVFormats[ i ] = GraphicsSystemPlatform::getD3dPixelFormat( colorFormat, false );
				}

				TIKI_VERIFY( SUCCEEDED( m_platformData.pDevice->CreateGraphicsPipelineState( &pipelineDesc, IID_PPV_ARGS( &pPipelineState ) ) ) );

				GraphicsContextPipelineState& pipelineState = m_platformData.pipelineStates.push();
				pipelineState.crc				= crc;
				pipelineState.pPpielineState	= pPipelineState;
			}

			static const D3D12_PRIMITIVE_TOPOLOGY s_aTopologieMapping[] =
			{
				D3D_PRIMITIVE_TOPOLOGY_POINTLIST,		// PrimitiveTopology_PointList
				D3D_PRIMITIVE_TOPOLOGY_LINELIST,		// PrimitiveTopology_LineList
				D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,		// PrimitiveTopology_LineStrip
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,	// PrimitiveTopology_TriangleList
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP	// PrimitiveTopology_TriangleStrip
			};
			TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aTopologieMapping ) == PrimitiveTopology_Count );

			m_platformData.pCommandList->IASetPrimitiveTopology( s_aTopologieMapping[ m_primitiveTopology ] );
			m_platformData.pCommandList->SetPipelineState( pPipelineState );
		}

		//// set descriptors
		//{
		//	FixedSizedArray< ID3D12DescriptorHeap*, GraphicsSystemLimits_MaxDescriptorHeaps > heaps;

		//	FixedSizedArray< ViewHandleDataD3d12, GraphicsSystemLimits_VertexShaderTextureSlots + GraphicsSystemLimits_PixelShaderTextureSlots > samplerViews;
		//	FixedSizedArray< ViewHandleDataD3d12, GraphicsSystemLimits_VertexShaderTextureSlots + GraphicsSystemLimits_PixelShaderTextureSlots > textureViews;
		//	FixedSizedArray< ViewHandleDataD3d12, GraphicsSystemLimits_VertexShaderConstantSlots + GraphicsSystemLimits_PixelShaderConstantSlots > constantViews;

		//	// vertex
		//	for( uint i = 0u; i < TIKI_COUNT( m_apVertexSamplerStates ); ++i )
		//	{
		//		if( m_apVertexSamplerStates[ i ] != nullptr )
		//		{
		//			ID3D12DescriptorHeap* pDescriptorHeap = m_apVertexSamplerStates[ i ]->m_platformData.pDescriptorHeap;

		//			heaps.push( pDescriptorHeap );
		//			samplerViews.push() = ViewHandleDataD3d12( (UINT)i, pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
		//		}
		//	}

		//	for( uint i = 0u; i < TIKI_COUNT( m_apVertexTextures ); ++i )
		//	{
		//		if( m_apVertexTextures[ i ] != nullptr )
		//		{
		//			ID3D12DescriptorHeap* pDescriptorHeap = m_apVertexTextures[ i ]->m_platformData.pDescriptorHeap;

		//			heaps.push( pDescriptorHeap );
		//			textureViews.push() = ViewHandleDataD3d12( (UINT)i, pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
		//		}
		//	}

		//	for( uint i = 0u; i < TIKI_COUNT( m_apVertexConstants ); ++i )
		//	{
		//		if( m_apVertexConstants[ i ] != nullptr )
		//		{
		//			ID3D12DescriptorHeap* pDescriptorHeap = m_apVertexConstants[ i ]->m_pDescriptorHeap;

		//			heaps.push( pDescriptorHeap );
		//			constantViews.push() = ViewHandleDataD3d12( (UINT)i, pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
		//		}
		//	}
		//	
		//	// pixel
		//	for( uint i = 0u; i < TIKI_COUNT( m_apPixelSamplerStates ); ++i )
		//	{
		//		if( m_apPixelSamplerStates[ i ] != nullptr )
		//		{
		//			ID3D12DescriptorHeap* pDescriptorHeap = m_apPixelSamplerStates[ i ]->m_platformData.pDescriptorHeap;

		//			heaps.push( pDescriptorHeap );
		//			samplerViews.push() = ViewHandleDataD3d12( (UINT)i, pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
		//		}
		//	}

		//	for( uint i = 0u; i < TIKI_COUNT( m_apPixelTextures ); ++i )
		//	{
		//		if( m_apPixelTextures[ i ] != nullptr )
		//		{
		//			ID3D12DescriptorHeap* pDescriptorHeap = m_apPixelTextures[ i ]->m_platformData.pDescriptorHeap;

		//			heaps.push( pDescriptorHeap );
		//			textureViews.push() = ViewHandleDataD3d12( (UINT)i, pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
		//		}
		//	}

		//	for( uint i = 0u; i < TIKI_COUNT( m_apPixelConstants ); ++i )
		//	{
		//		if( m_apPixelConstants[ i ] != nullptr )
		//		{
		//			ID3D12DescriptorHeap* pDescriptorHeap = m_apPixelConstants[ i ]->m_pDescriptorHeap;

		//			heaps.push( pDescriptorHeap );
		//			constantViews.push() = ViewHandleDataD3d12( (UINT)i, pDescriptorHeap->GetCPUDescriptorHandleForHeapStart() );
		//		}
		//	}

		//	m_platformData.pCommandList->SetDescriptorHeaps( (UINT)heaps.getCount(), heaps.getBegin() );

		//	for (UINT i = 0u; i < heaps.getCount(); ++i)
		//	{
		//		m_platformData.pCommandList->SetGraphicsRootDescriptorTable( i, heaps[ i ]->GetGPUDescriptorHandleForHeapStart() );
		//	}

		//	//for( uint i = 0u; i < samplerViews.getCount(); ++i )
		//	//{
		//	//	const ViewData& viewData = samplerViews[ i ];
		//	//	m_platformData.pCommandList->SetGraphicsRootShaderResourceView( viewData.slot, viewData.handle );
		//	//}

		//	//for( uint i = 0u; i < textureViews.getCount(); ++i )
		//	//{
		//	//	const ViewHandleDataD3d12& viewData = textureViews[ i ];
		//	//	m_platformData.pCommandList->SetGraphicsRootShaderResourceView( viewData.slot, viewData.handle );
		//	//}

		//	//for( uint i = 0u; i < constantViews.getCount(); ++i )
		//	//{
		//	//	const ViewHandleDataD3d12& viewData = constantViews[ i ];
		//	//	m_platformData.pCommandList->SetGraphicsRootShaderResourceView( viewData.slot, viewData.handle );
		//	//}
		//}
	}
}