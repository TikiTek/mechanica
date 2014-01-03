
#include "tiki/graphics/graphicscontext.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/indexbuffer.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphics/vertexinputbinding.hpp"
#include "tiki/graphicsbase/shadertype.hpp"
#include "tiki/graphicsresources/material.hpp"
#include "tiki/math/rectangle.hpp"

#include "graphicssystem_internal_d3d11.hpp"

namespace tiki
{
	GraphicsContext::GraphicsContext()
	{
		m_pGraphicsSystem	= nullptr;
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
		m_platformData.pDevice	= graphics::getDevice( graphicsSystem );
		m_platformData.pContext	= graphics::getContext( graphicsSystem );

		m_pRenderTarget = nullptr;

		for (uint i = 0u; i < TIKI_COUNT( m_pRenderPassesStack ); ++i)
		{
			m_pRenderPassesStack[ i ] = nullptr;
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

		if ( depthClearFlags != 0u )
		{
			m_platformData.pContext->ClearDepthStencilView( renderTarget.m_platformData.pDepthView, depthClearFlags, depthValue, stencilValue );
		}

		TIKI_COMPILETIME_ASSERT( ClearMask_Color1 == ClearMask_Color0 << 1u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color2 == ClearMask_Color0 << 2u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color3 == ClearMask_Color0 << 3u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color4 == ClearMask_Color0 << 4u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color5 == ClearMask_Color0 << 5u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color6 == ClearMask_Color0 << 6u );
		TIKI_COMPILETIME_ASSERT( ClearMask_Color7 == ClearMask_Color0 << 7u );
		for (size_t i = 0u; i < renderTarget.m_colorBufferCount; ++i)
		{
			if ( isBitSet( clearMask, ClearMask_Color0 << i ) )
			{
				m_platformData.pContext->ClearRenderTargetView( renderTarget.m_platformData.pColorViews[ i ], &floatColor.x );
			}
		}
	}

	void GraphicsContext::beginRenderPass( const RenderTarget& renderTarget )
	{
		TIKI_ASSERT( m_currentRenderPassDepth < GraphicsSystemLimits_RenderPassStackDepth );

		m_pRenderPassesStack[ m_currentRenderPassDepth ] = &renderTarget;
		m_currentRenderPassDepth++;

		m_platformData.pContext->OMSetRenderTargets( renderTarget.m_colorBufferCount, renderTarget.m_platformData.pColorViews, renderTarget.m_platformData.pDepthView );
	}

	void GraphicsContext::endRenderPass()
	{
		TIKI_ASSERT( m_currentRenderPassDepth != 0u );

		m_currentRenderPassDepth--;
		m_pRenderPassesStack[ m_currentRenderPassDepth ] = nullptr;

		if ( m_currentRenderPassDepth != 0u )
		{
			const RenderTarget& renderTarget = *m_pRenderPassesStack[ m_currentRenderPassDepth - 1u ];
			m_platformData.pContext->OMSetRenderTargets( renderTarget.m_colorBufferCount, renderTarget.m_platformData.pColorViews, renderTarget.m_platformData.pDepthView );
		}
	}

	void GraphicsContext::setVertexInputBinding( const VertexInputBinding* pVertexInputBinding )
	{
		TIKI_ASSERT( pVertexInputBinding != nullptr );
		m_platformData.pContext->IASetInputLayout( pVertexInputBinding->m_platformData.pInputLayout );
	}

	void GraphicsContext::setPrimitiveTopology( PrimitiveTopology topology )
	{
		static const D3D11_PRIMITIVE_TOPOLOGY topologies[] =
		{
			D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( topologies ) == PrimitiveTopology_Count );

		m_platformData.pContext->IASetPrimitiveTopology( topologies[ topology ] );
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

	void GraphicsContext::setVertexShaderSamplerState( size_t slot, const SamplerState* pSampler )
	{
		if ( m_pVertexSamplerStates[ slot ] != pSampler )
		{
			if ( pSampler == nullptr )
			{
				m_platformData.pContext->VSSetSamplers( slot, 1u, nullptr );
			}
			else
			{
				m_platformData.pContext->VSSetSamplers( slot, 1u, &pSampler->m_platformData.pSamplerState );
			}

			m_pVertexSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setVertexShaderTexture( size_t slot, const TextureData* pTextureData )
	{
		if ( m_pVertexTextures[ slot ] != pTextureData )
		{
			if ( pTextureData == nullptr )
			{
				m_platformData.pContext->VSSetShaderResources( slot, 1, nullptr );
			}
			else
			{
				m_platformData.pContext->VSSetShaderResources( slot, 1, &pTextureData->m_platformData.pShaderView );
			}

			m_pVertexTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setVertexShaderConstant( size_t slot, const ConstantBuffer& buffer )
	{
		if ( m_pVertexConstants[ slot ] != &buffer )
		{
			m_platformData.pContext->VSSetConstantBuffers( slot, 1u, &buffer.m_pBuffer );
			m_pVertexConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setPixelShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_PixelShader );

		if ( m_pPixelShader != pShader )
		{
			m_platformData.pContext->PSSetShader( pShader->m_platformData.pPixelShader, nullptr, 0 );
			m_pPixelShader = pShader;
		}
	}

	void GraphicsContext::setPixelShaderSamplerState( size_t slot, const SamplerState* pSampler )
	{
		if ( m_pPixelSamplerStates[ slot ] != pSampler )
		{
			if ( pSampler == nullptr )
			{
				m_platformData.pContext->PSSetSamplers( slot, 1u, nullptr );
			}
			else
			{
				m_platformData.pContext->PSSetSamplers( slot, 1u, &pSampler->m_platformData.pSamplerState );
			}

			m_pPixelSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setPixelShaderTexture( size_t slot, const TextureData* pTextureData )
	{
		if ( m_pPixelTextures[ slot ] != pTextureData )
		{
			if ( pTextureData == nullptr )
			{
				ID3D11ShaderResourceView* pNullResourceView = nullptr;
				m_platformData.pContext->PSSetShaderResources( slot, 1u, &pNullResourceView );
			}
			else
			{
				m_platformData.pContext->PSSetShaderResources( slot, 1u, &pTextureData->m_platformData.pShaderView );
			}

			m_pPixelTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setPixelShaderConstant( size_t slot, const ConstantBuffer& buffer )
	{
		if ( m_pPixelConstants[ slot ] != &buffer)
		{
			m_platformData.pContext->PSSetConstantBuffers( slot, 1u, &buffer.m_pBuffer );
			m_pPixelConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setIndexBuffer( const IndexBuffer& indexBuffer )
	{
		m_platformData.pContext->IASetIndexBuffer( 
			indexBuffer.m_pBuffer,
			( indexBuffer.m_indexType == IndexType_Uint32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT ),
			0u
		);
	}

	void GraphicsContext::setVertexBuffer( size_t slot, const VertexBuffer& buffer )
	{
		const size_t offset = 0u;
		m_platformData.pContext->IASetVertexBuffers( slot, 1u, &buffer.m_pBuffer, &buffer.m_stride, &offset );
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

		const uint offset = 0u;
		m_platformData.pContext->IASetVertexBuffers( 0u, 1u, &m_immediateVertexData.m_pBuffer, &m_immediateVertexStride, &offset );

		m_platformData.pContext->Draw( m_immediateVertexCount, 0u );
	}

	void GraphicsContext::drawGeometry( uint vertexCount, uint baseVertexOffset /*= 0u*/ )
	{
		m_platformData.pContext->Draw( vertexCount, baseVertexOffset );
	}

	void GraphicsContext::drawIndexed( uint indexCount, uint baseIndexOffset /*= 0u*/, uint baseVertexOffset /*= 0u*/ )
	{
		m_platformData.pContext->DrawIndexed( indexCount, baseIndexOffset, baseVertexOffset );
	}

	void* GraphicsContext::mapBuffer( BaseBuffer& buffer )
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		m_platformData.pContext->Map( buffer.m_pBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped );

		return mapped.pData;
	}

	void GraphicsContext::unmapBuffer( BaseBuffer& buffer )
	{
		m_platformData.pContext->Unmap( buffer.m_pBuffer, 0u );
	}

	const RenderTarget& GraphicsContext::getBackBuffer() const
	{
		return m_pGraphicsSystem->getBackBuffer();
	}
}