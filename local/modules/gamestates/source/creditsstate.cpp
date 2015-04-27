
#include "tiki/gamestates/creditsstate.hpp"

namespace tiki
{
	CreditsState::CreditsState()
	{
		m_pParentState = nullptr;
	}

	CreditsState::~CreditsState()
	{
		TIKI_ASSERT( m_pParentState == nullptr );
	}

	void CreditsState::create( ApplicationState* pParentState )
	{
		TIKI_ASSERT( pParentState != nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );

		m_pParentState = pParentState;
	}

	void CreditsState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState CreditsState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		return TransitionState_Finish;
	}

	void CreditsState::update()
	{

	}

	void CreditsState::render( GraphicsContext& graphicsContext )
	{

	}

	bool CreditsState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}
}