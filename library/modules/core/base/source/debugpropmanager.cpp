
#include "tiki/base/debugpropmanager.hpp"

#include "tiki/base/debugprop.hpp"

namespace tiki
{
	class DebugPropManager
	{
		TIKI_NONCOPYABLE_CLASS( DebugPropManager );

	public:

		DebugPropManager();
		~DebugPropManager();

		void					registerProperty( DebugProp& prop );
		void					unregisterProperty( DebugProp& prop );

		LinkedList<DebugProp>&	getProperties() { return m_properties; }

	private:

		LinkedList< DebugProp >	m_properties;

		DebugProp*				findPropertyByFullName( const char* pName );

	};

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
		TIKI_ASSERT( findPropertyByFullName( prop.getFullName() ) == nullptr );
		m_properties.push( prop );
	}

	void DebugPropManager::unregisterProperty( DebugProp& prop )
	{
		m_properties.removeSortedByValue( prop );
	}

	DebugProp* DebugPropManager::findPropertyByFullName( const char* pName )
	{
		for (DebugProp& prop : m_properties)
		{
			if ( isStringEquals( prop.getFullName(), pName ) )
			{
				return &prop;
			}
		}

		return nullptr;
	}
}