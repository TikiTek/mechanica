#include "tiki/entity_template_editor/sprite_component_view.hpp"

namespace tiki
{
	SpriteComponentView::SpriteComponentView( GenericDataTypeCollection& typeCollection )
		: GenericDataView( typeCollection, "SpriteComponentInitData" )
	{
	}

	SpriteComponentView::~SpriteComponentView()
	{
	}

	void SpriteComponentView::updateObject( GenericDataViewInfo& objectInfo )
	{
		objectInfo.rectangle = createAxisAlignedRectangle( Vector2::zero, Vector2::one );
	}

	void SpriteComponentView::renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo )
	{

	}
}