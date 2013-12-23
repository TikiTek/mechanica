
#include "tiki/graphicsresources/shaderset.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/resource/resourcefile.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct ShaderSetVariantData
	{
		uint16			shaderType;
		uint16			codeLength;
		crc32			variantKey;
		ResRef< void >	data;
	};

	struct ShaderSetInitializationData
	{
		uint32					variantCount;
		ShaderSetVariantData	variants[ 1u ]; // count is variantCount
	};

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
		TIKI_ASSERT( pShader == nullptr || pShader->getShaderType() == type );

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
		const ShaderSetFactoryContext& context = *static_cast< const ShaderSetFactoryContext* >( &factoryContext );
		const ShaderSetInitializationData* pInitData = static_cast< const ShaderSetInitializationData* >( initData.pData );

		m_shaders.create( pInitData->variantCount );
		for (uint i = 0u; i < pInitData->variantCount; ++i)
		{
			const ShaderSetVariantData& variant = pInitData->variants[ i ];
			const Shader* pShader = context.graphicsSystem.createShader( (ShaderType)variant.shaderType, variant.data.getData(), variant.codeLength );

			if ( pShader != nullptr )
			{
				m_shaders.set( variant.variantKey, pShader );
			}
		} 

		return true;
	}

	void ShaderSet::disposeInternal( const FactoryContext& factoryContext )
	{
		const ShaderSetFactoryContext& context = *static_cast< const ShaderSetFactoryContext* >( &factoryContext );

		const Shader* pShader = nullptr;
		for (uint i = 0u; i < m_shaders.getCount(); ++i)
		{
			m_shaders.getValueAt( &pShader, i );

			context.graphicsSystem.disposeShader( pShader );
		} 

		m_shaders.dispose();
	}

}