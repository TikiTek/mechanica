#include "tiki/entity_template_editor/transform2d_component_view.hpp"

namespace tiki
{
	Transform2dComponentView::Transform2dComponentView( GenericDataTypeCollection& typeCollection )
		: GenericDataView( typeCollection, "Transform2dComponentInitData" )
	{
	}

	Transform2dComponentView::~Transform2dComponentView()
	{
	}

	GenericDataObject* Transform2dComponentView::findTransformChild( GenericDataViewInfo& objectInfo ) const
	{
		for( GenericDataObject* pChildObject : objectInfo.childObjects )
		{
			if( pChildObject->getType() == m_pType )
			{
				return pChildObject;
			}
		}

		return nullptr;
	}

	bool Transform2dComponentView::getPosition( Vector2& targetPosition, const GenericDataObject* pTransformObject ) const
	{
		TIKI_ASSERT( pTransformObject->getType() == m_pType );
		return generic_data::getMemberVector2( targetPosition, pTransformObject, "position" );
	}

	bool Transform2dComponentView::getRotation( float& targetRotation, const GenericDataObject* pTransformObject ) const
	{
		TIKI_ASSERT( pTransformObject->getType() == m_pType );

		const GenericDataValue* pRotationValue = pTransformObject->getFieldValue( "rotation" );
		if( pRotationValue == nullptr )
		{
			return false;
		}

		return pRotationValue->getFloatingPoint( targetRotation );
	}

	bool Transform2dComponentView::getScale( Vector2& targetScale, const GenericDataObject* pTransformObject ) const
	{
		TIKI_ASSERT( pTransformObject->getType() == m_pType );
		return generic_data::getMemberVector2( targetScale, pTransformObject, "scale" );
	}

	void Transform2dComponentView::updateObject( GenericDataViewInfo& objectInfo, GenericDataViewInfo* pParentInfo )
	{
	}

	void Transform2dComponentView::renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo )
	{
	}
}