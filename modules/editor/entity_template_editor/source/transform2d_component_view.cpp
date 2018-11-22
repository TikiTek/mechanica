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

	void Transform2dComponentView::updateObject( GenericDataViewInfo& objectInfo )
	{

	}

	void Transform2dComponentView::renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo )
	{

	}
}