#include "tiki/entity_template_editor/entity_template_view.hpp"

#include "tiki/tool_generic_data/generic_data_array.hpp"
#include "tiki/tool_generic_data/generic_data_object.hpp"

namespace tiki
{
	EntityTemplateView::EntityTemplateView( GenericDataTypeCollection& typeCollection )
		: GenericDataView( typeCollection, "EntityTemplateData" )
	{
	}

	EntityTemplateView::~EntityTemplateView()
	{
	}

	void EntityTemplateView::updateObject( GenericDataViewInfo& objectInfo, GenericDataViewInfo* pParentInfo )
	{
		GenericDataObject* pObject = objectInfo.pObject;

		GenericDataValue* pComponentsValue = pObject->getFieldValue( "components", false );
		if( pComponentsValue == nullptr )
		{
			return;
		}

		GenericDataArray* pComponentsArray = nullptr;
		pComponentsValue->getArray( pComponentsArray );
		if( pComponentsArray == nullptr )
		{
			return;
		}

		for( uint i = 0u; i < pComponentsArray->getCount(); ++i )
		{
			GenericDataValue* pElementValue = pComponentsArray->getElement( i );
			if( pElementValue == nullptr )
			{
				continue;
			}

			GenericDataObject* pComponentObject = nullptr;
			pElementValue->getObject( pComponentObject );
			if( pComponentObject == nullptr )
			{
				continue;
			}

			GenericDataValue* pInitDataValue = pComponentObject->getFieldValue( "initData", false );
			if( pInitDataValue == nullptr )
			{
				continue;
			}

			GenericDataObject* pInitDataObject = nullptr;
			pInitDataValue->getPointer( pInitDataObject );
			if( pInitDataObject == nullptr )
			{
				continue;
			}

			objectInfo.childObjects.pushBack( pInitDataObject );
		}
	}

	void EntityTemplateView::renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo )
	{

	}
}