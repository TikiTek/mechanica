#pragma once
#ifndef TIKI_STATETREE_HPP__INCLUDED
#define TIKI_STATETREE_HPP__INCLUDED

#include "tiki/base/array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	enum
	{
		StateTree_MaxStateCount			= 16u,
		StateTree_MaxTransitionPathSize	= 16u * 2u,
		StateTree_InvalidTransitionStep	= TIKI_SIZE_T_MAX
	};

	enum TransitionState
	{
		TransitionState_Invalid,
		TransitionState_InProcess,
		TransitionState_Finish,
		TransitionState_Error,

		TransitionState_Count
	};

	struct StateDefinition
	{
		int		stateHierarchy[ StateTree_MaxStateCount ];
		size_t	hierarchyLength;

		int		transitionStepCount;
	};

	class StateTree
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( StateTree );

	public:

		void						create( StateDefinition* pStateDefinitions, const size_t stateCount );
		void						dispose();

		void						startTransition( const int stateIndex );

		void						updateTree( const TransitionState newState );

		const StateDefinition&		getCurrentStateDefinition() const { return m_stateDefinition[ m_currentState ]; }
		int							getCurrentState() const { return m_currentState; }
		int							getTransitionState() const { return m_transitionNextState; }
		bool						isInTransition() const { return m_transitionCurrentPathIndex != TIKI_SIZE_T_MAX; }

		int							getCurrentStep() const { return m_currentStep; }
		bool						isCreating() const { return m_isCreating; }
		bool						isInitial() const { return m_isInitial; }

	private:

		Array< StateDefinition >	m_stateDefinition;

		int							m_currentState;

		int							m_transitionNextState;
		int							m_transitionPathDirection;
		size_t						m_transitionCurrentPathIndex;
		int							m_transitionPath[ StateTree_MaxTransitionPathSize ];
		size_t						m_transitionPathSize;

		int							m_currentStep;
		bool						m_isCreating;
		bool						m_isInitial;

	};
}

#endif // TIKI_STATETREE_HPP__INCLUDED
