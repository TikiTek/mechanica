
#include "tiki/components/component.hpp"

namespace tiki
{
	ComponentBase::ComponentBase()
	{
		m_pFirstComponentState	= nullptr;
		m_pLastComponentState	= nullptr;
	}

	ComponentBase::~ComponentBase()
	{
		m_pFirstComponentState	= nullptr;
		m_pLastComponentState	= nullptr;
	}
}