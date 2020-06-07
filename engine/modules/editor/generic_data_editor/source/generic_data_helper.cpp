#include "tiki/generic_data_editor/generic_data_helper.hpp"

#include "tiki/math/vector.hpp"

namespace tiki
{
	bool generic_data::getMemberVector2( Vector2& target, const GenericDataObject* pObject, const DynamicString& memberName )
	{
		const GenericDataLevelValue vectorValue = pObject->getFieldLevelValue( memberName );
		if( vectorValue.pConstValue == nullptr )
		{
			return false;
		}

		const GenericDataObject* pVectorObject = nullptr;
		vectorValue.pConstValue->getObject( pVectorObject );
		if( pVectorObject == nullptr )
		{
			return false;
		}

		const GenericDataLevelValue xValue = pVectorObject->getFieldLevelValue( "x" );
		const GenericDataLevelValue yValue = pVectorObject->getFieldLevelValue( "y" );
		if( xValue.pConstValue == nullptr || yValue.pConstValue == nullptr )
		{
			return false;
		}

		return xValue.pConstValue->getFloatingPoint( target.x ) &&
			yValue.pConstValue->getFloatingPoint( target.y );
	}
}
