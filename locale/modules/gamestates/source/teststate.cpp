
#include "tiki/gamestates/teststate.hpp"

namespace tiki
{
	void TestState::create( ApplicationState* pParentState )
	{
		m_pParentState	= pParentState;
	}

	void TestState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState TestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case TestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				if ( isInital )
				{
					return TransitionState_Finish;
				}
				else
				{
					// todo: make resource manager async

					return TransitionState_Error;
				}
			}
			else
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			break;
		case TestStateTransitionSteps_CreateConponents:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			else
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			break;
		case TestStateTransitionSteps_InitializeConponents:
			if ( isCreating )
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			else
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			break;
		default:
			break;
		}

		TIKI_BREAK( "no result\n" );
		return TransitionState_Error;
	}

	void TestState::update()
	{
	}

	void TestState::render( GraphicsContext* pGraphicsContext ) const
	{
	}
}