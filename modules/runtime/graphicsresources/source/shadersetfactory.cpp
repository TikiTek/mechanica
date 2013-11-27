
#include "tiki/graphicsresources/shadersetfactory.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/graphicsresources/shaderset.hpp"

namespace tiki
{
	ShaderSetFactory::ShaderSetFactory()
	{
		m_pGraphicsSystem = nullptr;
	}

	ShaderSetFactory::~ShaderSetFactory()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
	}

	void ShaderSetFactory::create( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem = &graphicsSystem;
	}

	void ShaderSetFactory::dispose()
	{
		TIKI_ASSERT( m_pGraphicsSystem != nullptr );
		m_pGraphicsSystem = nullptr;
	}

	fourcc ShaderSetFactory::getType() const
	{
		return ShaderSet::s_resourceType;
	}

	Resource* ShaderSetFactory::createResource() const
	{
		return TIKI_NEW ShaderSet();
	}

	bool ShaderSetFactory::initializeResource( Resource* pResource, const void* pInitData, uint initDataSize ) const
	{
		TIKI_ASSERT( pResource != nullptr );
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( m_pGraphicsSystem != nullptr );
		
		return static_cast< ShaderSet* >( pResource )->create( *m_pGraphicsSystem, pInitData, initDataSize );
	}

	void ShaderSetFactory::destroyResource( Resource* pResource ) const
	{
		TIKI_ASSERT( pResource );

		ShaderSet* pShader = static_cast< ShaderSet* >( pResource );
		pShader->dispose();
		TIKI_DEL pShader;
	}
}