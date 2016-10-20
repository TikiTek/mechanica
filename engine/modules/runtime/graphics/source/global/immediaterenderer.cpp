
#include "tiki/graphics/immediaterenderer.hpp"

#include "tiki/base/numbers.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/primitivetopologies.hpp"
#include "tiki/graphics/shader/immediaterenderer_shader.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/math/axisalignedrectangle.hpp"
#include "tiki/math/box.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	ImmediateRenderer::ImmediateRenderer()
	{
		m_pContext = nullptr;

		m_pShaderSet = nullptr;

		m_pBlendState[ ImmediateBlendState_None ] = nullptr;
		m_pBlendState[ ImmediateBlendState_Add ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ] = nullptr;
		m_pRasterizerState		= nullptr;
		m_pSamplerState			= nullptr;

		m_pVertexFormat			= nullptr;
		m_pVertexInputBinding	= nullptr;
	}

	ImmediateRenderer::~ImmediateRenderer()
	{
		TIKI_ASSERT( m_pContext == nullptr );
		TIKI_ASSERT( m_pShaderSet == nullptr );

		TIKI_ASSERT( m_pBlendState[ ImmediateBlendState_None ]						== nullptr );
		TIKI_ASSERT( m_pBlendState[ ImmediateBlendState_Add ]						== nullptr );
		TIKI_ASSERT( m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ]	== nullptr );
		TIKI_ASSERT( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ]		== nullptr );
		TIKI_ASSERT( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ]		== nullptr );
		TIKI_ASSERT( m_pRasterizerState		== nullptr );
		TIKI_ASSERT( m_pSamplerState		== nullptr );

		TIKI_ASSERT( m_pVertexFormat		== nullptr );
		TIKI_ASSERT( m_pVertexInputBinding	== nullptr );
	}

	bool ImmediateRenderer::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_pShaderSet = resourceManager.loadResource< ShaderSet >( "immediate.shader" );
		if ( m_pShaderSet == nullptr )
		{
			return false;
		}

		{
			const VertexAttribute attributes[] =
			{
				{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32z32_float,	0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	0u,	VertexAttributeFormat_x16y16_unorm,		0u, VertexInputType_PerVertex },
				{ VertexSementic_Color,		0u,	VertexAttributeFormat_x8y8z8w8_unorm,	0u, VertexInputType_PerVertex }
			};		

			m_pVertexFormat = graphicsSystem.createVertexFormat( attributes, TIKI_COUNT( attributes ) );
			if ( m_pVertexFormat == nullptr )
			{
				dispose( graphicsSystem, resourceManager );
				return false;
			}

			m_pVertexInputBinding = graphicsSystem.createVertexInputBinding( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ), m_pVertexFormat );
			if ( m_pVertexInputBinding == nullptr )
			{
				dispose( graphicsSystem, resourceManager );
				return false;
			}
		}

		m_pBlendState[ ImmediateBlendState_None ]	= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pBlendState[ ImmediateBlendState_Add ]	= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );

		m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ]	= graphicsSystem.createDepthStencilState( false, false );
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ]	= graphicsSystem.createDepthStencilState( true, false );
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ]	= graphicsSystem.createDepthStencilState( true, true );

		m_pRasterizerState		= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerState			= graphicsSystem.createSamplerState( SamplerStateParamters() );

		if ( m_vertexConstantBuffer.create( graphicsSystem, sizeof( ImmediateRendererConstantData ), "ImmediateVertexConstants" ) == false )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}
		
		return true;
	}

	void ImmediateRenderer::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_pContext == nullptr );

		resourceManager.unloadResource( m_pShaderSet );
		m_pShaderSet = nullptr;

		graphicsSystem.disposeBlendState( m_pBlendState[ ImmediateBlendState_None ] );
		graphicsSystem.disposeBlendState( m_pBlendState[ ImmediateBlendState_Add ] );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ] );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ] );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ] );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeSamplerState( m_pSamplerState );
		m_pBlendState[ ImmediateBlendState_None ] = nullptr;
		m_pBlendState[ ImmediateBlendState_Add ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ] = nullptr;
		m_pRasterizerState = nullptr;
		m_pSamplerState = nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );
		m_pVertexInputBinding = nullptr;

		graphicsSystem.disposeVertexFormat( m_pVertexFormat );
		m_pVertexFormat = nullptr;

		m_vertexConstantBuffer.dispose( graphicsSystem );
	}

	void ImmediateRenderer::beginRendering( GraphicsContext& graphicsContext )
	{
		TIKI_ASSERT( m_pContext == nullptr );
		m_pContext = &graphicsContext;
	}

	void ImmediateRenderer::endRendering()
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_pContext = nullptr;
	}

	void ImmediateRenderer::beginRenderPass( const RenderTarget* pRenderTarget /*= nullptr*/, const Camera* pCamera /*= nullptr*/ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		if ( pRenderTarget == nullptr )
		{
			pRenderTarget = &m_pContext->getBackBuffer();
		}
		m_pContext->beginRenderPass( *pRenderTarget );
			
		Matrix44 transformMatrix;
		if ( pCamera == nullptr )
		{
			Projection projection;
			projection.createOrthographicCenter(
				(float)pRenderTarget->getWidth(),
				-(float)pRenderTarget->getHeight(),
				0.0f,
				1.0f
			);
			transformMatrix = projection.getMatrix();
		}
		else
		{
			transformMatrix = pCamera->getViewProjectionMatrix();
		}

		ImmediateRendererConstantData* pConstantData = static_cast< ImmediateRendererConstantData* >( m_pContext->mapBuffer( m_vertexConstantBuffer ) );
		TIKI_ASSERT( pConstantData != nullptr );
		createGraphicsMatrix44( pConstantData->projection, transformMatrix );
		m_pContext->unmapBuffer( m_vertexConstantBuffer );

		setState();
	}

	void ImmediateRenderer::endRenderPass() const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_pContext->endRenderPass();
	}

	void ImmediateRenderer::setBlendState( ImmediateBlendState blendState ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		TIKI_ASSERT( blendState < ImmediateBlendState_Count );
		m_pContext->setBlendState( m_pBlendState[ blendState ] );
	}

	void ImmediateRenderer::setDepthState( ImmediateDepthState depthState ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		TIKI_ASSERT( depthState < ImmediateDepthState_Count );
		m_pContext->setDepthStencilState( m_pDepthStencilState[ depthState ] );
	}

	void ImmediateRenderer::setPrimitiveTopology( PrimitiveTopology topology ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		TIKI_ASSERT( topology < PrimitiveTopology_Count );
		m_pContext->setPrimitiveTopology( topology );
	}

	void ImmediateRenderer::setShaderMode( ImmediateShaderMode shaderMode ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, shaderMode ) );
	}

	void ImmediateRenderer::drawRectangle( const AxisAlignedRectangle& destinationCoodinates, Color color /* = TIKI_COLOR_WHITE */ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );
		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, 4u );

		// bottom left
		createFloat3( vertices[ 0u ].position, destinationCoodinates.getLeft(), destinationCoodinates.getBottom(), 0.0f );
		vertices[ 0u ].u			= 0u;
		vertices[ 0u ].v			= 0u;
		vertices[ 0u ].color		= color;

		// top left
		createFloat3( vertices[ 1u ].position, destinationCoodinates.getLeft(), destinationCoodinates.getTop(), 0.0f );
		vertices[ 1u ].u			= 0u;
		vertices[ 1u ].v			= 0u;
		vertices[ 1u ].color		= color;

		// bottom right
		createFloat3( vertices[ 2u ].position, destinationCoodinates.getRight(), destinationCoodinates.getBottom(), 0.0f );
		vertices[ 2u ].u			= 0u;
		vertices[ 2u ].v			= 0u;
		vertices[ 2u ].color		= color;

		// top right
		createFloat3( vertices[ 3u ].position, destinationCoodinates.getRight(), destinationCoodinates.getTop(), 0.0f );
		vertices[ 3u ].u			= 0u;
		vertices[ 3u ].v			= 0u;
		vertices[ 3u ].color		= color;

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawTexturedRectangle( const TextureData& texture, const AxisAlignedRectangle& destinationCoodinates, Color color /* = TIKI_COLOR_WHITE */ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );
		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );
		m_pContext->setPixelShaderTexture( 0u, &texture );

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, 4u );

		// bottom left
		createFloat3( vertices[ 0u ].position, destinationCoodinates.getLeft(), destinationCoodinates.getBottom(), 0.0f );
		vertices[ 0u ].u			= u16::floatToUnorm( 0.0f );
		vertices[ 0u ].v			= u16::floatToUnorm( 1.0f );
		vertices[ 0u ].color		= color;

		// top left
		createFloat3( vertices[ 1u ].position, destinationCoodinates.getLeft(), destinationCoodinates.getTop(), 0.0f );
		vertices[ 1u ].u			= u16::floatToUnorm( 0.0f );
		vertices[ 1u ].v			= u16::floatToUnorm( 0.0f );
		vertices[ 1u ].color		= color;

		// bottom right
		createFloat3( vertices[ 2u ].position, destinationCoodinates.getRight(), destinationCoodinates.getBottom(), 0.0f );
		vertices[ 2u ].u			= u16::floatToUnorm( 1.0f );
		vertices[ 2u ].v			= u16::floatToUnorm( 1.0f );
		vertices[ 2u ].color		= color;

		// top right
		createFloat3( vertices[ 3u ].position, destinationCoodinates.getRight(), destinationCoodinates.getTop(), 0.0f );
		vertices[ 3u ].u			= u16::floatToUnorm( 1.0f );
		vertices[ 3u ].v			= u16::floatToUnorm( 0.0f );
		vertices[ 3u ].color		= color;

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawTexturedRectangle( const TextureData& texture, const AxisAlignedRectangle& destinationCoodinates, const AxisAlignedRectangle& sourceCoodinates, Color color /* = TIKI_COLOR_WHITE */ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		const float uScale = 1.0f / (float)texture.getWidth();
		const float vScale = 1.0f / (float)texture.getHeight();

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );
		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );
		m_pContext->setPixelShaderTexture( 0u, &texture );

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, 4u );

		// bottom left
		createFloat3( vertices[ 0u ].position, destinationCoodinates.getLeft(), destinationCoodinates.getBottom(), 0.0f );
		vertices[ 0u ].u			= u16::floatToUnorm( sourceCoodinates.getLeft() * uScale );
		vertices[ 0u ].v			= u16::floatToUnorm( sourceCoodinates.getBottom() * vScale );
		vertices[ 0u ].color		= color;

		// top left
		createFloat3( vertices[ 1u ].position, destinationCoodinates.getLeft(), destinationCoodinates.getTop(), 0.0f );
		vertices[ 1u ].u			= u16::floatToUnorm( sourceCoodinates.getLeft() * uScale );
		vertices[ 1u ].v			= u16::floatToUnorm( sourceCoodinates.getTop() * vScale );
		vertices[ 1u ].color		= color;

		// bottom right
		createFloat3( vertices[ 2u ].position, destinationCoodinates.getRight(), destinationCoodinates.getBottom(), 0.0f );
		vertices[ 2u ].u			= u16::floatToUnorm( sourceCoodinates.getRight() * uScale );
		vertices[ 2u ].v			= u16::floatToUnorm( sourceCoodinates.getBottom() * vScale );
		vertices[ 2u ].color		= color;

		// top right
		createFloat3( vertices[ 3u ].position, destinationCoodinates.getRight(), destinationCoodinates.getTop(), 0.0f );
		vertices[ 3u ].u			= u16::floatToUnorm( sourceCoodinates.getRight() * uScale );
		vertices[ 3u ].v			= u16::floatToUnorm( sourceCoodinates.getTop() * vScale );
		vertices[ 3u ].color		= color;

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawText( const Vector2& position, const Font& font, const char* pText, Color color /*= TIKI_COLOR_WHITE*/ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		const uint textLength = getStringSize( pText );
		if ( textLength == 0u )
		{
			return;
		}

		TIKI_ASSERT( textLength <= 128u );
		FontChar chars[ 128u ];
		font.fillVertices( chars, TIKI_COUNT( chars ), pText, textLength );

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );
		setShaderMode( ImmediateShaderMode_Font );

		m_pContext->setPixelShaderTexture( 0u, &font.getTextureData() );

		const size_t vertexCount = textLength * 4u;

		StaticArray< ImmediateVertex > vertices;
		m_pContext->beginImmediateGeometry( vertices, vertexCount );

		float x = 0.0f;
		for (size_t charIndex = 0u; charIndex < textLength; charIndex++)
		{
			const size_t vertexIndex = charIndex * 4u;
			const FontChar& character = chars[ charIndex ];

			const float charWidth	= character.width;
			const float charHeight	= character.height;

			const float posLeft		= position.x + x;
			const float posRight	= posLeft + charWidth;
			const float posTop		= position.y;
			const float posBottom	= position.y + charHeight;

			// bottom left
			createFloat3( vertices[ vertexIndex + 0u ].position, posLeft, posBottom, 0.0f );
			vertices[ vertexIndex + 0u ].u		= character.x1; 
			vertices[ vertexIndex + 0u ].v		= character.y2; 
			vertices[ vertexIndex + 0u ].color	= color;

			// top left
			createFloat3( vertices[ vertexIndex + 1u ].position, posLeft, posTop, 0.0f );
			vertices[ vertexIndex + 1u ].u		= character.x1; 
			vertices[ vertexIndex + 1u ].v		= character.y1; 
			vertices[ vertexIndex + 1u ].color	= color;

			// bottom right
			createFloat3( vertices[ vertexIndex + 2u ].position, posRight, posBottom, 0.0f );
			vertices[ vertexIndex + 2u ].u		= character.x2; 
			vertices[ vertexIndex + 2u ].v		= character.y2; 
			vertices[ vertexIndex + 2u ].color	= color;

			// top right
			createFloat3( vertices[ vertexIndex + 3u ].position, posRight, posTop, 0.0f );
			vertices[ vertexIndex + 3u ].u		= character.x2; 
			vertices[ vertexIndex + 3u ].v		= character.y1; 
			vertices[ vertexIndex + 3u ].color	= color;
		
			x += charWidth;
		}

		vertices.dispose();

		m_pContext->endImmediateGeometry();
	}
	
	void ImmediateRenderer::beginImmediateGeometry( StaticArray< ImmediateVertex >& vertices, uint capacity ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		return m_pContext->beginImmediateGeometry( vertices, capacity );
	}

	void ImmediateRenderer::endImmediateGeometry( StaticArray< ImmediateVertex >& vertices ) const
	{
		m_pContext->endImmediateGeometry( vertices );
	}

	void ImmediateRenderer::setState() const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		m_pContext->setVertexShader( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ) );
		m_pContext->setVertexShaderConstant( 0u, m_vertexConstantBuffer );

		m_pContext->setRasterizerState( m_pRasterizerState );
		m_pContext->setVertexInputBinding( m_pVertexInputBinding );

		m_pContext->setPixelShaderSamplerState( 0u, m_pSamplerState );

		setBlendState( ImmediateBlendState_Add );
		setDepthState( ImmediateDepthState_TestOffWriteOff );
	}
}
