#include "tiki/game_flow/game_flow_system.hpp"

#include "tiki/game_flow/game_flow_state.hpp"

namespace tiki
{
	GameFlowSystem::GameFlowSystem()
	{
		m_isCreated = false;
	}

	void GameFlowSystem::create( const GameStateDefinition* pDefinition, uintreg definitionCount )
	{
		StateDefinition stateTreeDefinitions[ StateTreeLimits_MaxStateCount ];

		m_stateCount		= definitionCount;
		m_activeStateCount	= 0u;

		for (uintreg i = 0u; i < definitionCount; ++i)
		{
			StateDefinition& targetDef = stateTreeDefinitions[ i ];
			const GameStateDefinition& def = pDefinition[ i ];

			m_states[ i ] = def;

			uintreg index = i;
			uintreg count = 1u;
			while ( index != 0u )
			{
				index = pDefinition[ index ].parentStateIndex;
				count++;
			}
			TIKI_ASSERT( count <= StateTreeLimits_MaxHierarchyDepth );

			index = i;
			for (uintreg j = count - 1u; j != TIKI_SIZE_T_MAX; --j)
			{
				targetDef.stateHierarchy[ j ] = index;
				index = pDefinition[ index ].parentStateIndex;
			}

			targetDef.transitionStepCount	= def.transitionStepCount;
			targetDef.hierarchyLength		= count;
			targetDef.pName					= def.pName;
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
		TransitionState result = TransitionState_Finish;
		while ( m_stateTree.isInTransition() && result == TransitionState_Finish  )
		{
			GameFlowState* pState = m_states[ m_stateTree.getTransitionState() ].pState;
			if ( pState != nullptr )
			{
				const uintreg step			= m_stateTree.getCurrentStep();
				const bool isCreateing	= m_stateTree.isCreating();
				const bool isInital		= m_stateTree.isInitial();
				result					= pState->processTransitionStep( step, isCreateing, isInital );
			}
			else
			{
				result = TransitionState_Finish;
			}

			m_stateTree.updateTree( result );

			m_activeStateCount = m_stateTree.getActiveStates( m_activeStates, TIKI_COUNT( m_activeStates ) );
		}

		for (uintreg i = 0u; i < m_activeStateCount; ++i)
		{
			const uintreg stateIndex = m_activeStates[ i ];
  			m_states[ stateIndex ].pState->update();
		}
	}

	void GameFlowSystem::render( GraphicsContext& graphicsContext ) const
	{
		for (uintreg i = 0u; i < m_activeStateCount; ++i)
		{
			const uintreg stateIndex = m_activeStates[ i ];
			m_states[ stateIndex ].pState->render( graphicsContext );
		}

		for (uintreg i = 0u; i < m_activeStateCount; ++i)
		{
			const uintreg stateIndex = m_activeStates[ i ];
			m_states[ stateIndex ].pState->postRender( graphicsContext );
		}
	}

	bool GameFlowSystem::processInputEvent( const InputEvent& inputEvent )
	{
		for (uintreg i = 0u; i < m_activeStateCount; ++i)
		{
			const uintreg stateIndex = m_activeStates[ i ];
			if ( m_states[ stateIndex ].pState->processInputEvent( inputEvent ) )
			{
				return true;
			}
		}

		return false;
	}

	void GameFlowSystem::processWindowEvent( const WindowEvent& windowEvent )
	{
		for (uintreg i = 0u; i < m_activeStateCount; ++i)
		{
			const uintreg stateIndex = m_activeStates[ i ];
			m_states[ stateIndex ].pState->processWindowEvent( windowEvent );
		}
	}

	void GameFlowSystem::startTransition( int stateIndex )
	{
		m_stateTree.startTransition( stateIndex );
	}
}