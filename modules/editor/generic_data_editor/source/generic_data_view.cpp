#include "tiki/generic_data_editor/generic_data_view.hpp"

namespace tiki
{
	GenericDataView::GenericDataView( GenericDataTypeCollection& typeCollection, const char* pTypeName )
		: m_typeCollection( typeCollection )
		, m_pType( nullptr )
	{
		const GenericDataType* pType = m_typeCollection.findTypeByName( pTypeName );
		if( pType == nullptr )
		{
			TIKI_TRACE_ERROR( "[editor] Could not find type '%s' for view.\n", pTypeName );
			TIKI_ASSERT( false );
			return;
		}

		if( pType->getType() != GenericDataTypeType_Struct )
		{
			TIKI_TRACE_ERROR( "[editor] Type '%s' for view is not a struct.\n", pTypeName );
			TIKI_ASSERT( false );
			return;
		}

		m_pType = (const GenericDataTypeStruct*)pType;
	}
}