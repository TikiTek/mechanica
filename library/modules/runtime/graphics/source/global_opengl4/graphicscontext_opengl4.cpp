
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

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static void setGlMode( GLenum mode, GLboolean enable )
	{
		( enable ? glEnable : glDisable )( mode );
	}

	GraphicsContext::GraphicsContext()
	{
		m_pGraphicsSystem = nullptr;
	}

	GraphicsContext::~GraphicsContext()
	{
	}

	bool GraphicsContext::create( GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );

		m_pGraphicsSystem		= &graphicsSystem;

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

		m_immediateVertexData.dispose( graphicsSystem );
	}

	void GraphicsContext::clear( const RenderTarget& renderTarget, Color clearColor /* = TIKI_COLOR_BLACK */, float depthValue /* = 1.0f */, uint8 stencilValue /* = 0u */, ClearMask clearMask /* = ClearMask_All */ )
	{
		glBindFramebuffer( GL_FRAMEBUFFER, renderTarget.m_platformData.frameBufferId );

		float4 floatColor;
		color::toFloat4( floatColor, clearColor );
		glClearColor( floatColor.x, floatColor.y, floatColor.z, floatColor.w );
		glClearDepth( depthValue );
		glClearStencil( stencilValue );

		GLbitfield glClearMask = 0u;
		if ( isBitSet( clearMask, ClearMask_Color ) )
		{
			glClearMask |= GL_COLOR_BUFFER_BIT;
		}

		if ( isBitSet( clearMask, ClearMask_Depth ) )
		{
			glClearMask |= GL_DEPTH_BUFFER_BIT;
		}

		if ( isBitSet( clearMask, ClearMask_Stencil ) )
		{
			glClearMask |= GL_STENCIL_BUFFER_BIT;
		}

		glClear( glClearMask );
	}

	void GraphicsContext::copyTextureData( const TextureData& sourceData, const TextureData& targetData )
	{

	}

	void GraphicsContext::beginRenderPass( const RenderTarget& renderTarget, const Viewport* pViewport /* = nullptr*/ )
	{
		TIKI_ASSERT( m_currentRenderPassDepth < GraphicsSystemLimits_RenderPassStackDepth );



		invalidateState();
	}

	void GraphicsContext::endRenderPass()
	{
		TIKI_ASSERT( m_currentRenderPassDepth != 0u );


		invalidateState();
	}


	void GraphicsContext::setBlendState( const BlendState* pBlendState )
	{
		TIKI_ASSERT( pBlendState != nullptr );

		if ( m_pBlendState != pBlendState )
		{
			setGlMode( GL_BLEND, pBlendState->m_platformData.blendEnabled );

			glBlendEquation( pBlendState->m_platformData.blendOperation );
			glBlendFunc(
				pBlendState->m_platformData.sourceBlend,
				pBlendState->m_platformData.destinationBlend
			);
			glColorMask(
				pBlendState->m_platformData.colorWriteMask[ 0u ],
				pBlendState->m_platformData.colorWriteMask[ 1u ],
				pBlendState->m_platformData.colorWriteMask[ 2u ],
				pBlendState->m_platformData.colorWriteMask[ 3u ]
			);

			m_pBlendState = pBlendState;
		}		
	}

	void GraphicsContext::setDepthStencilState( const DepthStencilState* pDepthStencilState )
	{
		TIKI_ASSERT( pDepthStencilState != nullptr );

		if ( m_pDepthStencilState != pDepthStencilState )
		{
			setGlMode( GL_DEPTH_TEST, pDepthStencilState->m_platformData.depthEnabled );
			setGlMode( GL_STENCIL_TEST, pDepthStencilState->m_platformData.stencilEnabled );
			glDepthMask( pDepthStencilState->m_platformData.depthWriteEnabled );

			glDepthFunc( pDepthStencilState->m_platformData.depthFunction );

			glStencilMask( pDepthStencilState->m_platformData.stencilWriteMask );

			glStencilFuncSeparate(
				GL_FRONT,
				pDepthStencilState->m_platformData.frontFace.stencilFunction,
				pDepthStencilState->m_platformData.stencilRefernce,
				pDepthStencilState->m_platformData.stencilReadMask
			);
			glStencilOpSeparate(
				GL_FRONT,
				pDepthStencilState->m_platformData.frontFace.stencilFailOperation,
				pDepthStencilState->m_platformData.frontFace.depthFailOperation,
				pDepthStencilState->m_platformData.frontFace.stencilPassOperation
			);

			glStencilFuncSeparate(
				GL_BACK,
				pDepthStencilState->m_platformData.backFace.stencilFunction,
				pDepthStencilState->m_platformData.stencilRefernce,
				pDepthStencilState->m_platformData.stencilReadMask
			);
			glStencilOpSeparate(
				GL_BACK,
				pDepthStencilState->m_platformData.backFace.stencilFailOperation,
				pDepthStencilState->m_platformData.backFace.depthFailOperation,
				pDepthStencilState->m_platformData.backFace.stencilPassOperation
			);

			m_pDepthStencilState = pDepthStencilState;
		}		
	}

	void GraphicsContext::setRasterizerState( const RasterizerState* pRasterizerState )
	{
		TIKI_ASSERT( pRasterizerState != nullptr );

		if ( m_pRasterizerState != pRasterizerState )
		{
			setGlMode( GL_CULL_FACE, pRasterizerState->m_platformData.cullEnabled );

			glCullFace( pRasterizerState->m_platformData.cullMode );
			glPolygonMode( GL_FRONT_AND_BACK, pRasterizerState->m_platformData.fillMode );
			glFrontFace( pRasterizerState->m_platformData.windingOrder );

			m_pRasterizerState = pRasterizerState;
		}
	}

	void GraphicsContext::setPrimitiveTopology( PrimitiveTopology topology )
	{
		//static const D3D11_PRIMITIVE_TOPOLOGY s_aTopologies[ PrimitiveTopology_Count ] =
		//{
		//	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		//	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		//};

		if ( m_primitiveTopology != topology )
		{
			//m_platformData.pContext->IASetPrimitiveTopology( s_aTopologies[ topology ] );
			m_primitiveTopology = topology;
		}
	}

	void GraphicsContext::setVertexShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_VertexShader );

		if ( m_pVertexShader != pShader )
		{
			//m_platformData.pContext->VSSetShader( pShader->m_platformData.pVertexShader, nullptr, 0 );
			m_pVertexShader = pShader;
		}
	}

	void GraphicsContext::setVertexInputBinding( const VertexInputBinding* pVertexInputBinding )
	{
		TIKI_ASSERT( pVertexInputBinding != nullptr );
		if ( m_pVertexInputBinding != pVertexInputBinding )
		{
			//m_platformData.pContext->IASetInputLayout( pVertexInputBinding->m_platformData.pInputLayout );
			m_pVertexInputBinding = pVertexInputBinding;
		}
	}

	void GraphicsContext::setVertexShaderSamplerState( uint slot, const SamplerState* pSampler )
	{
		if ( m_pVertexSamplerStates[ slot ] != pSampler )
		{
			const UINT d3dSlot = (UINT)slot;

			if ( pSampler == nullptr )
			{
				//m_platformData.pContext->VSSetSamplers( d3dSlot, 1u, nullptr );
			}
			else
			{
				//m_platformData.pContext->VSSetSamplers( d3dSlot, 1u, &pSampler->m_platformData.pSamplerState );
			}

			m_pVertexSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setVertexShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_pVertexTextures[ slot ] != pTextureData )
		{
			const UINT d3dSlot = (UINT)slot;

			if ( pTextureData == nullptr )
			{
				//m_platformData.pContext->VSSetShaderResources( d3dSlot, 1, nullptr );
			}
			else
			{
				//m_platformData.pContext->VSSetShaderResources( d3dSlot, 1, &pTextureData->m_platformData.pShaderView );
			}

			m_pVertexTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setVertexShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_pVertexConstants[ slot ] != &buffer )
		{
			//m_platformData.pContext->VSSetConstantBuffers( (UINT)slot, 1u, &buffer.m_pBuffer );
			m_pVertexConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setPixelShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_PixelShader );

		if ( m_pPixelShader != pShader )
		{
			//m_platformData.pContext->PSSetShader( pShader->m_platformData.pPixelShader, nullptr, 0 );
			m_pPixelShader = pShader;
		}
	}

	void GraphicsContext::setPixelShaderSamplerState( uint slot, const SamplerState* pSampler )
	{
		if ( m_pPixelSamplerStates[ slot ] != pSampler )
		{
			const UINT d3dSlot = (UINT)slot;

			if ( pSampler == nullptr )
			{
				glBindSampler( slot, 0u );
			}
			else
			{
				glBindSampler( slot, pSampler->m_platformData.samplerId );
			}

			m_pPixelSamplerStates[ slot ] = pSampler;
		}
	}

	void GraphicsContext::setPixelShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_pPixelTextures[ slot ] != pTextureData )
		{
			const UINT d3dSlot = (UINT)slot;

			if ( pTextureData == nullptr )
			{
				//ID3D11ShaderResourceView* pNullResourceView = nullptr;
				//m_platformData.pContext->PSSetShaderResources( d3dSlot, 1u, &pNullResourceView );
			}
			else
			{
				//m_platformData.pContext->PSSetShaderResources( d3dSlot, 1u, &pTextureData->m_platformData.pShaderView );
			}

			m_pPixelTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setPixelShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_pPixelConstants[ slot ] != &buffer)
		{
			//m_platformData.pContext->PSSetConstantBuffers( (UINT)slot, 1u, &buffer.m_pBuffer );
			m_pPixelConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setIndexBuffer( const IndexBuffer& indexBuffer )
	{
		//m_platformData.pContext->IASetIndexBuffer( 
		//	indexBuffer.m_pBuffer,
		//	( indexBuffer.m_indexType == IndexType_Uint32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT ),
		//	0u
		//);
	}

	void GraphicsContext::setVertexBuffer( uint slot, const VertexBuffer& buffer )
	{
		//const UINT offset = 0u;
		//const UINT vertexStride = static_cast< UINT >( buffer.m_stride );
		//m_platformData.pContext->IASetVertexBuffers( (UINT)slot, 1u, &buffer.m_pBuffer, &vertexStride, &offset );
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

		//const UINT offset = 0u;
		//const UINT vertexStride = static_cast< UINT >( m_immediateVertexStride );
		//m_platformData.pContext->IASetVertexBuffers( 0u, 1u, &m_immediateVertexData.m_pBuffer, &vertexStride, &offset );

		//m_platformData.pContext->Draw( (UINT)m_immediateVertexCount, 0u );
	}

	void GraphicsContext::drawGeometry( uint vertexCount, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );
		//m_platformData.pContext->Draw( (UINT)vertexCount, (UINT)baseVertexOffset );
	}

	void GraphicsContext::drawIndexedGeometry( uint indexCount, uint baseIndexOffset /*= 0u*/, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );
		//m_platformData.pContext->DrawIndexed( (UINT)indexCount, (UINT)baseIndexOffset, (UINT)baseVertexOffset );
	}

	void* GraphicsContext::mapBuffer( const BaseBuffer& buffer )
	{
		//TIKI_ASSERT( buffer.m_pBuffer != nullptr );

		//D3D11_MAPPED_SUBRESOURCE mapped;
		//m_platformData.pContext->Map( buffer.m_pBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped );

		//return mapped.pData;
		return nullptr;
	}

	void GraphicsContext::unmapBuffer( const BaseBuffer& buffer )
	{
		//m_platformData.pContext->Unmap( buffer.m_pBuffer, 0u );
	}

	const RenderTarget& GraphicsContext::getBackBuffer() const
	{
		return m_pGraphicsSystem->getBackBuffer();
	}
}