#pragma once
#ifndef TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED
#define TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED

#include "tiki/base/types.hpp"

#include "tiki/gameflow/statetree.hpp"

namespace tiki
{
	class GameFlowState;
	class GraphicsContext;
	class InputSystem;
	struct InputEvent;
	struct WindowEvent;

	struct GameStateDefinition
	{
		GameFlowState*	pState;
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
		void				processWindowEvent( const WindowEvent& windowEvent );

		void				startTransition( int stateIndex );

		bool				isInState( int stateIndex ) const	{ return m_stateTree.isInState( stateIndex ); }
		bool				isInTransition() const				{ return m_stateTree.isInTransition(); }
		uint				getCurrentState() const				{ return m_stateTree.getCurrentState(); }

		uint				getStateCount() const				{ return m_stateCount; }

		bool				isCreated() const					{ return m_isCreated; }

	private:

		bool				m_isCreated;

		StateTree			m_stateTree;
		uint				m_activeStates[ StateTreeLimits_MaxStateCount ];
		uint				m_activeStateCount;

		uint				m_stateCount;
		GameStateDefinition	m_states[ StateTreeLimits_MaxStateCount ];
		
	};
}

#endif // TIKI_GAMEFLOWSYSTEM_HPP__INCLUDED
