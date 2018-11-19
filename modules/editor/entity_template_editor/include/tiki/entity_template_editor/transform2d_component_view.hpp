#pragma once

#include "tiki/generic_data_editor/generic_data_view.hpp"

namespace tiki
{
	class Transform2dComponentView : public GenericDataView
	{
	public:

										Transform2dComponentView();
		virtual							~Transform2dComponentView();

		virtual GenericDataViewState*	addState( GenericDataObject* pObject ) TIKI_OVERRIDE_FINAL;
		virtual void					removeState( GenericDataViewState* pState ) TIKI_OVERRIDE_FINAL;

		virtual void					update( GenericDataViewState* pState ) TIKI_OVERRIDE_FINAL;
		virtual void					render( Renderer2d& renderer, GenericDataViewState* pState ) TIKI_OVERRIDE_FINAL;

		virtual bool					handleInputEvent( const InputEvent& inputEvent, GenericDataViewState* pState ) TIKI_OVERRIDE_FINAL;

	private:

		struct Transform2dViewState : public GenericDataViewState
		{

		};

		Transform2dViewState*			getState( GenericDataViewState* pState );
	};
}