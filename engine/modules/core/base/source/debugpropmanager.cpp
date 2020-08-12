
#include "tiki/base/debugpropmanager.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/io/xmlreader.hpp"

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

		void					loadProperties( const char* pFilename );
		void					saveProperties( const char* pFilename );

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

	void debugprop::loadProperties( const char* pFilename )
	{
		getDebugProManager().loadProperties( pFilename );
	}

	void debugprop::saveProperties( const char* pFilename )
	{
		getDebugProManager().saveProperties( pFilename );
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

	void DebugPropManager::loadProperties( const char* pFilename )
	{
		XmlReader xml;
		if ( !xml.create( pFilename ) )
		{
			return;
		}

		const XmlElement* pRootNode = xml.findNodeByName( "DebugProperties" );



		xml.dispose();
	}

	void DebugPropManager::saveProperties( const char* pFilename )
	{

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