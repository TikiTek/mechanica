
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
#include "tiki/graphics/shadertype.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexinputbinding.hpp"
#include "tiki/graphics/viewport.hpp"
#include "tiki/math/rectangle.hpp"

#include <d3d11.h>

namespace tiki
{
	GraphicsContext::GraphicsContext()
	{
		m_pGraphicsSystem = nullptr;
	}

	GraphicsContext::~GraphicsContext()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_platformData.pDevice == nullptr );
		TIKI_ASSERT( m_platformData.pContext == nullptr );
	}

	bool GraphicsContext::create( GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_platformData.pDevice == nullptr );
		TIKI_ASSERT( m_platformData.pContext == nullptr );

		m_pGraphicsSystem		= &graphicsSystem;
		m_platformData.pDevice	= GraphicsSystemPlatform::getDevice( graphicsSystem );
		m_platformData.pContext	= GraphicsSystemPlatform::getContext( graphicsSystem );

		m_pRenderTarget = nullptr;

		for (uint i = 0u; i < TIKI_COUNT( m_apRenderPassesStack ); ++i)
		{
			m_apRenderPassesStack[ i ] = nullptr;
		}
		m_currentRenderPassDepth = 0u;

		m_immediateVertexData.create( graphicsSystem, MaxImmediateGeometrySize / 4u, 4u );
		m_platformData.immediateVertexCount		= 0u;
		m_platformData.immediateVertexStride	= 0u;

		invalidateState();

		return true;
	}

	void GraphicsContext::dispose( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem		= nullptr;
		m_platformData.pDevice	= nullptr;
		m_platformData.pContext	= nullptr;

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

		if ( depthClearFlags != 0u && renderTarget.m_platformData.pDepthView != nullptr )
		{
			TIKI_ASSERT( renderTarget.m_platformData.pDepthView != nullptr );
			m_platformData.pContext->ClearDepthStencilView( renderTarget.m_platformData.pDepthView, depthClearFlags, depthValue, stencilValue );
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
				TIKI_ASSERT( renderTarget.m_platformData.pColorViews[ i ] != nullptr );
				m_platformData.pContext->ClearRenderTargetView( renderTarget.m_platformData.pColorViews[ i ], &floatColor.x );
			}
		}
	}

	void GraphicsContext::copyTextureData( const TextureData& sourceData, const TextureData& targetData )
	{
		m_platformData.pContext->CopyResource(
			targetData.m_platformData.pResource,
			sourceData.m_platformData.pResource
		);
	}

	void GraphicsContext::beginRenderPass( const RenderTarget& renderTarget, const Viewport* pViewport /* = nullptr*/ )
	{
		TIKI_ASSERT( m_currentRenderPassDepth < GraphicsSystemLimits_RenderPassStackDepth );

		m_apRenderPassesStack[ m_currentRenderPassDepth ] = &renderTarget;
		m_currentRenderPassDepth++;
		
		ID3D11ShaderResourceView* apShaderResources[ GraphicsSystemLimits_PixelShaderTextureSlots ] =
		{
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
		};
		m_platformData.pContext->PSSetShaderResources( 0u, TIKI_COUNT( apShaderResources ), apShaderResources );

		m_platformData.pContext->OMSetRenderTargets( (UINT)renderTarget.m_colorBufferCount, renderTarget.m_platformData.pColorViews, renderTarget.m_platformData.pDepthView );

		D3D11_VIEWPORT viewPort;
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
		m_platformData.pContext->RSSetViewports( 1u, &viewPort );

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
			m_platformData.pContext->OMSetRenderTargets( (UINT)renderTarget.m_colorBufferCount, renderTarget.m_platformData.pColorViews, renderTarget.m_platformData.pDepthView );
		}
		else
		{
			ID3D11RenderTargetView* apRenderTargetViews[ GraphicsSystemLimits_RenderTargetSlots ] =
			{
				nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
			};

			m_platformData.pContext->OMSetRenderTargets( GraphicsSystemLimits_RenderTargetSlots, apRenderTargetViews, nullptr );
		}

		invalidateState();
	}


	void GraphicsContext::setBlendState( const BlendState* pBlendState )
	{
		TIKI_ASSERT( pBlendState != nullptr );

		if ( m_pBlendState != pBlendState )
		{
			static const float s_aBlendFactor[ 4u ] = { 1.0f, 1.0f, 1.0f, 1.0f };

			m_platformData.pContext->OMSetBlendState( pBlendState->m_platformData.pBlendState, s_aBlendFactor, 0xffffffffu );
			m_pBlendState = pBlendState;
		}		
	}

	void GraphicsContext::setDepthStencilState( const DepthStencilState* pDepthStencilState )
	{
		TIKI_ASSERT( pDepthStencilState != nullptr );

		if ( m_pDepthStencilState != pDepthStencilState )
		{
			m_platformData.pContext->OMSetDepthStencilState( pDepthStencilState->m_platformData.pDepthStencilState, UINT( pDepthStencilState->m_platformData.stencilRef ) );
			m_pDepthStencilState = pDepthStencilState;
		}		
	}

	void GraphicsContext::setRasterizerState( const RasterizerState* pRasterizerState )
	{
		TIKI_ASSERT( pRasterizerState != nullptr );

		if ( m_pRasterizerState != pRasterizerState )
		{
			m_platformData.pContext->RSSetState( pRasterizerState->m_platformData.pRasterizerState );
			m_pRasterizerState = pRasterizerState;
		}
	}

	void GraphicsContext::setPrimitiveTopology( PrimitiveTopology topology )
	{
		static const D3D11_PRIMITIVE_TOPOLOGY s_aTopologies[ PrimitiveTopology_Count ] =
		{
			D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		};

		if ( m_primitiveTopology != topology )
		{
			m_platformData.pContext->IASetPrimitiveTopology( s_aTopologies[ topology ] );
			m_primitiveTopology = topology;
		}
	}

	void GraphicsContext::setVertexShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_VertexShader );

		if ( m_pVertexShader != pShader )
		{
			m_platformData.pContext->VSSetShader( pShader->m_platformData.pVertexShader, nullptr, 0 );
			m_pVertexShader = pShader;
		}
	}

	void GraphicsContext::setVertexInputBinding( const VertexInputBinding* pVertexInputBinding )
	{
		TIKI_ASSERT( pVertexInputBinding != nullptr );
		if ( m_pVertexInputBinding != pVertexInputBinding )
		{
			m_platformData.pContext->IASetInputLayout( pVertexInputBinding->m_platformData.pInputLayout );
			m_pVertexInputBinding = pVertexInputBinding;
		}
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

		if ( m_pPixelShader != pShader )
		{
			m_platformData.pContext->PSSetShader( pShader->m_platformData.pPixelShader, nullptr, 0 );
			m_pPixelShader = pShader;
		}
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
		m_platformData.pContext->IASetIndexBuffer( 
			indexBuffer.m_pBuffer,
			( indexBuffer.m_indexType == IndexType_UInt32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT ),
			0u
		);
	}

	void GraphicsContext::setVertexBuffer( uint slot, const VertexBuffer& buffer )
	{
		const UINT offset = 0u;
		const UINT vertexStride = static_cast< UINT >( buffer.m_stride );
		m_platformData.pContext->IASetVertexBuffers( (UINT)slot, 1u, &buffer.m_pBuffer, &vertexStride, &offset );
	}

	void* GraphicsContext::beginImmediateGeometry( uint vertexStride, uint vertexCount )
	{
		TIKI_ASSERT( vertexStride * vertexCount <= MaxImmediateGeometrySize );

		m_platformData.immediateVertexStride	= vertexStride;
		m_platformData.immediateVertexCount		= vertexCount;

		return mapBuffer( m_immediateVertexData );
	}

	void GraphicsContext::endImmediateGeometry()
	{
		unmapBuffer( m_immediateVertexData );
		if( !validateDrawCall() )
		{
			return;
		}

		const UINT offset = 0u;
		const UINT vertexStride = static_cast< UINT >(m_platformData.immediateVertexStride );
		m_platformData.pContext->IASetVertexBuffers( 0u, 1u, &m_immediateVertexData.m_pBuffer, &vertexStride, &offset );

		m_platformData.pContext->Draw( (UINT)m_platformData.immediateVertexCount, 0u );
	}

	void GraphicsContext::drawGeometry( uint vertexCount, uint baseVertexOffset /*= 0u*/ )
	{
		if( !validateDrawCall() )
		{
			return;
		}

		m_platformData.pContext->Draw( (UINT)vertexCount, (UINT)baseVertexOffset );
	}

	void GraphicsContext::drawIndexedGeometry( uint indexCount, uint baseIndexOffset /*= 0u*/, uint baseVertexOffset /*= 0u*/ )
	{
		if( !validateDrawCall() )
		{
			return;
		}

		m_platformData.pContext->DrawIndexed( (UINT)indexCount, (UINT)baseIndexOffset, (UINT)baseVertexOffset );
	}

	void* GraphicsContext::mapBuffer( const BaseBuffer& buffer )
	{
		TIKI_ASSERT( buffer.m_pBuffer != nullptr );

		D3D11_MAPPED_SUBRESOURCE mapped;
		m_platformData.pContext->Map( buffer.m_pBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped );

		return mapped.pData;
	}

	void GraphicsContext::unmapBuffer( const BaseBuffer& buffer )
	{
		m_platformData.pContext->Unmap( buffer.m_pBuffer, 0u );
	}

	const RenderTarget& GraphicsContext::getBackBuffer() const
	{
		return m_pGraphicsSystem->getBackBuffer();
	}
}