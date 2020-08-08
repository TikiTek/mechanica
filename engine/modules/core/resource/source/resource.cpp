
#include "tiki/base/crc32.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	Resource::Resource()
	{
		m_referenceCount = 1u;
	}

	Resource::~Resource()
	{
		TIKI_ASSERT( m_referenceCount == 0u );
	}

	bool Resource::create( const ResourceName& name, const ResourceSectionData& sectionData, const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		m_name				= name;
		m_sectionData		= sectionData;

		return createInternal( initData, factoryContext );
	}

	void Resource::dispose( const FactoryContext& factoryContext )
	{
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