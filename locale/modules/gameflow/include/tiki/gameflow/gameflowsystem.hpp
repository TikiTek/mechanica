#pragma once
#ifndef TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED
#define TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED

#include "tiki/base/types.hpp"

#include "tiki/gameflow/statetree.hpp"

namespace tiki
{
	class GameState;
	class GpuContext;

	struct GameStateDefinition
	{
		GameState*		pState;
		size_t			parentStateIndex;
		size_t			transitionStepCount;
	};

	class GameFlowSystem
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( GameFlowSystem );

	public:

		void				create( const GameStateDefinition* pDefinition, size_t definitionCount );
		void				dispose();

		void				update();
		void				render( GpuContext* pGraphicsCommands ) const;

		void				startTransition( const int stateIndex );

		const StateTree&	getStateTree() const { return m_stateTree; }

		bool				isInTransition() const { return m_stateTree.isInTransition(); }
		int					getCurrentState() const { return m_stateTree.getCurrentState(); }

		size_t				getStateCount() const { return m_stateCount; }

	private:

		StateTree			m_stateTree;

		size_t				m_stateCount;
		GameStateDefinition	m_states[ StateTree_MaxStateCount ];
		
	};
}

#endif // TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED
