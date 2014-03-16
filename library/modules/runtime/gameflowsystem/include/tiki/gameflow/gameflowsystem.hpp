#pragma once
#ifndef TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED
#define TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED

#include "tiki/base/types.hpp"

#include "tiki/gameflow/statetree.hpp"

namespace tiki
{
	class GameState;
	class GraphicsContext;
	class InputSystem;
	struct InputEvent;

	struct GameStateDefinition
	{
		GameState*		pState;
		uint			parentStateIndex;
		uint			transitionStepCount;

		const char*		pName;
	};

	class GameFlowSystem
	{
		TIKI_NONCOPYABLE_CLASS( GameFlowSystem );

	public:

		GameFlowSystem();

		void				create( const GameStateDefinition* pDefinition, uint definitionCount );
		void				dispose();

		void				update();
		void				render( GraphicsContext& graphicsContext ) const;

		bool				processInputEvent( const InputEvent& inputEvent );

		void				startTransition( int stateIndex );

		const StateTree&	getStateTree() const	{ return m_stateTree; }

		bool				isInTransition() const	{ return m_stateTree.isInTransition(); }
		int					getCurrentState() const	{ return m_stateTree.getCurrentState(); }

		uint				getStateCount() const	{ return m_stateCount; }

		bool				isCreated() const		{ return m_isCreated; }

	private:

		bool				m_isCreated;

		StateTree			m_stateTree;
		int					m_activeStates[ StateTree_MaxStateCount ];
		uint				m_activeStateCount;

		uint				m_stateCount;
		GameStateDefinition	m_states[ StateTree_MaxStateCount ];
		
	};
}

#endif // TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED
