
#include "tiki/gamestates/menustate.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/game/framework_game.hpp"
#include "tiki/game/game.hpp"
#include "tiki/gameflow/gameflowsystem.hpp"

namespace tiki
{
	MenuState::MenuState()
	{
		m_pParentState = nullptr;
	}

	MenuState::~MenuState()
	{
		TIKI_ASSERT( m_pParentState == nullptr );
	}

	void MenuState::create( ApplicationState* pParentState )
	{
		TIKI_ASSERT( pParentState != nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );

		m_pParentState = pParentState;

		m_renderer.dispose(
			framework::getGraphicsSystem(),
			framework::getResourceManager()
		);
	}

	void MenuState::dispose()
	{
		m_renderer.dispose(
			framework::getGraphicsSystem(),
			framework::getResourceManager()
		);

		m_pParentState = nullptr;
	}

	TransitionState MenuState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		return TransitionState_Finish;
	}

	void MenuState::update()
	{

	}

	void MenuState::render( GraphicsContext& graphicsContext )
	{

	}

	bool MenuState::processInputEvent( const InputEvent& inputEvent )
	{
		if (inputEvent.eventType == InputEventType_Keyboard_Up)
		{
			framework::getGameFlowSystem().startTransition( GameStates_Play );

			return true;
		}

		return false;
	}
}