
#include "tiki/graphics/graphicscontext.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/functions.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/indexbuffer.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphicsbase/primitivetopologies.hpp"
#include "tiki/graphicsbase/shadertype.hpp"
#include "tiki/graphicsresources/material.hpp"
#include "tiki/graphicsresources/shader.hpp"
#include "tiki/math/rectangle.hpp"

#include "graphicshandles.hpp"

namespace tiki
{
	GraphicsContext::GraphicsContext()
	{
		m_pGraphicsSystem	= nullptr;
		m_pHandles			= nullptr;
	}

	GraphicsContext::~GraphicsContext()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_pHandles == nullptr );
	}

	bool GraphicsContext::create( GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
		TIKI_ASSERT( m_pHandles == nullptr );

		m_pVertexShader = nullptr;

		for (size_t i = 0u; i < TIKI_COUNT( m_pVertexSamplerStates ); ++i)
		{
			m_pVertexSamplerStates[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_pVertexTextures ); ++i)
		{
			m_pVertexTextures[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_pVertexConstants ); ++i)
		{
			m_pVertexConstants[ i ] = nullptr;
		}

		m_pPixelShader = nullptr;

		for (size_t i = 0u; i < TIKI_COUNT( m_pPixelSamplerStates ); ++i)
		{
			m_pPixelSamplerStates[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_pPixelTextures ); ++i)
		{
			m_pPixelTextures[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_pPixelConstants ); ++i)
		{
			m_pPixelConstants[ i ] = nullptr;
		}

		m_pGraphicsSystem	= &graphicsSystem;
		m_pHandles			= getHandles( graphicsSystem );

		return true;
	}

	void GraphicsContext::dispose()
	{
		m_pGraphicsSystem	= nullptr;
		m_pHandles			= nullptr;
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
			m_pHandles->pContext->ClearDepthStencilView( renderTarget.m_pDepthView, depthClearFlags, depthValue, stencilValue );
		}

		for (size_t i = 0u; i < renderTarget.m_colorBufferCount; ++i)
		{
			if ( isBitSet( clearMask, ClearMask_Color0 << i ) )
			{
				m_pHandles->pContext->ClearRenderTargetView( renderTarget.m_pColorViews[ i ], &floatColor.x );
			}
		}
	}

	void GraphicsContext::beginRenderPass( const RenderTarget& renderTarget )
	{

	}

	void GraphicsContext::endRenderPass()
	{

	}

	//void GraphicsContext::setMaterial( const Material* pMaterial )
	//{
	//	TIKI_ASSERT( pMaterial );
	//	TIKI_ASSERT( pMaterial->m_pPixelShader );
	//	TIKI_ASSERT( pMaterial->m_pVertexShader );

	//	setVertexShader( pMaterial->m_pVertexShader );
	//	setPixelShader( pMaterial->m_pPixelShader );

	//	for( uint i = 0; i < pMaterial->m_textures.getCount(); ++i )
	//	{
	//		auto& iatr = pMaterial->m_textures[ i ];

	//		setPixelShaderTexture( &iatr.m_pTexture->getTextureData(), iatr.m_slot );
	//	}
	//}

	void GraphicsContext::setInputLayout( const VertexFormat* pVertexFormat )
	{
		m_pHandles->pContext->IASetInputLayout( pVertexFormat->m_pInputLayout );
	}

	void GraphicsContext::setPrimitiveTopology( PrimitiveTopology topology )
	{
		static const D3D11_PRIMITIVE_TOPOLOGY topologies[] =
		{
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( topologies ) == PrimitiveTopology_Count );

		m_pHandles->pContext->IASetPrimitiveTopology( topologies[ topology ] );
	}

	void GraphicsContext::setVertexShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_VertexShader );

		if ( m_pVertexShader != pShader )
		{
			m_pHandles->pContext->VSSetShader( (TGVertexShader*)pShader->getShaderObject(), nullptr, 0 );
			m_pVertexShader = pShader;
		}
	}

	void GraphicsContext::setVertexShaderSamplerState( size_t slot, const SamplerState* pSampler )
	{
		if ( m_pVertexSamplerStates[ slot ] != pSampler )
		{
			if ( pSampler == nullptr )
			{
				ID3D11SamplerState* pNullSampler = nullptr;
				m_pHandles->pContext->VSSetSamplers( slot, 1u, &pNullSampler );
			}
			else
			{
				m_pHandles->pContext->VSSetSamplers( slot, 1u, &pSampler->m_pSamplerState );
			}

			m_pVertexSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setVertexShaderTexture( size_t slot, const TextureData* pTextureData )
	{
		if ( pTextureData == nullptr )
		{
			m_pHandles->pContext->VSSetShaderResources( slot, 1, nullptr );
		}
		else
		{
			m_pHandles->pContext->VSSetShaderResources( slot, 1, &pTextureData->m_pShaderView );
		}
	}

	void GraphicsContext::setVertexShaderConstant( size_t slot, const ConstantBuffer& buffer )
	{
		if ( m_pVertexConstants[ slot ] != &buffer )
		{
			m_pHandles->pContext->VSSetConstantBuffers( slot, 1u, &buffer.m_pBuffer );
			m_pVertexConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setPixelShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_PixelShader );

		if ( m_pPixelShader != pShader )
		{
			m_pHandles->pContext->PSSetShader( (TGPixelShader*) pShader->getShaderObject(), nullptr, 0 );
			m_pPixelShader = pShader;
		}
	}

	void GraphicsContext::setPixelShaderSamplerState( size_t slot, const SamplerState* pSampler )
	{
		if ( m_pPixelSamplerStates[ slot ] != pSampler )
		{
			if ( pSampler == nullptr )
			{
				ID3D11SamplerState* pNullSampler = nullptr;
				m_pHandles->pContext->PSSetSamplers( slot, 1u, &pNullSampler );
			}
			else
			{
				m_pHandles->pContext->PSSetSamplers( slot, 1u, &pSampler->m_pSamplerState );
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
				m_pHandles->pContext->PSSetShaderResources( slot, 1u, &pNullResourceView );
			}
			else
			{
				m_pHandles->pContext->PSSetShaderResources( slot, 1u, &pTextureData->m_pShaderView );
			}

			m_pPixelTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setPixelShaderConstant( size_t slot, const ConstantBuffer& buffer )
	{
		if ( m_pPixelConstants[ slot ] != &buffer)
		{
			m_pHandles->pContext->PSSetConstantBuffers( slot, 1u, &buffer.m_pBuffer );
			m_pPixelConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setIndexBuffer( const IndexBuffer& indexBuffer )
	{
		m_pHandles->pContext->IASetIndexBuffer( 
			indexBuffer.m_pBuffer,
			( indexBuffer.m_indexType == IndexType_Uint32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT ),
			0u
		);
	}

	void GraphicsContext::setVertexBuffer( size_t slot, const VertexBuffer& buffer )
	{
		const size_t offset = 0u;
		m_pHandles->pContext->IASetVertexBuffers( slot, 1u, &buffer.m_pBuffer, &buffer.m_stride, &offset );
	}
	
	void GraphicsContext::drawGeometry( const uint indexcount, const uint startIndex /*= 0u*/, const uint basevertex /*= 0u */ )
	{

	}

	void* GraphicsContext::mapBuffer( BaseBuffer& buffer )
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		m_pHandles->pContext->Map( buffer.m_pBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped );

		return mapped.pData;
	}

	void GraphicsContext::unmapBuffer( BaseBuffer& buffer )
	{
		m_pHandles->pContext->Unmap( buffer.m_pBuffer, 0u );
	}

	const RenderTarget* GraphicsContext::getBackBuffer() const
	{
		return m_pGraphicsSystem->getBackBuffer();
	}
}