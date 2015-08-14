
#include "tiki/particle/particleeffect.hpp"

#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct ParticleEffectFactoryContext : public FactoryContextGenericBase< ParticleEffect >
	{
	};

	struct ParticleEffectInitData
	{
		ParticleEffectData* pData;
	};

	void ParticleEffect::registerResourceType( ResourceManager& resourceManager )
	{
		static ParticleEffectFactoryContext context;
		resourceManager.registerResourceType( s_resourceType, context );
	}

	void ParticleEffect::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	bool ParticleEffect::createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		if ( initData.size != sizeof( ParticleEffectInitData ) )
		{
			TIKI_TRACE_ERROR( "[ParticleEffect::createInternal] Invalid init data size(%i != %i)", initData.size, sizeof( ParticleEffectInitData ) );
			return false;
		}

		const ParticleEffectInitData* pInitData = static_cast< const ParticleEffectInitData* >( initData.pData );
		if ( pInitData != nullptr )
		{
			m_pData = pInitData->pData;

			return m_pData != nullptr;
		}

		return false;
	}

	void ParticleEffect::disposeInternal( const FactoryContext& factoryContext )
	{
		m_pData = nullptr;
	}

	ParticleEffect::ParticleEffect()
	{
		m_pData = nullptr;
	}

	ParticleEffect::~ParticleEffect()
	{
		TIKI_ASSERT( m_pData == nullptr );
	}
}