
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
#include "shaderlinker_opengl4.hpp"

namespace tiki
{
	namespace graphics
	{
		static void setGlMode( GLenum mode, GLboolean enable );

		static void setLinkedShaderProgram( GraphicsSystem& graphicsSystem, GLuint vertexShaderId, GLuint pixelShaderId );
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

		for (uint i = 0u; i < TIKI_COUNT( m_apRenderPassesStack ); ++i)
		{
			m_apRenderPassesStack[ i ] = nullptr;
		}
		m_currentRenderPassDepth = 0u;

		m_immediateVertexData.create( graphicsSystem, MaxImmediateGeometrySize, true, GraphicsBufferType_VertexBuffer, nullptr );
		m_immediateVertexCount	= 0u;
		m_immediateVertexStride	= 0u;

		invalidateState();
		TIKI_CHECK_GRAPHICS;

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
		TIKI_CHECK_GRAPHICS;
	}

	void GraphicsContext::copyTextureData( const TextureData& sourceData, const TextureData& targetData )
	{

	}

	void GraphicsContext::beginRenderPass( const RenderTarget& renderTarget, const Viewport* pViewport /* = nullptr*/ )
	{
		TIKI_ASSERT( m_currentRenderPassDepth < GraphicsSystemLimits_RenderPassStackDepth );

		m_apRenderPassesStack[ m_currentRenderPassDepth ] = &renderTarget;
		m_currentRenderPassDepth++;

		glBindFramebuffer( GL_FRAMEBUFFER, renderTarget.m_platformData.frameBufferId );

		if ( pViewport == nullptr )
		{
			glViewport( 0, 0, renderTarget.getWidth(), renderTarget.getHeight() );
		}
		else
		{
			glViewport( (GLint)pViewport->x, (GLint)pViewport->y, (GLsizei)pViewport->width, (GLsizei)pViewport->height );
			//viewPort.MinDepth	= pViewport->minDepth;
			//viewPort.MaxDepth	= pViewport->maxDepth;
		}

		invalidateState();
		TIKI_CHECK_GRAPHICS;
	}

	void GraphicsContext::endRenderPass()
	{
		TIKI_ASSERT( m_currentRenderPassDepth != 0u );

		m_currentRenderPassDepth--;
		m_apRenderPassesStack[ m_currentRenderPassDepth ] = nullptr;

		if ( m_currentRenderPassDepth != 0u )
		{
			const RenderTarget& renderTarget = *m_apRenderPassesStack[ m_currentRenderPassDepth - 1u ];
			glBindFramebuffer( GL_FRAMEBUFFER, renderTarget.m_platformData.frameBufferId );
		}
		else
		{
			glBindFramebuffer( GL_FRAMEBUFFER, 0u );
		}

		invalidateState();
		TIKI_CHECK_GRAPHICS;
	}

	void GraphicsContext::setBlendState( const BlendState* pBlendState )
	{
		TIKI_ASSERT( pBlendState != nullptr );

		if ( m_pBlendState != pBlendState )
		{
			graphics::setGlMode( GL_BLEND, pBlendState->m_platformData.blendEnabled );

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
			TIKI_CHECK_GRAPHICS;
		}		
	}

	void GraphicsContext::setDepthStencilState( const DepthStencilState* pDepthStencilState )
	{
		TIKI_ASSERT( pDepthStencilState != nullptr );

		if ( m_pDepthStencilState != pDepthStencilState )
		{
			graphics::setGlMode( GL_DEPTH_TEST, pDepthStencilState->m_platformData.depthEnabled );
			graphics::setGlMode( GL_STENCIL_TEST, pDepthStencilState->m_platformData.stencilEnabled );
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
			TIKI_CHECK_GRAPHICS;
		}		
	}

	void GraphicsContext::setRasterizerState( const RasterizerState* pRasterizerState )
	{
		TIKI_ASSERT( pRasterizerState != nullptr );

		if ( m_pRasterizerState != pRasterizerState )
		{
			graphics::setGlMode( GL_CULL_FACE, pRasterizerState->m_platformData.cullEnabled );

			glCullFace( pRasterizerState->m_platformData.cullMode );
			glPolygonMode( GL_FRONT_AND_BACK, pRasterizerState->m_platformData.fillMode );
			glFrontFace( pRasterizerState->m_platformData.windingOrder );

			m_pRasterizerState = pRasterizerState;
			TIKI_CHECK_GRAPHICS;
		}
	}

