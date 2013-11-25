
#include "tiki/graphicsresources/shaderfactory.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/graphics/shader.hpp"

namespace tiki
{
	ShaderFactory::ShaderFactory()
	{
		m_pGraphicsSystem = nullptr;
	}

	ShaderFactory::~ShaderFactory()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
	}

	void ShaderFactory::create( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem = &graphicsSystem;
	}

	void ShaderFactory::dispose()
	{
		TIKI_ASSERT( m_pGraphicsSystem != nullptr );
		m_pGraphicsSystem = nullptr;
	}

	fourcc ShaderFactory::getType() const
	{
		return Shader::s_resourceType;
	}

	Resource* ShaderFactory::createResource() const
	{
		return TIKI_NEW Shader();
	}

	bool ShaderFactory::initializeResource( Resource* pResource, const void* pInitData, uint initDataSize ) const
	{
		TIKI_ASSERT( pResource != nullptr );
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( m_pGraphicsSystem != nullptr );
		
		Shader* pShader = (Shader*)pResource;
		return pShader->create( *m_pGraphicsSystem, pInitData, initDataSize );
	}

	void ShaderFactory::destroyResource( Resource* pResource ) const
	{
		TIKI_ASSERT( pResource );

		Shader* pShader = (Shader*)pResource;
		pShader->dispose();

		TIKI_DEL pShader;
	}
}