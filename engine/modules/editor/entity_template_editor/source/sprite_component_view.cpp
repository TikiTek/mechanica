#include "tiki/entity_template_editor/sprite_component_view.hpp"

#include "tiki/math/rectangle.hpp"
#include "tiki/tool_generic_data/generic_data_helper.hpp"

namespace tiki
{
	SpriteComponentView::SpriteComponentView( GenericDataTypeCollection& typeCollection, Transform2dComponentView& transformView )
		: GenericDataView( typeCollection, "SpriteComponentInitData" )
		, m_transformView( transformView )
	{
	}

	SpriteComponentView::~SpriteComponentView()
	{
	}

	bool SpriteComponentView::getTextureName( DynamicString& targetTextureName, const GenericDataObject* pSpriteObject ) const
	{
		TIKI_ASSERT( pSpriteObject->getType() == m_pType );

		const GenericDataValue* pTextureValue = pSpriteObject->getFieldValue( "texture" );
		if( pTextureValue == nullptr )
		{
			return false;
		}

		return pTextureValue->getReference( targetTextureName );
	}

	bool SpriteComponentView::getOffset( Vector2& targetOffset, const GenericDataObject* pSpriteObject ) const
	{
		TIKI_ASSERT( pSpriteObject->getType() == m_pType );
		return generic_data::getMemberVector2( targetOffset, pSpriteObject, "offset" );
	}

	bool SpriteComponentView::getLayerId( uint32& targetLayerId, const GenericDataObject* pSpriteObject ) const
	{
		TIKI_ASSERT( pSpriteObject->getType() == m_pType );

		const GenericDataValue* pLayerIdValue = pSpriteObject->getFieldValue( "layerId" );
		if( pLayerIdValue == nullptr )
		{
			return false;
		}

		return pLayerIdValue->getUnsignedValue( targetLayerId );
	}

	void SpriteComponentView::updateObject( GenericDataViewInfo& objectInfo, GenericDataViewInfo* pParentInfo )
	{
		TIKI_ASSERT( pParentInfo != nullptr );

		DynamicString textureName;
		if( !getTextureName( textureName, objectInfo.pObject ) )
		{
			return;
		}

		Vector2 position;
		float rotaion;
		Vector2 scale;

		GenericDataObject* pTransformObject = m_transformView.findTransformChild( *pParentInfo );
		if( pTransformObject == nullptr ||
			!m_transformView.getPosition( position, pTransformObject ) ||
			!m_transformView.getRotation(rotaion, pTransformObject) ||
			!m_transformView.getScale( scale, pTransformObject ) )
		{
			return;
		}

		Rectangle rectangle;
		rectangle.createFromCenterExtends( position, scale, rotaion );
		objectInfo.rectangle = rectangle.getBounds();
	}

	void SpriteComponentView::renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo )
	{

	}
}