#pragma once

#include "tiki/generic_data_editor/generic_data_view.hpp"

namespace tiki
{
	class Transform2dComponentView : public GenericDataView
	{
	public:

												Transform2dComponentView( GenericDataTypeCollection& typeCollection );
		virtual									~Transform2dComponentView();

		virtual void							updateObject( GenericDataViewInfo& objectInfo, List< GenericDataObject* >& childObjects ) TIKI_OVERRIDE_FINAL;
		virtual void							renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo ) TIKI_OVERRIDE_FINAL;
	};
}