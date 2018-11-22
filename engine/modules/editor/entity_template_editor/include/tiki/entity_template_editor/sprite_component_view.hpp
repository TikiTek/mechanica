#pragma once

#include "tiki/generic_data_editor/generic_data_view.hpp"

namespace tiki
{
	class SpriteComponentView : public GenericDataView
	{
	public:

												SpriteComponentView( GenericDataTypeCollection& typeCollection );
		virtual									~SpriteComponentView();

		virtual void							updateObject( GenericDataViewInfo& objectInfo ) TIKI_OVERRIDE_FINAL;
		virtual void							renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo ) TIKI_OVERRIDE_FINAL;
	};
}