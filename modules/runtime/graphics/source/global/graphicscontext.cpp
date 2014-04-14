
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

		return result;
#undef TIKI_VALIDATE_EXPR
	}
#endif

	void GraphicsContext::drawFullScreenQuadPos2()
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos2* pVertices = static_cast< StockVertexPos2* >( beginImmediateGeometry( sizeof( StockVertexPos2 ), 4u ) );

		createFloat2( pVertices[ 0u ].position, -1.0f, -1.0f );
		createFloat2( pVertices[ 1u ].position, -1.0f, 1.0f );
		createFloat2( pVertices[ 2u ].position, 1.0f, -1.0f );
		createFloat2( pVertices[ 3u ].position, 1.0f, 1.0f );

		endImmediateGeometry();
	}

	void GraphicsContext::drawFullScreenQuadPos2Tex2( float x0 /*= 0.0f*/, float y0 /*= 0.0f*/, float x1 /*= 1.0f*/, float y1 /*= 1.0f*/ )
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos2Tex2* pVertices = static_cast< StockVertexPos2Tex2* >( beginImmediateGeometry( sizeof( StockVertexPos2Tex2 ), 4u ) );
		
		createFloat2( pVertices[ 0u ].position, -1.0f, -1.0f );
		createFloat2( pVertices[ 0u ].texCoord, x0, y1 );

		createFloat2( pVertices[ 1u ].position, -1.0f, 1.0f );
		createFloat2( pVertices[ 1u ].texCoord, x0, y0 );

		createFloat2( pVertices[ 2u ].position, 1.0f, -1.0f );
		createFloat2( pVertices[ 2u ].texCoord, x1, y1 );

		createFloat2( pVertices[ 3u ].position, 1.0f, 1.0f );
		createFloat2( pVertices[ 3u ].texCoord, x1, y0 );

		endImmediateGeometry();
	}

	void GraphicsContext::drawFullScreenQuadPos3()
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos3* pVertices = static_cast< StockVertexPos3* >( beginImmediateGeometry( sizeof( StockVertexPos3 ), 4u ) );

		createFloat3( pVertices[ 0u ].position, -1.0f, -1.0f, 0.0f );
		createFloat3( pVertices[ 1u ].position, -1.0f, 1.0f, 0.0f );
		createFloat3( pVertices[ 2u ].position, 1.0f, -1.0f, 0.0f );
		createFloat3( pVertices[ 3u ].position, 1.0f, 1.0f, 0.0f );

		endImmediateGeometry();
	}

	void GraphicsContext::drawFullScreenQuadPos3Tex2( float x0 /*= 0.0f*/, float y0 /*= 0.0f*/, float x1 /*= 1.0f*/, float y1 /*= 1.0f */ )
	{
		setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		TIKI_ASSERT( validateDrawCall() );

		StockVertexPos3Tex2* pVertices = static_cast< StockVertexPos3Tex2* >( beginImmediateGeometry( sizeof( StockVertexPos3Tex2 ), 4u ) );

		createFloat3( pVertices[ 0u ].position, -1.0f, -1.0f, 0.0f );
		createFloat2( pVertices[ 0u ].texCoord, x0, y1 );

		createFloat3( pVertices[ 1u ].position, -1.0f, 1.0f, 0.0f );
		createFloat2( pVertices[ 1u ].texCoord, x0, y0 );

		createFloat3( pVertices[ 2u ].position, 1.0f, -1.0f, 0.0f );
		createFloat2( pVertices[ 2u ].texCoord, x1, y1 );

		createFloat3( pVertices[ 3u ].position, 1.0f, 1.0f, 0.0f );
		createFloat2( pVertices[ 3u ].texCoord, x1, y0 );

		endImmediateGeometry();
	}

}