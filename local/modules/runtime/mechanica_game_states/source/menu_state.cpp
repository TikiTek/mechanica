#include "tiki/game_states/menu_state.hpp"

#include "tiki/game/game.hpp"
#include "tiki/game_flow/game_flow_system.hpp"

namespace tiki
{
	MenuState::MenuState()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;
	}

	MenuState::~MenuState()
	{
		TIKI_ASSERT( m_pGame == nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );
	}

	void MenuState::create(Game* pGame, ApplicationState* pParentState )
	{
		TIKI_ASSERT( pGame != nullptr );
		TIKI_ASSERT( pParentState != nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );

		m_pGame			= pGame;
		m_pParentState	= pParentState;
	}

	void MenuState::dispose()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;
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
			m_pGame->getGameFlowSystem().startTransition( GameStates_Play );

			return true;
		}

		return false;
	}
}