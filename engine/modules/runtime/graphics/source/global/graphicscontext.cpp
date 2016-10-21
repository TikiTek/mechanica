
#include "tiki/graphics/graphicscontext.hpp"

#include "tiki/graphics/stockvertex.hpp"

namespace tiki
{
	void GraphicsContext::invalidateState()
	{
		m_primitiveTopology		= PrimitiveTopology_Count;

		m_pBlendState			= nullptr;
		m_pDepthStencilState	= nullptr;
		m_pRasterizerState		= nullptr;

		m_pVertexShader			= nullptr;
		m_pVertexInputBinding	= nullptr;

		for (size_t i = 0u; i < TIKI_COUNT( m_apVertexSamplerStates ); ++i)
		{
			m_apVertexSamplerStates[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_apVertexTextures ); ++i)
		{
			m_apVertexTextures[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_apVertexConstants ); ++i)
		{
			m_apVertexConstants[ i ] = nullptr;
		}

		m_pPixelShader = nullptr;

		for (size_t i = 0u; i < TIKI_COUNT( m_apPixelSamplerStates ); ++i)
		{
			m_apPixelSamplerStates[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_apPixelTextures ); ++i)
		{
			m_apPixelTextures[ i ] = nullptr;
		}

		for (size_t i = 0u; i < TIKI_COUNT( m_apPixelConstants ); ++i)
		{
			m_apPixelConstants[ i ] = nullptr;
		}
	}
	
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	bool GraphicsContext::validateDrawCall() const
	{
#define TIKI_VALIDATE_EXPR( expr, message ) if ( !( result &= ( expr ) ) ) TIKI_TRACE_ERROR( "[grpahics] validateDrawCall failed: %s\n", message )
		bool result = true;

		TIKI_VALIDATE_EXPR( m_primitiveTopology != PrimitiveTopology_Count,	"PrimitiveTopology must be set" );

		TIKI_VALIDATE_EXPR( m_pBlendState != nullptr,						"BlendState must be set" );
		TIKI_VALIDATE_EXPR( m_pDepthStencilState != nullptr,				"DepthStencilState must be set" );
		TIKI_VALIDATE_EXPR( m_pRasterizerState != nullptr,					"RasterizerState must be set" );
		TIKI_VALIDATE_EXPR( m_pVertexInputBinding != nullptr,				"VertexInputBinding must be set" );

		TIKI_ASSERT( result );
		return result;
#undef TIKI_VALIDATE_EXPR
	}
#endif

	void GraphicsContext::drawFullScreenQuadPos2()
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos2* pVertices = static_cast< StockVertexPos2* >( beginImmediateGeometry( sizeof( StockVertexPos2 ), 4u ) );

		static const StockVertexPos2 s_aVertices[] =
		{
			{ -1.0f, -1.0f },
			{ -1.0f,  1.0f },
			{  1.0f, -1.0f },
			{  1.0f,  1.0f }
		};
		pVertices[ 0u ] = s_aVertices[ 0u ];
		pVertices[ 1u ] = s_aVertices[ 1u ];
		pVertices[ 2u ] = s_aVertices[ 2u ];
		pVertices[ 3u ] = s_aVertices[ 3u ];

		endImmediateGeometry();
	}

	void GraphicsContext::drawFullScreenQuadPos2Tex2( float u0 /* = 0.0f */, float v0 /* = 0.0f */, float u1 /* = 1.0f */, float v1 /* = 1.0f */ )
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos2Tex2* pVertices = static_cast< StockVertexPos2Tex2* >( beginImmediateGeometry( sizeof( StockVertexPos2Tex2 ), 4u ) );
		
		createFloat2( pVertices[ 0u ].position, -1.0f, -1.0f );
		createFloat2( pVertices[ 0u ].texCoord, u0, v1 );

		createFloat2( pVertices[ 1u ].position, -1.0f, 1.0f );
		createFloat2( pVertices[ 1u ].texCoord, u0, v0 );

		createFloat2( pVertices[ 2u ].position, 1.0f, -1.0f );
		createFloat2( pVertices[ 2u ].texCoord, u1, v1 );

		createFloat2( pVertices[ 3u ].position, 1.0f, 1.0f );
		createFloat2( pVertices[ 3u ].texCoord, u1, v0 );

		endImmediateGeometry();
	}

	void GraphicsContext::drawFullScreenQuadPos3( float z /* = 0.0f */ )
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos3* pVertices = static_cast< StockVertexPos3* >( beginImmediateGeometry( sizeof( StockVertexPos3 ), 4u ) );

		static const StockVertexPos3 s_aVertices[] =
		{
			{ -1.0f, -1.0f, z },
			{ -1.0f,  1.0f, z },
			{  1.0f, -1.0f, z },
			{  1.0f,  1.0f, z }
		};
		pVertices[ 0u ] = s_aVertices[ 0u ];
		pVertices[ 1u ] = s_aVertices[ 1u ];
		pVertices[ 2u ] = s_aVertices[ 2u ];
		pVertices[ 3u ] = s_aVertices[ 3u ];

		endImmediateGeometry();
	}

	void GraphicsContext::drawFullScreenQuadPos3Tex2( float u0 /* = 0.0f */, float v0 /* = 0.0f */, float u1 /* = 1.0f */, float v1 /* = 1.0f */, float z /* = 0.0f */ )
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos3Tex2* pVertices = static_cast< StockVertexPos3Tex2* >( beginImmediateGeometry( sizeof( StockVertexPos3Tex2 ), 4u ) );

		createFloat3( pVertices[ 0u ].position, -1.0f, -1.0f, z );
		createFloat2( pVertices[ 0u ].texCoord, u0, v1 );

		createFloat3( pVertices[ 1u ].position, -1.0f, 1.0f, z );
		createFloat2( pVertices[ 1u ].texCoord, u0, v0 );

		createFloat3( pVertices[ 2u ].position, 1.0f, -1.0f, z );
		createFloat2( pVertices[ 2u ].texCoord, u1, v1 );

		createFloat3( pVertices[ 3u ].position, 1.0f, 1.0f, z );
		createFloat2( pVertices[ 3u ].texCoord, u1, v0 );

		endImmediateGeometry();
	}
}