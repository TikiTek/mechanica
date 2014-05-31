
#include "tiki/gamestates/basicstate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/graphicscontext.hpp"

namespace tiki
{
	void BasicTestState::create()
	{
	}

	void BasicTestState::dispose()
	{
	}

	TransitionState BasicTestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		return TransitionState_Finish;
	}

	void BasicTestState::update()
	{
	}

	void BasicTestState::render( GraphicsContext& graphicsContext )
	{
		const Color aColors[] =
		{
			TIKI_COLOR_RED,
			TIKI_COLOR_GREEN,
			TIKI_COLOR_BLUE,
			TIKI_COLOR_YELLOW,
			TIKI_COLOR_PINK,
			TIKI_COLOR_PURPLE,
			TIKI_COLOR_ORANGE
		};
		const uint colorIndex = uint( framework::getFrameTimer().getTotalTime() / 5.0 );

		graphicsContext.clear( graphicsContext.getBackBuffer(), aColors[ colorIndex ] );
	}

	bool BasicTestState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

}