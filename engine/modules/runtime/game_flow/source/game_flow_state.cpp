#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/resource/resource_request_pool.hpp"

namespace tiki
{
	GameFlowState::GameFlowState()
	{
	}

	GameFlowState::~GameFlowState()
	{
	}

	TransitionState GameFlowState::processTransitionStep( uintreg currentStep, bool isCreating, bool isInital )
	{
		if( isCreating )
		{
			return processCreationStep( currentStep, isInital );
		}
		else
		{
			return processDestructionStep( currentStep, isInital );
		}
	}

	TransitionState GameFlowState::processCreationStep( uintreg currentStep, bool isInital )
	{
		return TransitionState_Finish;
	}

	TransitionState GameFlowState::processDestructionStep( uintreg currentStep, bool isInital )
	{
		return TransitionState_Finish;
	}

	TransitionState GameFlowState::waitForResources( const ResourceRequestPool& resourceRequestPool ) const
	{
		if( resourceRequestPool.isFinish() )
		{
			if( resourceRequestPool.hasError() )
			{
				return TransitionState_Error;
			}

			return TransitionState_Finish;
		}

		return TransitionState_InProcess;
	}
}
