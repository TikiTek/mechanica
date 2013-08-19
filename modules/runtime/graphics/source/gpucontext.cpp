
#include "tiki/graphics/gpucontext.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/sampler.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphicsbase/primitivetopologies.hpp"

#include "tiki/math/rectangle.hpp"

#include "graphicshandles.hpp"

namespace tiki
{
	GpuContext::GpuContext()
	{
		m_pGraphics	= nullptr;
		m_pHandles	= nullptr;
	}

	GpuContext::~GpuContext()
	{
		TIKI_ASSERT( m_pGraphics == nullptr );
		TIKI_ASSERT( m_pHandles == nullptr );
	}

	bool GpuContext::create( GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pGraphics == nullptr );
		TIKI_ASSERT( m_pHandles == nullptr );

		m_pGraphics	= &graphicsSystem;
		m_pHandles	= getHandles( graphicsSystem );

		if( !m_lineRender.create( this ) )
			return false;

		if( !m_spriteRenderer.create( this ) )
			return false;

		if( !m_deferredRenderer.create( graphicsSystem ) )
			return false;

		m_renderTargets.create( 8 );
		m_renderTargetCount = 0;

		return true;
	}

	void GpuContext::dispose()
	{
		if ( m_pGraphics == nullptr )
		{
			return;
		}

		m_pGraphics	= nullptr;
		m_pHandles	= nullptr;

		m_lineRender.dispose();
		m_spriteRenderer.dispose();
		m_deferredRenderer.dipose();

		m_renderTargets.dispose();
	}

	void GpuContext::beginFrame()
	{
		clearBackBuffer();
		clearDepthBuffer();

		//clearRenderTargetStack();
		//pushBackBufferTarget();
		//flushRenderTargetStack();

		m_deferredRenderer.begin( this );
	}

	void GpuContext::endFrame()
	{
		m_lineRender.flush();
		m_deferredRenderer.end( this );
		m_spriteRenderer.flush();
	}

	void GpuContext::beginLightPass()
	{
		m_deferredRenderer.beginLightPass( this );
	}

	void GpuContext::endLightPass()
	{
		m_deferredRenderer.endLightPass( this );
	}

	void GpuContext::mapRenderTargets()
	{
		m_deferredRenderer.mapRenderTargets( this );
	}

	void GpuContext::unmapRenderTargets()
	{
		m_deferredRenderer.unmapRenderTargets( this );
	}

	void GpuContext::flushSpriteRenderer()
	{
		m_spriteRenderer.flush();
	}

	void GpuContext::clearRenderTarget( const RenderTarget& renderTarget, const Color& color /*= Color::black*/ )
	{
		m_pHandles->pContext->ClearRenderTargetView( (TGRenderTarget*)renderTarget.getRenderTarget(), &color.r );
	}

	void GpuContext::clearBackBuffer( const Color& color /*= Color::black*/ )
	{
		m_pHandles->pContext->ClearRenderTargetView( m_pHandles->pBackBufferTargetView, &color.r );
	}

	void GpuContext::clearDepthBuffer()
	{
		m_pHandles->pContext->ClearDepthStencilView( m_pHandles->pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
	}

	const Vector2& GpuContext::getBackBufferSize() const
	{
		return m_pGraphics->getBackBufferSizeFloat();
	}

	void GpuContext::enableDepth()
	{
		m_pHandles->pContext->OMSetDepthStencilState( m_pHandles->pDepthStencilState, 1 );
	}

	void GpuContext::disableDepth()
	{
		m_pHandles->pContext->OMSetDepthStencilState( m_pHandles->pDepthStencilStateNoDepth, 1 );
	}

	void GpuContext::enableAlpha()
	{
		m_pHandles->pContext->OMSetBlendState( m_pHandles->pBlendStateAlphaOn, 0, 0xffffffff );
	}

	void GpuContext::disableAlpha()
	{
		m_pHandles->pContext->OMSetBlendState( m_pHandles->pBlendStateAlphaOff, 0, 0xffffffff );
	}

	void GpuContext::setMaterial( const Material* pMaterial )
	{
		TIKI_ASSERT( pMaterial );
		TIKI_ASSERT( pMaterial->m_pPixelShader );
		TIKI_ASSERT( pMaterial->m_pVertexShader );

		setVertexShader( pMaterial->m_pVertexShader );
		setPixelShader( pMaterial->m_pPixelShader );

		for( uint i = 0; i < pMaterial->m_textures.getCount(); ++i )
		{
			auto& iatr = pMaterial->m_textures[ i ];

			setPixelShaderTexture( &iatr.m_pTexture->getTextureData(), iatr.m_slot );
		}
	}

	void GpuContext::setVertexShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader );
		TIKI_ASSERT( pShader->getShaderType() == ShaderTypes_VertexShader );

		m_pHandles->pContext->VSSetShader( (TGVertexShader*) pShader->getShaderObject(), nullptr, 0 );
	}

	void GpuContext::setPixelShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader );
		TIKI_ASSERT( pShader->getShaderType() == ShaderTypes_PixelShader );

		m_pHandles->pContext->PSSetShader( (TGPixelShader*) pShader->getShaderObject(), nullptr, 0 );
	}

	void GpuContext::setInputLayout( const VertexFormat* pVertexFormat )
	{
		TIKI_ASSERT( pVertexFormat != nullptr );
		m_pHandles->pContext->IASetInputLayout( pVertexFormat->m_pInputLayout );
	}

	void GpuContext::setPrimitiveTopology( PrimitiveTopology topology )
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

	void GpuContext::setSampler( const Sampler& sampler, const uint slot /*= 0u*/ )
	{
		m_pHandles->pContext->PSSetSamplers( slot, 1, sampler.getState() );
	}

	void GpuContext::setVertexShaderTexture( const TextureData* pTextureData, const size_t slot /*= 0u */ )
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

	void GpuContext::setPixelShaderTexture( const TextureData* pTextureData, const size_t slot /* = 0u */ )
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

	void GpuContext::clearRenderTargetStack()
	{
		for( uint i = 0; i < 8; ++i )
			m_renderTargets[ i ] = nullptr;
		m_renderTargetCount = 0;
	}
	
	void GpuContext::pushRenderTarget( const RenderTarget& renderTarget )
	{
		TIKI_ASSERT( m_renderTargetCount != 8 );
		m_renderTargets[ m_renderTargetCount++ ] = renderTarget.getRenderTarget();
	}

	void GpuContext::pushBackBufferTarget()
	{
		m_renderTargets[ m_renderTargetCount++ ] = m_pHandles->pBackBufferTargetView;
	}
	
	void GpuContext::flushRenderTargetStack()
	{
		m_pHandles->pContext->OMSetRenderTargets( m_renderTargetCount, (TGRenderTarget*const*)m_renderTargets.getData(), 
												  m_pHandles->pDepthStencilView );
	}

	void GpuContext::drawIndexed( const uint indexcount, const uint startIndex /*= 0u*/, const uint basevertex /*= 0u*/ )
	{
		m_pHandles->pContext->DrawIndexed( indexcount, startIndex, basevertex );
	}

	void GpuContext::drawLine( const Vector3& start, const Vector3& end, const Color& color )
	{
		m_lineRender.drawLine( start, end, color );
	}

	void GpuContext::draw( const uint count, const uint offset /*= 0u*/ )
	{
		m_pHandles->pContext->Draw( count, offset );
	}

	void GpuContext::drawTexture( const Rectangle& rect, const TextureData& textureData, bool percentage /*= false*/ )
	{
		m_spriteRenderer.drawTexture( rect, textureData, percentage );
	}

	void GpuContext::drawTexture( const Rectangle& dest, const Rectangle& src, const TextureData& tex )
	{
		m_spriteRenderer.drawTexture( dest, src, tex );
	}

	void GpuContext::drawText( const Vector2& position, const Font& font, const string& text )
	{
		m_spriteRenderer.drawText( position, font, text );
	}

}