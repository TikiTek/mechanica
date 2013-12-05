
#include "tiki/gameflow/gameflowsystem.hpp"

#include "tiki/gameflow/gamestate.hpp"

namespace tiki
{
	GameFlowSystem::GameFlowSystem()
	{
		m_isCreated = false;
	}

	void GameFlowSystem::create( const GameStateDefinition* pDefinition, size_t definitionCount )
	{
		StateDefinition stateTreeDefinitions[ StateTree_MaxStateCount ];

		m_stateCount = definitionCount;

		for (size_t i = 0u; i < definitionCount; ++i)
		{
			StateDefinition& targetDef = stateTreeDefinitions[ i ];
			const GameStateDefinition& def = pDefinition[ i ];

			m_states[ i ] = def;
		
			size_t index = i;
			size_t count = 1u;
			while ( index != 0u )
			{
				index = pDefinition[ index ].parentStateIndex;
				count++;
			}

			index = i;
			for (size_t j = count - 1u; j != TIKI_SIZE_T_MAX; --j)
			{
				targetDef.stateHierarchy[ j ] = index;
				index = pDefinition[ index ].parentStateIndex;
			}

			targetDef.transitionStepCount	= def.transitionStepCount;
			targetDef.hierarchyLength		= count;
		}

		m_stateTree.create( stateTreeDefinitions, definitionCount );

		m_isCreated = true;
	}

	void GameFlowSystem::dispose()
	{
		m_stateTree.dispose();

		m_isCreated = false;
	}

	void GameFlowSystem::update()
	{
		if ( m_stateTree.isInTransition() )
		{
			GameState* pState = m_states[ m_stateTree.getTransitionState() ].pState;

			TransitionState result;
			if ( pState != nullptr )
			{
				const int step		= m_stateTree.getCurrentStep();
				const bool create	= m_stateTree.isCreating();
				const bool inital	= m_stateTree.isInitial();
				result				= pState->processTransitionStep( step, create, inital );
			}
			else
			{
				result = TransitionState_Finish;
			}

			m_stateTree.updateTree( result );
		}

		const StateDefinition& currentState = m_stateTree.getCurrentStateDefinition();

		for (size_t i = 1u; i < currentState.hierarchyLength; ++i)
		{
			const size_t index = currentState.stateHierarchy[ i ];

			if ( m_stateTree.isInTransition() )
			{
				const size_t transitionIndex = ( m_stateTree.isCreating() ? m_stateTree.getTransitionState() : m_stateTree.getCurrentState() );

				if ( index == transitionIndex )
				{
					continue;
				}
			}

  			m_states[ index ].pState->update();
		}
	}

	void GameFlowSystem::render( GraphicsContext* pGraphicsContext ) const
	{
		const StateDefinition& currentState = m_stateTree.getCurrentStateDefinition();

		for (size_t i = 1u; i < currentState.hierarchyLength; ++i)
		{
			const size_t index = currentState.stateHierarchy[ i ];

			if ( index == m_stateTree.getCurrentState() && m_stateTree.isInTransition() )
			{
				continue;
			}

			m_states[ index ].pState->render( pGraphicsContext );
		}
	}

	void GameFlowSystem::startTransition( const int stateIndex )
	{		
		m_stateTree.startTransition( stateIndex );
	}
}