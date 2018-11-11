
#include "tiki/gameflow/statetree.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	StateTree::StateTree()
	{
		m_currentState	= 0u;
		m_isInitial		= false;
	}

	void StateTree::create( StateDefinition* pStateDefinitions, uint stateCount )
	{
		m_stateDefinition.create( pStateDefinitions, stateCount );

		m_currentState	= 0u;
		m_isInitial		= false;

		m_transition.clear();
	}

	void StateTree::dispose()
	{
		TIKI_ASSERT( !isInTransition() );

		m_stateDefinition.dispose();
	}
	
	uint StateTree::getActiveStates( uint* pTargetActiveStates, uint capacity ) const
	{
		const uint currentStateIndex		= getCurrentState();
		const uint currentTransitionIndex	= isInTransition() ? m_transition.getCurrentTransitionState() : InvalidStateIndex;
		const StateDefinition& currentState	= m_stateDefinition[ currentStateIndex ];

		uint activeStateCount = 0u;
		for (uint i = 1u; i < currentState.hierarchyLength; ++i)
		{
			const uint stateIndex = currentState.stateHierarchy[ i ];
			TIKI_ASSERT( stateIndex != InvalidStateIndex );

			if ( stateIndex == currentTransitionIndex )
			{
				break;
			}

			pTargetActiveStates[ activeStateCount++ ] = stateIndex;
		}

		return activeStateCount;
	}

	bool StateTree::isInState( uint stateIndex ) const
	{
		const uint currentStateIndex		= getCurrentState();
		const uint currentTransitionIndex	= isInTransition() ? m_transition.getCurrentTransitionState() : InvalidStateIndex;
		const StateDefinition& currentState	= m_stateDefinition[ currentStateIndex ];

		for( uint i = 1u; i < currentState.hierarchyLength; ++i )
		{
			const uint currentStateIndex = currentState.stateHierarchy[ i ];
			TIKI_ASSERT( currentStateIndex != InvalidStateIndex );

			if( currentStateIndex == currentTransitionIndex )
			{
				break;
			}

			if( currentStateIndex == stateIndex )
			{
				return true;
			}
		}

		return false;
	}

	void StateTree::startTransition( uint targetStateIndex )
	{
		TIKI_ASSERT( targetStateIndex < m_stateDefinition.getCount() );

		const uint currentState = isInTransition() ? m_transition.getCurrentTransitionState() : m_currentState;

		if ( m_currentState == targetStateIndex && !isInTransition() )
		{
			return;
		}
		m_transition.sourceState		= currentState;
		m_transition.destinationState	= targetStateIndex;
		
		const StateDefinition& sourceDefinition			= m_stateDefinition[ currentState ];
		const StateDefinition& destinationDefinition	= m_stateDefinition[ targetStateIndex ];

		uint sourceMatchIndex		= 0u;
		uint destinationMatchIndex	= 0u;
		for (uint i = sourceDefinition.hierarchyLength - 1u; i != TIKI_SIZE_T_MAX && sourceMatchIndex == 0u; --i)
		{
			for (uint j = 0u; j < destinationDefinition.hierarchyLength; ++j)
			{
				if ( sourceDefinition.stateHierarchy[ i ] == destinationDefinition.stateHierarchy[ j ] )
				{
					sourceMatchIndex		= i;
					destinationMatchIndex	= j;
					break;
				}
			}
		}

		m_transition.path.clear();

		const uint curStart = sourceDefinition.hierarchyLength - 1u;
		for (uint i = curStart - 1u; i >= sourceMatchIndex && i != TIKI_SIZE_T_MAX; --i)
		{
			m_transition.path.push( sourceDefinition.stateHierarchy[ i ] );
		}

		for (uint i = destinationMatchIndex + 1u; i < destinationDefinition.hierarchyLength; ++i)
		{
			m_transition.path.push( destinationDefinition.stateHierarchy[ i ] );
		}

		TIKI_TRACE_DEBUG( "[statetree] start transition to: %i -> %i, path:", currentState, targetStateIndex );
		for (uint i = 0u; i < m_transition.path.getCount(); ++i)
		{
			TIKI_TRACE_DEBUG( " %i", m_transition.path[ i ] );
		}
		TIKI_TRACE_DEBUG( "\n" );

		m_transition.pathIndex		= 0u;
		m_transition.currentState	= currentState;
		
		const bool forward			= m_transition.isInForwardTransition();
		const uint transitionState	= ( forward ? m_transition.getCurrentTransitionState() : currentState );

		if ( !isInTransition() )
		{
			m_transition.currentStep = ( forward ? 0u : m_stateDefinition[ currentState ].transitionStepCount - 1u );
		}

		m_transition.targetStep		= ( forward ? m_stateDefinition[ transitionState ].transitionStepCount : InvalidTransitionStep );
		m_isInitial					= true;
		m_currentState				= InvalidStateIndex;
	}

	void StateTree::updateTree( TransitionState newState )
	{
		m_isInitial = false;

		TIKI_TRACE_DEBUG( "[statetree] state: %s(%u), step: %i, status: %i\n", m_stateDefinition[ m_transition.getCurrentTransitionState() ].pName, m_transition.getCurrentTransitionState(), m_transition.currentStep, newState );

		switch ( newState )
		{
		case TransitionState_InProcess:
			return;
		case TransitionState_Finish:
			{
				if ( m_transition.currentStep != m_transition.targetStep )
				{
					if ( m_transition.targetStep != InvalidTransitionStep )
					{
						m_transition.currentStep++;
					}
					else
					{
						m_transition.currentStep--;
					}
				}
				m_isInitial = true;

				if ( m_transition.currentStep == m_transition.targetStep )
				{
					m_transition.currentState = m_transition.path[ m_transition.pathIndex ];
					m_transition.pathIndex++;

					if ( m_transition.pathIndex == m_transition.path.getCount() )
					{
						// transition finish
						m_currentState = m_transition.currentState;
						m_transition.clear();
					}
					else
					{
						const bool forward			= m_transition.isInForwardTransition();
						const uint transitionState	= m_transition.getCurrentTransitionState();
						m_transition.currentStep	= ( forward ? 0u : m_stateDefinition[ transitionState ].transitionStepCount - 1u );
						m_transition.targetStep		= ( forward ? m_stateDefinition[ transitionState ].transitionStepCount : InvalidTransitionStep );
					}
				}
			}
			break;

		case TransitionState_Error:
			{
				TIKI_TRACE_ERROR( "[statetree] error has occured. current state: %s(%u), current step: %i\n", m_stateDefinition[ m_transition.getCurrentTransitionState() ].pName, m_transition.getCurrentTransitionState(), m_transition.currentStep );
				if ( !m_transition.isInForwardTransition() )
				{
					// ignore errors during shutdown
					break;
				}

				startTransition( m_transition.sourceState );
			}
			break;

		default:
			TIKI_BREAK( "[statetree] wrong transition state.\n" );
			break;
		}
	}
}