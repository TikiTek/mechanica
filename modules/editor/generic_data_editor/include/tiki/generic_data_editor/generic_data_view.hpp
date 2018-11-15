#pragma once

#include "tiki/base/types.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"

namespace tiki
{
	class GenericDataObject;
	class GraphicsContext;
	struct InputEvent;

	struct GenericDataViewState
	{
		GenericDataObject*		pObject;
		AxisAlignedRectangle	rectangle;
	};

	class GenericDataView
	{
	public:

		virtual GenericDataViewState*	addState( GenericDataObject* pObject ) TIKI_PURE;
		virtual void					removeState( GenericDataViewState* pState ) TIKI_PURE;

		virtual void					update( GenericDataViewState* pState ) TIKI_PURE;
		virtual void					render( GraphicsContext& graphicsContext, GenericDataViewState* pState ) TIKI_PURE;

		virtual bool					handleInputEvent( const InputEvent& inputEvent, GenericDataViewState* pState ) TIKI_PURE;

	protected:

										GenericDataView();
		virtual							~GenericDataView();
	};
}