	void GraphicsContext::setPrimitiveTopology( PrimitiveTopology topology )
	{
		static const GLenum s_aTopologies[] =
		{
			GL_POINTS,			// PrimitiveTopology_PointList
			GL_LINES,			// PrimitiveTopology_LineList
			GL_TRIANGLES,		// PrimitiveTopology_TriangleList
			GL_TRIANGLE_STRIP	// PrimitiveTopology_TriangleStrip
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aTopologies ) == PrimitiveTopology_Count );

		m_platformData.primitiveTopology = s_aTopologies[ topology ];
		m_primitiveTopology = topology;
	}

	void GraphicsContext::setVertexShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_VertexShader );

		if ( m_pVertexShader != pShader )
		{
			if ( pShader != nullptr )
			{
				m_platformData.vertexShaderId = pShader->m_platformData.shaderId;
			}
			else
			{
				m_platformData.vertexShaderId = 0u;
			}

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
			TIKI_CHECK_GRAPHICS;
		}
	}

	void GraphicsContext::setVertexShaderSamplerState( uint slot, const SamplerState* pSampler )
	{
		if ( m_apVertexSamplerStates[ slot ] != pSampler )
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

			m_apVertexSamplerStates[ slot ] = pSampler;
			TIKI_CHECK_GRAPHICS;
		}
	}

	void GraphicsContext::setVertexShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_apVertexTextures[ slot ] != pTextureData )
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

			m_apVertexTextures[ slot ] = pTextureData;
			TIKI_CHECK_GRAPHICS;
		}
	}

	void GraphicsContext::setVertexShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_apVertexConstants[ slot ] != &buffer )
		{
			//m_platformData.pContext->VSSetConstantBuffers( (UINT)slot, 1u, &buffer.m_pBuffer );
			//glGetUniformBlockIndex();
			//glUniformBlockBinding();
			m_apVertexConstants[ slot ] = &buffer;
			TIKI_CHECK_GRAPHICS;
		}
	}

	void GraphicsContext::setPixelShader( const Shader* pShader )
	{
		TIKI_ASSERT( pShader != nullptr );
		TIKI_ASSERT( pShader->getShaderType() == ShaderType_PixelShader );

		if ( m_pPixelShader != pShader )
		{
			if ( pShader != nullptr )
			{
				m_platformData.pixelShaderId = pShader->m_platformData.shaderId;
			}
			else
			{
				m_platformData.pixelShaderId = 0u;
			}

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
				glBindSampler( slot, 0u );
			}
			else
			{
				glBindSampler( slot, pSampler->m_platformData.samplerId );
			}

			m_apPixelSamplerStates[ slot ] = pSampler;
			TIKI_CHECK_GRAPHICS;
		}
	}

	void GraphicsContext::setPixelShaderTexture( uint slot, const TextureData* pTextureData )
	{
		if ( m_apPixelTextures[ slot ] != pTextureData )
		{
			const UINT d3dSlot = (UINT)slot;

			glActiveTexture( GL_TEXTURE0 + slot );
			if ( pTextureData == nullptr )
			{
				glBindTexture( m_apPixelTextures[ slot ]->m_platformData.textureType, 0u );
			}
			else
			{
				glBindTexture( pTextureData->m_platformData.textureType, pTextureData->m_platformData.textureId );
			}

			m_apPixelTextures[ slot ] = pTextureData;
		}
	}

	void GraphicsContext::setPixelShaderConstant( uint slot, const ConstantBuffer& buffer )
	{
		if ( m_apPixelConstants[ slot ] != &buffer)
		{
			//glUniformBufferEXT();
			//m_platformData.pContext->PSSetConstantBuffers( (UINT)slot, 1u, &buffer.m_pBuffer );
			m_apPixelConstants[ slot ] = &buffer;
		}
	}

	void GraphicsContext::setIndexBuffer( const IndexBuffer& indexBuffer )
	{
		TIKI_ASSERT( indexBuffer.m_bufferId != GL_INVALID_ENUM );
		TIKI_ASSERT( indexBuffer.m_bufferType == GL_ELEMENT_ARRAY_BUFFER );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer.m_bufferId );

		static const GLenum s_aIndexTypeMapping[] =
		{
			GL_INVALID_ENUM,	// IndexType_Invalid
			GL_UNSIGNED_BYTE,	// IndexType_UInt8
			GL_UNSIGNED_SHORT,	// IndexType_UInt16
			GL_INVALID_ENUM,
			GL_UNSIGNED_INT		// IndexType_UInt32
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aIndexTypeMapping ) == IndexType_MaxSize + 1u );
		m_platformData.indexType = s_aIndexTypeMapping[ indexBuffer.m_indexType ];

		TIKI_CHECK_GRAPHICS;
	}

	void GraphicsContext::setVertexBuffer( uint slot, const VertexBuffer& buffer )
	{
		TIKI_ASSERT( buffer.m_bufferId != GL_INVALID_ENUM );
		TIKI_ASSERT( buffer.m_bufferType == GL_ARRAY_BUFFER );

		glBindBuffer( GL_ARRAY_BUFFER, buffer.m_bufferId );

		TIKI_CHECK_GRAPHICS;
	}

	void* GraphicsContext::beginImmediateGeometry( uint vertexStride, uint vertexCount )
	{
		TIKI_ASSERT( vertexStride * vertexCount <= MaxImmediateGeometrySize );

		m_immediateVertexStride	= vertexStride;
		m_immediateVertexCount	= vertexCount;

		void* pData = mapBuffer( m_immediateVertexData );
		TIKI_ASSERT( pData != nullptr );
		return pData;
	}

	void GraphicsContext::endImmediateGeometry()
	{
		unmapBuffer( m_immediateVertexData );
		TIKI_ASSERT( validateDrawCall() );

		//const UINT offset = 0u;
		//const UINT vertexStride = static_cast< UINT >( m_immediateVertexStride );
		//m_platformData.pContext->IASetVertexBuffers( 0u, 1u, &m_immediateVertexData.m_pBuffer, &vertexStride, &offset );
		glBindBuffer( GL_ARRAY_BUFFER, m_immediateVertexData.m_bufferId );

		graphics::setLinkedShaderProgram( *m_pGraphicsSystem, m_platformData.pixelShaderId, m_platformData.vertexShaderId );
		glDrawArrays( m_platformData.primitiveTopology, 0u, m_immediateVertexCount );

		TIKI_CHECK_GRAPHICS;
	}

	void GraphicsContext::drawGeometry( uint vertexCount, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );

		graphics::setLinkedShaderProgram( *m_pGraphicsSystem, m_platformData.pixelShaderId, m_platformData.vertexShaderId );
		glDrawArrays( m_platformData.primitiveTopology, baseVertexOffset, vertexCount );

		TIKI_CHECK_GRAPHICS;
	}

	void GraphicsContext::drawIndexedGeometry( uint indexCount, uint baseIndexOffset /*= 0u*/, uint baseVertexOffset /*= 0u*/ )
	{
		TIKI_ASSERT( validateDrawCall() );

		graphics::setLinkedShaderProgram( *m_pGraphicsSystem, m_platformData.pixelShaderId, m_platformData.vertexShaderId );
		glDrawElementsBaseVertex( m_platformData.primitiveTopology, indexCount, m_platformData.indexType, nullptr, baseVertexOffset );

		TIKI_CHECK_GRAPHICS;
	}

	void* GraphicsContext::mapBuffer( const BaseBuffer& buffer )
	{
		TIKI_ASSERT( buffer.m_bufferId != GL_INVALID_ENUM );

		glBindBuffer( buffer.m_bufferType, buffer.m_bufferId );
		void* pData = glMapBuffer( buffer.m_bufferType, GL_WRITE_ONLY );
		TIKI_CHECK_GRAPHICS;

		return pData;
	}

	void GraphicsContext::unmapBuffer( const BaseBuffer& buffer )
	{
		glUnmapBuffer( buffer.m_bufferType );
		glBindBuffer( buffer.m_bufferType, 0u );

		TIKI_CHECK_GRAPHICS;
	}

	const RenderTarget& GraphicsContext::getBackBuffer() const
	{
		return m_pGraphicsSystem->getBackBuffer();
	}

	static void graphics::setGlMode( GLenum mode, GLboolean enable )
	{
		( enable ? glEnable : glDisable )( mode );
	}

	void graphics::setLinkedShaderProgram( GraphicsSystem& graphicsSystem, GLuint vertexShaderId, GLuint pixelShaderId )
	{
		ShaderLinker& shaderLinker = graphics::getShaderLinker( graphicsSystem );

		GLuint programId = shaderLinker.findOrCreateProgram( vertexShaderId, pixelShaderId );
		if ( programId != 0u )
		{
			glUseProgram( programId );
		}
	}
}