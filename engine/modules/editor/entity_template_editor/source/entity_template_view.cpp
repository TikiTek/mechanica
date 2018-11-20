#include "tiki/entity_template_editor/entity_template_view.hpp"

namespace tiki
{
	EntityTemplateView::EntityTemplateView( GenericDataTypeCollection& typeCollection )
		: GenericDataView( typeCollection, "EntityTemplateData" )
	{
	}

	EntityTemplateView::~EntityTemplateView()
	{
	}

	void EntityTemplateView::updateObject( GenericDataViewInfo& objectInfo, List< GenericDataObject* >& childObjects )
	{

	}

	void EntityTemplateView::renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo )
	{

	}
}