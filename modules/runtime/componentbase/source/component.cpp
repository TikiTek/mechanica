#include "tiki/components/component.hpp"

#include "tiki/base/crc32.hpp"

namespace tiki
{
	ComponentBase::ComponentBase( crc32 typeCrc, const char* pTypeName, uint32 stateSize, bool constructState )
		: m_pFirstComponentState( nullptr )
		, m_typeCrc( typeCrc )
		, m_pTypeName( pTypeName )
		, m_stateSize( stateSize )
		, m_constuctState( constructState )
		, m_registedTypeId( InvalidComponentTypeId )
	{
	}

	ComponentBase::~ComponentBase()
	{
		TIKI_ASSERT( m_registedTypeId == InvalidComponentTypeId );
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