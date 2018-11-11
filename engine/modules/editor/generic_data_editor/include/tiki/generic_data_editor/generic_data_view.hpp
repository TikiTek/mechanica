#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	class GenericDataObject;
	class GraphicsContext;
	struct InputEvent;

	class GenericDataView
	{
	public:

		virtual void		update() TIKI_PURE;
		virtual void		render( GraphicsContext& graphicsContext ) TIKI_PURE;

		virtual bool		handleInputEvent( const InputEvent& inputEvent ) TIKI_PURE;

	protected:

							GenericDataView( GenericDataObject*	pObject );
		virtual				~GenericDataView();

		GenericDataObject*	m_pObject;
	};
}