
#include "tiki/framework/frameworkfactories.hpp"

#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	FrameworkFactories::FrameworkFactories()
	{
		m_pResourceManager = nullptr;
	}

	FrameworkFactories::~FrameworkFactories()
	{
		TIKI_ASSERT( m_pResourceManager == nullptr );
	}

	void FrameworkFactories::create( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
	{
		TIKI_ASSERT( m_pResourceManager == nullptr );

		m_pResourceManager	= &resourceManager;

		m_fontFactory.create( graphicsSystem );
		m_materialFactory.create();
		m_modelFactory.create( &graphicsSystem );
		m_textureFactory.create( graphicsSystem );
		m_shaderSetFactory.create( graphicsSystem );
		
		m_pResourceManager->registerFactory( m_fontFactory );
		m_pResourceManager->registerFactory( m_materialFactory );
		m_pResourceManager->registerFactory( m_modelFactory );
		m_pResourceManager->registerFactory( m_textureFactory );
		m_pResourceManager->registerFactory( m_shaderSetFactory );
	}

	void FrameworkFactories::dispose()
	{
		if ( m_pResourceManager == nullptr )
		{
			return;
		}

		m_pResourceManager->unregisterFactory( m_fontFactory );
		m_pResourceManager->unregisterFactory( m_materialFactory );
		m_pResourceManager->unregisterFactory( m_modelFactory );
		m_pResourceManager->unregisterFactory( m_textureFactory );
		m_pResourceManager->unregisterFactory( m_shaderSetFactory );

		m_fontFactory.dispose();
		m_materialFactory.dispose();
		m_modelFactory.dispose();
		m_textureFactory.dispose();
		m_shaderSetFactory.dispose();

		m_pResourceManager = nullptr;
	}
}