
#include "tiki/graphics/graphicscontext.hpp"

#include "tiki/base/assert.hpp"
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
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexinputbinding.hpp"
#include "tiki/graphics/shadertype.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/graphics/viewport.hpp"

#include "graphicssystem_internal_d3d12.hpp"

namespace tiki
{
	GraphicsContext::GraphicsContext()
	{
		m_pGraphicsSystem = nullptr;
	}

	GraphicsContext::~GraphicsContext()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_platformData.pCommandList == nullptr );
		TIKI_ASSERT( m_platformData.pDescriptionHeap == nullptr );
	}

	bool GraphicsContext::create( GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_platformData.pCommandList == nullptr );
		TIKI_ASSERT( m_platformData.pDescriptionHeap == nullptr );

		m_pGraphicsSystem				= &graphicsSystem;
		m_platformData.pDevice			= graphics::getDevice( graphicsSystem );
		m_platformData.pCommandList		= graphics::getCommandList( graphicsSystem );
		m_platformData.pRootSignature	= graphics::getPlatformData( graphicsSystem ).pRootSignature;

		m_pRenderTarget = nullptr;

		for (uint i = 0u; i < TIKI_COUNT( m_apRenderPassesStack ); ++i)
		{
			m_apRenderPassesStack[ i ] = nullptr;
		}
		m_currentRenderPassDepth = 0u;

		m_immediateVertexData.create( graphicsSystem, MaxImmediateGeometrySize, true, GraphicsBufferType_VertexBuffer, nullptr );
		m_immediateVertexCount	= 0u;
		m_immediateVertexStride	= 0u;

		invalidateState();

		return true;
	}

	void GraphicsContext::dispose( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem				= nullptr;
		m_platformData.pCommandList		= nullptr;
		m_platformData.pDescriptionHeap	= nullptr;

		m_immediateVertexData.dispose( graphicsSystem );
	}

	void GraphicsContext::clear( const RenderTarget& renderTarget, Color color /* = TIKI_COLOR_BLACK */, float depthValue /* = 1.0f */, uint8 stencilValue /* = 0u */, ClearMask clearMask /* = ClearMask_All */ )
	{
		float4 floatColor;
		color::toFloat4( floatColor, color );
		
		UINT depthClearFlags = 0u;
		if ( isBitSet( clearMask, ClearMask_Depth ) )
		{
			depthClearFlags |= D3D10_CLEAR_DEPTH;
		}

		if ( isBitSet( clearMask, ClearMask_Stencil ) )
		{
			depthClearFlags |= D3D10_CLEAR_STENCIL;
		}

		if ( depthClearFlags != 0u && renderTarget.m_platformData.pDepthHandle != nullptr )
		{
			m_platformData.pCommandList->ClearDepthStencilView( *renderTarget.m_platformData.pDepthHandle, depthClearFlags, depthValue, stencilValue, nullptr, 0u );
		}

		TIKI_COMPILETIME_ASSERT( ClearMask_Color1 == ClearMask_Color0 << 1u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color2 == ClearMask_Color0 << 2u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color3 == ClearMask_Color0 << 3u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color4 == ClearMask_Color0 << 4u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color5 == ClearMask_Color0 << 5u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color6 == ClearMask_Color0 << 6u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color7 == ClearMask_Color0 << 7u );
		for (uint i = 0u; i < renderTarget.m_colorBufferCount; ++i)
		{
			if ( isBitSet( clearMask, ClearMask_Color0 << i ) )
			{
				TIKI_ASSERT( renderTarget.m_platformData.pColorHandle != nullptr );
				m_platformData.pCommandList->ClearRenderTargetView( *renderTarget.m_platformData.pColorHandle, &floatColor.x, nullptr, 0u );
			}
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
		
		TGShaderResourceView* apShaderResources[ GraphicsSystemLimits_PixelShaderTextureSlots ] =
		{
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
		};
		m_platformData.pCommandList->PSSetShaderResources( 0u, TIKI_COUNT( apShaderResources ), apShaderResources );

		m_platformData.pCommandList->SetRenderTargets( renderTarget.m_platformData.pColorHandle, TRUE, (UINT)renderTarget.m_colorBufferCount, renderTarget.m_platformData.pDepthHandle );

		D3D12_VIEWPORT viewPort;
		if ( pViewport == nullptr )
		{
			viewPort.TopLeftX	= 0.0f;
			viewPort.TopLeftY	= 0.0f;
			viewPort.Width		= (float)renderTarget.getWidth();
			viewPort.Height		= (float)renderTarget.getHeight();
			viewPort.MinDepth	= 0.0f;
			viewPort.MaxDepth	= 1.0f;
		}
		else
		{
			viewPort.TopLeftX	= pViewport->x;
			viewPort.TopLeftY	= pViewport->y;
			viewPort.Width		= pViewport->width;
			viewPort.Height		= pViewport->height;
			viewPort.MinDepth	= pViewport->minDepth;
			viewPort.MaxDepth	= pViewport->maxDepth;
		}
		m_platformData.pCommandList->RSSetViewports( 1u, &viewPort );

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
			m_platformData.pCommandList->SetRenderTargets( renderTarget.m_platformData.pColorHandle, TRUE, (UINT)renderTarget.m_colorBufferCount, renderTarget.m_platformData.pDepthHandle );
		}
		else
		{
			m_platformData.pCommandList->SetRenderTargets( nullptr, FALSE, 0u, nullptr );
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
			const UINT d3dSlot = (UINT)slot;

			if ( pSampler == nullptr )
			{
				m_platformData.pContext->VSSetSamplers( d3dSlot, 1u, nullptr );
			}
			else
			{
				m_platformData.pContext->VSSetSamplers( d3dSlot, 1u, &pSampler->m_platformData.pSamplerState );
			}

			m_apVertexSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setVertexShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_apVertexTextures[ slot ] != pTextureData )
		{
			const UINT d3dSlot = (UINT)slot;

			if ( pTextureData == nullptr )
			{
				m_platformData.pContext->VSSetShaderResources( d3dSlot, 1, nullptr );
			}
			else
			{
				m_platformData.pContext->VSSetShaderResources( d3dSlot, 1, &pTextureData->m_platformData.pShaderView );
			}

			m_apVertexTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setVertexShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_apVertexConstants[ slot ] != &buffer )
		{
			m_platformData.pContext->VSSetConstantBuffers( (UINT)slot, 1u, &buffer.m_pBuffer );
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
			const UINT d3dSlot = (UINT)slot;

			if ( pSampler == nullptr )
			{
				m_platformData.pContext->PSSetSamplers( d3dSlot, 1u, nullptr );
			}
			else
			{
				m_platformData.pContext->PSSetSamplers( d3dSlot, 1u, &pSampler->m_platformData.pSamplerState );
			}

			m_apPixelSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setPixelShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_apPixelTextures[ slot ] != pTextureData )
		{
			const UINT d3dSlot = (UINT)slot;

			if ( pTextureData == nullptr )
			{
				ID3D11ShaderResourceView* pNullResourceView = nullptr;
				m_platformData.pContext->PSSetShaderResources( d3dSlot, 1u, &pNullResourceView );
			}
			else
			{
				m_platformData.pContext->PSSetShaderResources( d3dSlot, 1u, &pTextureData->m_platformData.pShaderView );
			}

			m_apPixelTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setPixelShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_apPixelConstants[ slot ] != &buffer)
		{
			m_platformData.pContext->PSSetConstantBuffers( (UINT)slot, 1u, &buffer.m_pBuffer );
			m_apPixelConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setIndexBuffer( const IndexBuffer& indexBuffer )
	{
		m_platformData.pCommandList->SetIndexBuffer( &indexBuffer.m_pBuffer );
	}

	void GraphicsContext::setVertexBuffer( uint slot, const VertexBuffer& buffer )
	{
		m_platformData.pCommandList->SetVertexBuffers( (UINT)slot, &buffer.m_pBuffer, FALSE, 1u );
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

		const UINT offset = 0u;
		const UINT vertexStride = static_cast< UINT >( m_immediateVertexStride );
		m_platformData.pContext->IASetVertexBuffers( 0u, 1u, &m_immediateVertexData.m_pBuffer, &vertexStride, &offset );

		m_platformData.pCommandList->DrawInstanced( (UINT)m_immediateVertexCount, 1u, 0u, 0u );
	}

	void GraphicsContext::drawGeometry( uint vertexCount, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );
		m_platformData.pCommandList->DrawInstanced( (UINT)vertexCount, 1u, (UINT)baseVertexOffset, 0u );
	}

	void GraphicsContext::drawIndexedGeometry( uint indexCount, uint baseIndexOffset /*= 0u*/, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );
		m_platformData.pCommandList->DrawIndexedInstanced( (UINT)indexCount, 1u, (UINT)baseIndexOffset, (UINT)baseVertexOffset, 0u );
	}

	void* GraphicsContext::mapBuffer( const BaseBuffer& buffer )
	{
		TIKI_ASSERT( buffer.m_pBuffer != nullptr );

		void* pData;
		buffer.m_pBuffer->Map( nullptr, &pData );

		return pData;
	}

	void GraphicsContext::unmapBuffer( const BaseBuffer& buffer )
	{
		buffer.m_pBuffer->Unmap( nullptr );
	}

	const RenderTarget& GraphicsContext::getBackBuffer() const
	{
		return m_pGraphicsSystem->getBackBuffer();
	}

	void GraphicsContext::prepareDrawCall()
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
		const crc32 crc = crcBytes( aRelevantStates, sizeof(aRelevantStates) );

		ID3D12PipelineState* pPipelineState = nullptr;
		for (uint i = 0u; i < m_platformData.pipelineStates.getCount(); ++i)
		{
			const GraphicsContextPipelineState& pipelineState = m_platformData.pipelineStates[ i ];

			if( pipelineState.crc == crc )
			{
				pPipelineState = pipelineState.pPpielineState;
				break;
			}
		}

		static const D3D12_PRIMITIVE_TOPOLOGY_TYPE s_aTopologies[ PrimitiveTopology_Count ] =
		{
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
		};

		if( pPipelineState == nullptr )
		{
			TIKI_DECLARE_STACKANDZERO( D3D12_GRAPHICS_PIPELINE_STATE_DESC, pipelineDesc );
			pipelineDesc.BlendState				= m_pBlendState->m_platformData.blendDesc;
			pipelineDesc.DepthStencilState		= m_pDepthStencilState->m_platformData.depthStencilDesc;
			//pipelineDesc.DSVFormat
			pipelineDesc.InputLayout			= m_pVertexInputBinding->m_platformData.inputLayoutDesc;
			pipelineDesc.NumRenderTargets		= (UINT)m_pRenderTarget->m_colorBufferCount;
			pipelineDesc.PrimitiveTopologyType	= s_aTopologies[ m_primitiveTopology ];
			pipelineDesc.pRootSignature			= m_platformData.pRootSignature;
			pipelineDesc.PS						= m_pPixelShader->m_platformData.shaderCode;
			pipelineDesc.RasterizerState		= m_pRasterizerState->m_platformData.rasterizerDesc;
			//pipelineDesc.RTVFormats
			pipelineDesc.VS						= m_pVertexShader->m_platformData.shaderCode;
			TIKI_VERIFY( SUCCEEDED( m_platformData.pDevice->CreateGraphicsPipelineState( &pipelineDesc, &pPipelineState ) ) );

			GraphicsContextPipelineState& pipelineState = m_platformData.pipelineStates.push();
			pipelineState.crc				= crc;
			pipelineState.pPpielineState	= pPipelineState;
		}

		m_platformData.pCommandList->SetPipelineState( pPipelineState );
	}
}