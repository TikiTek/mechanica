
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

	void Resource::initialize( const ResourceId& id, const ResourceSectorData& sectorData )
	{
		m_id				= id;
		m_sectorData		= sectorData;
		m_referenceCount	= 1u;
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