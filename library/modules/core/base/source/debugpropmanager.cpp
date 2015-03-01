
#include "tiki/base/debugpropmanager.hpp"

#include "tiki/base/debugprop.hpp"

namespace tiki
{
	static DebugPropManager& getDebugProManager()
	{
		static DebugPropManager s_manager;
		return s_manager;
	}

	void debugprop::registerProperty( DebugProp& prop )
	{
		getDebugProManager().registerProperty( prop );
	}

	void debugprop::unregisterProperty( DebugProp& prop )
	{
		getDebugProManager().unregisterProperty( prop );
	}

	LinkedList<DebugProp>& debugprop::getProperties()
	{
		return getDebugProManager().getProperties();
	}
	
	DebugPropManager::DebugPropManager()
	{
	}

	DebugPropManager::~DebugPropManager()
	{
	}

	void DebugPropManager::registerProperty( DebugProp& prop )
	{
		m_properties.push( prop );
	}

	void DebugPropManager::unregisterProperty( DebugProp& prop )
	{
		m_properties.removeSortedByValue( prop );
	}
}