
#include "tiki/graphics/graphicscontext.hpp"

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
}