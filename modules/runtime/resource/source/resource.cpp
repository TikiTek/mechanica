
#include "tiki/base/crc32.hpp"
#include "tiki/resource/resource.hpp"

namespace tiki
{
	Resource::Resource()
	{
		m_id.crcName	= InvalidCrc32;
		m_references	= 0u;
	}

	Resource::~Resource()
	{		
	}

	void Resource::initialize( ResourceId id )
	{
		m_id	= id;
	}

	size_t Resource::addReference() const
	{
		return ++m_references;
	}

	size_t Resource::releaseReference() const
	{
		return --m_references;
	}
}