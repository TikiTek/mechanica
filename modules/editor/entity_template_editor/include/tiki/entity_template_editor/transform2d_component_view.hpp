#pragma once

#include "tiki/generic_data_editor/generic_data_view.hpp"

namespace tiki
{
	class Transform2dComponentView : public GenericDataView
	{
	public:

									Transform2dComponentView( GenericDataTypeCollection& typeCollection );
		virtual						~Transform2dComponentView();

		GenericDataObject*			findTransformChild( GenericDataViewInfo& objectInfo ) const;

		bool						getPosition( Vector2& targetPosition, const GenericDataObject* pTransformObject ) const;
		bool						getRotation( float& targetRotation, const GenericDataObject* pTransformObject ) const;
		bool						getScale( Vector2& targetScale, const GenericDataObject* pTransformObject ) const;

		virtual void				updateObject( GenericDataViewInfo& objectInfo, GenericDataViewInfo* pParentInfo ) TIKI_OVERRIDE_FINAL;
		virtual void				renderObject( Renderer2d& renderer, const GenericDataViewInfo& objectInfo ) TIKI_OVERRIDE_FINAL;
	};
}