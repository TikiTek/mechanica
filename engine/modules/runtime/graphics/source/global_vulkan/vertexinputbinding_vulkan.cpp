
#include "tiki/graphics/vertexinputbinding.hpp"

#include "graphicssystem_internal_vulkan.hpp"

namespace tiki
{
	VertexInputBinding::VertexInputBinding()
	{
		m_pShader						= nullptr;
		m_pVertexFormat					= nullptr;
	}

	VertexInputBinding::~VertexInputBinding()
	{
		TIKI_ASSERT( m_pShader			== nullptr );
		TIKI_ASSERT( m_pVertexFormat	== nullptr );
	}

	bool VertexInputBinding::create( GraphicsSystem& graphicsSystem, const VertexInputBindingParameters& creationParameters )
	{
		return false;
	}

	void VertexInputBinding::dispose( GraphicsSystem& graphicsSystem )
	{
		m_pVertexFormat	= nullptr;
		m_pShader		= nullptr;
		GraphicsStateObject::dispose();
	}
}