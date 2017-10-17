#include "tiki/converterbase/resource_section_writer.hpp"

namespace tiki
{
	ResourceSectionWriter::ResourceSectionWriter()
	{
		m_pResourceWriter = nullptr;
	}

	ResourceSectionWriter::~ResourceSectionWriter()
	{
		TIKI_ASSERT( m_pResourceWriter == nullptr );
	}
}
