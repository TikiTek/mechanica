
#include "tiki/components/component.hpp"

namespace tiki
{
	ComponentBase::ComponentBase()
	{
		m_pFirstComponentState	= nullptr;

		m_registedTypeId = InvalidComponentTypeId;
	}

	ComponentBase::~ComponentBase()
	{
		TIKI_ASSERT( m_registedTypeId == InvalidComponentTypeId );

		m_pFirstComponentState	= nullptr;
	}

	void ComponentBase::registerComponent( ComponentTypeId typeId )
	{
		TIKI_ASSERT( m_registedTypeId == InvalidComponentTypeId );
		m_registedTypeId = typeId;
	}

	void ComponentBase::unregisterComponent()
	{
		m_registedTypeId = InvalidComponentTypeId;
	}
}