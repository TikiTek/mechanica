
#include "tiki/graphics/graphicscontext.hpp"

namespace tiki
{
	void GraphicsContext::invalidateState()
	{
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

	bool GraphicsContext::validateDrawCall() const
	{
		return true;
	}
}