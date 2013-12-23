
#include "tiki/gamestates/introstate.hpp"

namespace tiki
{
	void IntroState::create()
	{
	}

	void IntroState::dispose()
	{
	}

	TransitionState IntroState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		return TransitionState_Finish;
	}

	void IntroState::update()
	{
	}

	void IntroState::render( GraphicsContext& graphicsContext ) const
	{
	}
}