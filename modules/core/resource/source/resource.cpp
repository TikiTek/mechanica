
#include "tiki/base/crc32.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	Resource::Resource()
	{
		m_referenceCount = 0u;
	}

	Resource::~Resource()
	{
		TIKI_ASSERT( m_referenceCount == 0u );
	}

	bool Resource::create( const ResourceId& id, const ResourceSectorData& sectorData, const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		m_id				= id;
		m_sectorData		= sectorData;
		m_referenceCount	= 1u;

		return createInternal( initData, factoryContext );
	}

	void Resource::dispose( const FactoryContext& factoryContext )
	{
		m_referenceCount = 0u;
		disposeInternal( factoryContext );
	}

	void Resource::addReference() const
	{
		++m_referenceCount;
	}

	bool Resource::releaseReference() const
	{
		return --m_referenceCount == 0u;
	}
}