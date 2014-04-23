
#include "tiki/gameflow/statetree.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	StateTree::StateTree()
	{
		m_currentStep	= 0u;
		m_isCreating	= false;
		m_isInitial		= false;
	}

	void StateTree::create( StateDefinition* pStateDefinitions, uint stateCount )
	{
		m_stateDefinition.create( pStateDefinitions, stateCount );

		m_currentState					= 0;		
		m_transitionSourceState			= 0;
		m_transitionNextState			= 0;
		m_transitionPathSize			= 0;
		m_transitionCurrentPathIndex	= TIKI_SIZE_T_MAX;
		
		m_currentStep	= StateTree_InvalidTransitionStep;
		m_isCreating	= false;
		m_isInitial		= false;
	}

	void StateTree::dispose()
	{
		TIKI_ASSERT( m_transitionCurrentPathIndex == TIKI_SIZE_T_MAX );

		m_stateDefinition.dispose();
	}

	void StateTree::startTransition( int stateIndex )
	{
		TIKI_ASSERT( stateIndex < (int)m_stateDefinition.getCount() );

		if ( m_currentState == stateIndex && m_transitionCurrentPathIndex == TIKI_SIZE_T_MAX )
		{
			return;
		}
		m_transitionSourceState = m_currentState;

		const StateDefinition& curDef	= m_stateDefinition[ m_currentState ];
		const StateDefinition& destDef	= m_stateDefinition[ stateIndex ];

		size_t curIndex = 0u;
		size_t destIndex = 0u;

		for (size_t i = curDef.hierarchyLength - 1u; i != TIKI_SIZE_T_MAX && curIndex == 0u; --i)
		{
			for (size_t j = 0u; j < destDef.hierarchyLength; ++j)
			{
				if ( curDef.stateHierarchy[ i ] == destDef.stateHierarchy[ j ] )
				{
					curIndex	= i;
					destIndex	= j;
					break;
				}
			}
		}

		m_transitionPathSize = 0u;

		const size_t curStart = curDef.hierarchyLength - 1u;
		for (size_t i = curStart; i >= curIndex && i != TIKI_SIZE_T_MAX; --i)
		{
			m_transitionPath[ m_transitionPathSize++ ] = curDef.stateHierarchy[ i ];
		}

		for (size_t i = destIndex + 1u; i < destDef.hierarchyLength; ++i)
		{
			m_transitionPath[ m_transitionPathSize++ ] = destDef.stateHierarchy[ i ];
		}

		//TIKI_TRACE( "[statetree] start transition to: %i -> %i, path:", m_currentState, stateIndex );
		//for (size_t i = 0u; i < m_transitionPathSize; ++i)
		//{
		//	TIKI_TRACE( " %i", m_transitionPath[ i ] );
		//}
		//TIKI_TRACE( "\n" );

		m_transitionNextState			= m_transitionPath[ 0u ];
		m_transitionPathDirection		= 1;
		m_transitionCurrentPathIndex	= 0u;
		
		TIKI_ASSERT( m_transitionPathSize > 1u );
		const bool forward = ( m_currentState < m_transitionPath[ 1u ] );
		m_currentStep	= ( forward ? 0u : curDef.transitionStepCount - 1u );
		m_isCreating	= forward;
		m_isInitial		= true;
	}

	void StateTree::updateTree( TransitionState newState )
	{
		m_isInitial = false;

		//TIKI_TRACE( "[statetree] state: %i, step: %i, status: %i\n", m_transitionNextState, m_currentStep, newState );

		switch ( newState )
		{
		case TransitionState_InProcess:
			return;
		case TransitionState_Finish:
			{
				m_currentStep += ( m_isCreating ? 1 : -1 );
				m_isInitial = true;

				if ( ( m_currentStep >= m_stateDefinition[ m_transitionNextState ].transitionStepCount && m_isCreating ) ||
					 ( m_currentStep < 0 && !m_isCreating ) )
				{
					m_transitionCurrentPathIndex += m_transitionPathDirection;

					if ( m_isCreating )
					{
						m_currentState = m_transitionNextState;
					}
					else
					{
						m_currentState = m_transitionPath[ m_transitionCurrentPathIndex ];
					}

					if ( m_transitionCurrentPathIndex == m_transitionPathSize )
					{
						// transition finish
						m_transitionCurrentPathIndex = -1;
					}
					else
					{
						m_transitionNextState	= m_transitionPath[ m_transitionCurrentPathIndex ];

						const bool forward		= ( m_currentState < m_transitionNextState );
						m_currentStep			= ( forward ? 0u : m_stateDefinition[ m_currentState ].transitionStepCount - 1u );
						m_isCreating			= ( forward ? true : false );
					}
				}
			}
			break;

		case TransitionState_Error:
			{
				//TIKI_TRACE( "[statetree] error has occured. current state: %i, current step: %i\n", m_currentState, m_currentStep );
				if ( !m_isCreating )
				{
					// ignore errors during shutdown
					break;
				}

				startTransition( m_transitionSourceState );
			}
			break;

		default:
			TIKI_BREAK( "[statetree] wrong transition state.\n" );
			break;
		}
	}
}