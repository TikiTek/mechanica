#include "tiki/game_states/intro_state.hpp"

namespace tiki
{
	IntroState::IntroState()
	{
		m_pParentState = nullptr;
	}

	IntroState::~IntroState()
	{
		TIKI_ASSERT( m_pParentState == nullptr );
	}

	void IntroState::create( ApplicationState* pParentState )
	{
		TIKI_ASSERT( pParentState != nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );

		m_pParentState = pParentState;
	}

	void IntroState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState IntroState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		return TransitionState_Finish;
	}

	void IntroState::update()
	{
	}

	void IntroState::render( GraphicsContext& graphicsContext )
	{
	}

	bool IntroState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}
}