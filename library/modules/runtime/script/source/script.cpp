#include "tiki/script/script.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	Script::Script()
	{
		m_pTest = nullptr;
	}

	Script::~Script()
	{
		TIKI_ASSERT( m_pTest == nullptr );
	}

	bool Script::create( const char* pCode )
	{
		return true;
	}

	void Script::dispose()
	{

	}

}