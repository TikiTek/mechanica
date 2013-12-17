
#include "tiki/gamestates/playstate.hpp"

namespace tiki
{
	void PlayState::create( ApplicationState* pParentState )
	{
		m_pParentState	= pParentState;
	}

	void PlayState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState PlayState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case PlayStateTransitionSteps_InitializeBase:
			TIKI_ASSERT( isInital );

			if ( isCreating )
			{
			}
			else
			{
			}

			return TransitionState_Finish;
		case PlayStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			else
			{
				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_CreateComponents:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			else
			{

				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_InitializeComponents:
			if ( isCreating )
			{

				return TransitionState_Finish;
			}
			else
			{
				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_CreateEntities:
			if ( isCreating )
			{
				return TransitionState_Finish;
			}
			else
			{
				return TransitionState_Finish;
			}
			break;
		case PlayStateTransitionSteps_CreateLevel:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );
				return TransitionState_Finish;
			}
			else
			{
				return TransitionState_Finish;
			}
			break;
		default:
			break;
		}

		return TransitionState_Error;
	}

	void PlayState::update()
	{
	}

	void PlayState::render( GraphicsContext* pGraphicsContext ) const
	{
	}
}