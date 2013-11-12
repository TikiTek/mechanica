
#include "tiki/base/crc32.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	Resource::Resource()
	{
		m_references = 1u;
	}

	Resource::~Resource()
	{
		TIKI_ASSERT( m_references == 0u );
	}

	void Resource::initialize( ResourceId id )
	{
		m_id = id;
	}

	void Resource::addReference() const
	{
		++m_references;
	}

	bool Resource::releaseReference() const
	{
		return --m_references == 0u;
	}
}