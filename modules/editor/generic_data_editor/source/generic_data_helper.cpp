#include "tiki/generic_data_editor/generic_data_helper.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	bool generic_data::getMemberVector2( Vector2& target, const GenericDataObject* pObject, const DynamicString& memberName )
	{
		const GenericDataValue* pVectorValue = pObject->getFieldOrDefaultValue( memberName );
		if( pVectorValue == nullptr )
		{
			return false;
		}

		const GenericDataObject* pVectorObject = nullptr;
		pVectorValue->getObject( pVectorObject );
		if( pVectorObject == nullptr )
		{
			return false;
		}

		const GenericDataValue* pXValue = pVectorObject->getFieldValue( "x" );
		const GenericDataValue* pYValue = pVectorObject->getFieldValue( "y" );
		if( pXValue == nullptr || pYValue == nullptr )
		{
			return false;
		}

		return pXValue->getFloatingPoint( target.x ) &&
			pYValue->getFloatingPoint( target.y );
	}
}
