
#include "tiki/graphics/rendertarget.hpp"

#include "tiki/graphics/texturedata.hpp"

#include "graphicssystem_internal_opengl4.hpp"

namespace tiki
{
	static struct { GLenum status; const char* pString; } s_aStatusStrings[] =
	{
		{ GL_FRAMEBUFFER_UNDEFINED,							"GL_FRAMEBUFFER_UNDEFINED"						},
		{ GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,				"GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"			},
		{ GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,		"GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"	},
		{ GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,			"GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"			},
		{ GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,			"GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"			},
		{ GL_FRAMEBUFFER_UNSUPPORTED,						"GL_FRAMEBUFFER_UNSUPPORTED"					},
		{ GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,			"GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"			},
		{ GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,			"GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"		}
	};

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
	}

	RenderTarget::~RenderTarget()
	{
		TIKI_ASSERT( m_platformData.frameBufferId == 0u );
	}

	bool RenderTarget::create( GraphicsSystem& graphicsSystem, size_t width, size_t height, const RenderTargetBuffer* pColorBuffers, size_t colorBufferCount, const RenderTargetBuffer* pDepthBuffer )
	{
		TIKI_ASSERT( colorBufferCount <= GraphicsSystemLimits_RenderTargetSlots );

		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT0  == GL_COLOR_ATTACHMENT0 + 0u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT1  == GL_COLOR_ATTACHMENT0 + 1u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT2  == GL_COLOR_ATTACHMENT0 + 2u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT3  == GL_COLOR_ATTACHMENT0 + 3u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT4  == GL_COLOR_ATTACHMENT0 + 4u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT5  == GL_COLOR_ATTACHMENT0 + 5u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT6  == GL_COLOR_ATTACHMENT0 + 6u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT7  == GL_COLOR_ATTACHMENT0 + 7u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT8  == GL_COLOR_ATTACHMENT0 + 8u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT9  == GL_COLOR_ATTACHMENT0 + 9u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT10 == GL_COLOR_ATTACHMENT0 + 10u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT11 == GL_COLOR_ATTACHMENT0 + 11u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT12 == GL_COLOR_ATTACHMENT0 + 12u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT13 == GL_COLOR_ATTACHMENT0 + 13u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT14 == GL_COLOR_ATTACHMENT0 + 14u );
		TIKI_COMPILETIME_ASSERT( GL_COLOR_ATTACHMENT15 == GL_COLOR_ATTACHMENT0 + 15u );

		m_colorBufferCount	= colorBufferCount;
		m_width				= width;
		m_height			= height;

		glGenFramebuffers( 1, &m_platformData.frameBufferId );
		glBindFramebuffer( GL_FRAMEBUFFER, m_platformData.frameBufferId );

		TIKI_ASSERT( pColorBuffers != nullptr || colorBufferCount == 0u );
		for (size_t i = 0u; i < m_colorBufferCount; ++i)
		{
			TIKI_ASSERT( pColorBuffers[ i ].pDataBuffer != nullptr );
			m_colorBuffers[ i ] = pColorBuffers[ i ];

			checkSize( m_width, m_height, pColorBuffers[ i ].pDataBuffer->getWidth(), pColorBuffers[ i ].pDataBuffer->getHeight() );

			const GLuint colorBufferId = pColorBuffers[ i ].pDataBuffer->m_platformData.textureId;
			m_platformData.aColorBufferIds[ i ] = colorBufferId;
			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBufferId, 0 );
		}

		for (size_t i = m_colorBufferCount; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].clear();
			m_platformData.aColorBufferIds[ i ] = 0u;
		}

		if ( pDepthBuffer != nullptr )
		{
			TIKI_ASSERT( pDepthBuffer->pDataBuffer != nullptr );
			m_depthBuffer = *pDepthBuffer;

			checkSize( m_width, m_height, pDepthBuffer->pDataBuffer->getWidth(), pDepthBuffer->pDataBuffer->getHeight() );

			const GLuint depthBufferId = pDepthBuffer->pDataBuffer->m_platformData.textureId;
			m_platformData.depthBufferId = depthBufferId;
			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferId, 0 );
			glFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBufferId, 0 );
		}
		else
		{
			m_depthBuffer.clear();
			m_platformData.depthBufferId = 0u;
		}

		const GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
		if ( status != GL_FRAMEBUFFER_COMPLETE )
		{
			uint statusIndex = 0u;
			for (uint i = 0u; i < TIKI_COUNT( s_aStatusStrings ); ++i)
			{
				if ( s_aStatusStrings[ i ].status == status )
				{
					statusIndex = i;
					break;
				}
			} 

			TIKI_TRACE_ERROR( "[graphics] Could not create RenderTarget. Status: %s\n", s_aStatusStrings[ statusIndex ].pString );
			return false;
		}
		glBindFramebuffer( GL_FRAMEBUFFER, 0u );

		return true;
	}

	void RenderTarget::dispose( GraphicsSystem& graphicsSystem )
	{
		m_width		= 0u;
		m_height	= 0u;

		for (size_t i = 0u; i < TIKI_COUNT( m_colorBuffers ); ++i)
		{
			m_colorBuffers[ i ].clear();
		}
		m_colorBufferCount = 0u;

		m_depthBuffer.clear();

		if ( m_platformData.frameBufferId != 0u )
		{
			glDeleteFramebuffers( 1u, &m_platformData.frameBufferId );
			m_platformData.frameBufferId = 0u;
		}
	}
}
