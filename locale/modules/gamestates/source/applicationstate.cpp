
#include "tiki/gamestates/applicationstate.hpp"

namespace tiki
{
	void ApplicationState::create()
	{
	}

	void ApplicationState::dispose()
	{
	}

	TransitionState ApplicationState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		return TransitionState_Finish;
	}

	void ApplicationState::update()
	{
	}

	void ApplicationState::render( GraphicsContext* pGraphicsContext ) const
	{
	}
}