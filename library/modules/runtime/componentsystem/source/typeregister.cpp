
#include "tiki/base/crc32.hpp"
#include "tiki/components/component.hpp"
#include "tiki/components/typeregister.hpp"

namespace tiki
{
	bool ComponentTypeRegister::create( uint maxTypeCount )
	{
		if ( m_types.create( maxTypeCount ) == false )
		{
			return false;
		}

		for (uint i = 0u; i < m_types.getCount(); ++i)
		{
			RegisterType& type = m_types[ i ];

			type.pComponent	= nullptr;
			type.typeCrc	= InvalidCrc32;
			type.stateSize	= 0u;
			type.pName		= nullptr;
		} 

		return true;
	}

	void ComponentTypeRegister::dispose()
	{
		m_types.dispose();
	}

	ComponentTypeId ComponentTypeRegister::registerType( ComponentBase* pComponent )
	{
		TIKI_ASSERT( pComponent != nullptr );

		ComponentTypeId typeId = 0u;
		while ( m_types[ typeId ].typeCrc != InvalidCrc32 )
		{
			typeId++;

			if ( typeId == m_types.getCount() )
			{
				return InvalidComponentTypeId;
			}
		}

		RegisterType& type = m_types[ typeId ];
		type.pComponent	= pComponent;
		type.typeCrc	= pComponent->getTypeCrc();
		type.stateSize	= pComponent->getStateSize();
		type.pName		= pComponent->getTypeName();

		return typeId;
	}

	void ComponentTypeRegister::unregisterType( ComponentTypeId typeId )
	{
		if ( typeId == InvalidComponentTypeId )
		{
			return;
		}

		RegisterType& type = m_types[ typeId ];
		type.stateSize	= 0u;
		type.typeCrc	= InvalidCrc32;
	}

	bool ComponentTypeRegister::isTypeRegistred( ComponentTypeId typeId ) const
	{
		if ( typeId == InvalidComponentTypeId )
		{
			return false;
		}

		return m_types[ typeId ].typeCrc != InvalidCrc32;
	}

	bool ComponentTypeRegister::isComponentRegistred( const ComponentBase* pComponent ) const
	{
		for (uint i = 0u; i < m_types.getCount(); ++i)
		{
			if ( m_types[ i ].pComponent == pComponent )
			{
				return true;
			}
		}

		return false;
	}

	ComponentBase* ComponentTypeRegister::getTypeComponent( ComponentTypeId typeId ) const
	{
		TIKI_ASSERT( isTypeRegistred( typeId ) );
		return m_types[ typeId ].pComponent;
	}

	crc32 ComponentTypeRegister::getTypeCrc( ComponentTypeId typeId ) const
	{
		TIKI_ASSERT( isTypeRegistred( typeId ) );
		return m_types[ typeId ].typeCrc;
	}

	uint ComponentTypeRegister::getTypeStateSize( ComponentTypeId typeId ) const
	{
		TIKI_ASSERT( isTypeRegistred( typeId ) );
		return m_types[ typeId ].stateSize;
	}

	const char* ComponentTypeRegister::getTypeName( ComponentTypeId typeId ) const
	{
		TIKI_ASSERT( isTypeRegistred( typeId ) );
		return m_types[ typeId ].pName;
	}
}