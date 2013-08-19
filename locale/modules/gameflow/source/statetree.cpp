
#include "tiki/gameflow/statetree.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	void StateTree::create( StateDefinition* pStateDefinitions, const size_t stateCount )
	{
		m_stateDefinition.create( pStateDefinitions, stateCount );

		m_currentState					= 0;		
		m_transitionNextState			= 0;
		m_transitionPathSize			= 0;
		m_transitionCurrentPathIndex	= s_unsignedMinusOne;
		
		m_currentStep	= StateTree_InvalidTransitionStep;
		m_isCreating	= false;
		m_isInitial		= false;
	}

	void StateTree::dispose()
	{
		TIKI_ASSERT( m_transitionCurrentPathIndex == s_unsignedMinusOne );

		m_stateDefinition.dispose();
	}

	void StateTree::startTransition( const int stateIndex )
	{
		TIKI_ASSERT( stateIndex < (int)m_stateDefinition.getCount() );
		TIKI_ASSERT( m_transitionCurrentPathIndex == s_unsignedMinusOne );

		if ( m_currentState == stateIndex )
		{
			return;
		}

		const StateDefinition& curDef	= m_stateDefinition[ m_currentState ];
		const StateDefinition& destDef	= m_stateDefinition[ stateIndex ];

		size_t curIndex = 0u;
		size_t destIndex = 0u;

		for (size_t i = curDef.hierarchyLength - 1u; i != s_unsignedMinusOne && curIndex == 0u; --i)
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
		for (size_t i = curStart; i >= curIndex && i != s_unsignedMinusOne; --i)
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
		
		m_currentStep	= ( m_currentState < m_transitionNextState ? 0u : curDef.transitionStepCount - 1u );
		m_isCreating	= ( m_currentState < m_transitionNextState ? true : false );
		m_isInitial		= true;
	}

	void StateTree::updateTree( const TransitionState newState )
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
					m_transitionCurrentPathIndex	+= m_transitionPathDirection;
					m_currentState					= m_transitionNextState;

					if ( m_transitionCurrentPathIndex == m_transitionPathSize )
					{
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
				TIKI_BREAK( "[statetree] errors currently not supported.\n");
				//TIKI_ASSERT( m_isCreating ); // shutdown must success

				//m_transitionPathDirection	= -1;
				//m_isCreating				= false;
				//m_currentStep--;
			}
			break;
		default:
			TIKI_BREAK( "[statetree] wrong transition state.\n" );
			break;
		}
	}

}