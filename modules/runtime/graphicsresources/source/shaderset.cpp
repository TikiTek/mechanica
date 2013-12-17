
#include "tiki/graphicsresources/shaderset.hpp"

#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct ShaderSetFactoryContext : public FactoryContextGenericBase< ShaderSet >
	{
		ShaderSetFactoryContext( GraphicsSystem& _graphicsSystem )
			: graphicsSystem( _graphicsSystem )
		{
		}

		GraphicsSystem& graphicsSystem;
	};

	void ShaderSet::registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
	{
		static ShaderSetFactoryContext context( graphicsSystem );
		resourceManager.registerResourceType( s_resourceType, context );
	}

	void ShaderSet::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	const Shader* ShaderSet::getShader( ShaderType type, crc32 variantKey ) const
	{
		const uint32 shaderData[] = { type, variantKey };
		const crc32 shaderCrc = crcBytes( shaderData, sizeof( shaderData ) );

		const Shader* pShader = nullptr;
		m_shaders.findValue( &pShader, shaderCrc );

		return pShader;
	}

	ShaderSet::ShaderSet()
	{
	}

	ShaderSet::~ShaderSet()
	{
	}

	bool ShaderSet::createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		return false;
	}

	void ShaderSet::disposeInternal( const FactoryContext& factoryContext )
	{
		m_shaders.dispose();
	}

}