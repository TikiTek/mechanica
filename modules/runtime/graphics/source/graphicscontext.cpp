
#include "tiki/graphics/graphicscontext.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphicsbase/primitivetopologies.hpp"
#include "tiki/graphicsresources/material.hpp"
#include "tiki/graphicsresources/shader.hpp"
#include "tiki/math/rectangle.hpp"

#include "graphicshandles.hpp"

namespace tiki
{
	GraphicsContext::GraphicsContext()
	{
		m_pGraphics	= nullptr;
		m_pHandles	= nullptr;
	}

	GraphicsContext::~GraphicsContext()
	{
		TIKI_ASSERT( m_pGraphics == nullptr );
		TIKI_ASSERT( m_pHandles == nullptr );
	}

	bool GraphicsContext::create( GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pGraphics == nullptr );
		TIKI_ASSERT( m_pHandles == nullptr );

		m_pGraphics	= &graphicsSystem;
		m_pHandles	= getHandles( graphicsSystem );

		return true;
	}

	void GraphicsContext::dispose()
	{
		if ( m_pGraphics == nullptr )
		{
			return;
		}

		m_pGraphics	= nullptr;
		m_pHandles	= nullptr;
	}

	void GraphicsContext::clear( const RenderTarget& renderTarget, const Color& color /* = Color::black */, float depthValue /* = 1.0f */, uint8 stencilValue /* = 0u */, ClearMask clearMask /* = ClearMask_All */ )
	{
		//m_pHandles->pContext->ClearRenderTargetView( (TGRenderTarget*)renderTarget.getRenderTarget(), &color.r );
	}

	const RenderTarget* GraphicsContext::getBackBuffer() const
	{
		return m_pRenderTarget;
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

	void GraphicsContext::setVertexShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader );
		TIKI_ASSERT( pShader->getShaderType() == ShaderTypes_VertexShader );

		m_pHandles->pContext->VSSetShader( (TGVertexShader*) pShader->getShaderObject(), nullptr, 0 );
	}

	void GraphicsContext::setPixelShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader );
		TIKI_ASSERT( pShader->getShaderType() == ShaderTypes_PixelShader );

		m_pHandles->pContext->PSSetShader( (TGPixelShader*) pShader->getShaderObject(), nullptr, 0 );
	}

	void GraphicsContext::setInputLayout( const VertexFormat& vertexFormat )
	{
		m_pHandles->pContext->IASetInputLayout( vertexFormat.m_pInputLayout );
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

	void GraphicsContext::setSamplerState( size_t slot, const SamplerState& sampler )
	{
		//m_pHandles->pContext->PSSetSamplers( slot, 1, sampler.getState() );
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

	void GraphicsContext::setPixelShaderTexture( size_t slot, const TextureData* pTextureData )
	{
		if ( pTextureData == nullptr )
		{
			m_pHandles->pContext->PSSetShaderResources( slot, 1, nullptr );
		}
		else
		{
			m_pHandles->pContext->PSSetShaderResources( slot, 1, &pTextureData->m_pShaderView );
		}
	}

	//void GraphicsContext::setVertexBuffer( size_t slot, const VertexBuffer& buffer )
	//{
	//	uint offset = 0;
	//	uint stride = buffer.getStride();
	//	m_pHandles->pContext->IASetVertexBuffers( slot, 1, &buffer.m_pBuffer, &stride, &offset );
	//}

	//void GraphicsContext::setIndexBuffer( size_t slot, const IndexBuffer& indexBuffer )
	//{
	//	m_pHandles->pContext->IASetIndexBuffer( buffer.m_pBuffer, DXGI_FORMAT_R32_UINT, 0 );
	//}

	//void GraphicsContext::setConstantBuffer( size_t slot, const IndexBuffer& buffer )
	//{
	//	m_pHandles->pContext->PSSetConstantBuffers( slot, 1, &buffer.m_pBuffer );
	//	m_pHandles->pContext->VSSetConstantBuffers( slot, 1, &buffer.m_pBuffer );
	//}
}