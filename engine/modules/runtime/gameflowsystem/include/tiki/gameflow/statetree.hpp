#pragma once
#ifndef TIKI_STATETREE_HPP__INCLUDED
#define TIKI_STATETREE_HPP__INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/fixedsizedarray.hpp"

namespace tiki
{
	enum
	{
		StateTreeLimits_MaxStateCount			= 16u,
		StateTreeLimits_MaxHierarchyDepth		= 8u,
		StateTreeLimits_MaxTransitionPathSize	= 16u * 2u
	};

	enum
	{
		InvalidTransitionStep	= TIKI_SIZE_T_MAX,
		InvalidStateIndex		= TIKI_SIZE_T_MAX
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
		uint		stateHierarchy[ StateTreeLimits_MaxHierarchyDepth ];
		uint		hierarchyLength;

		uint		transitionStepCount;

		const char*	pName;
	};

	class StateTree
	{
		TIKI_NONCOPYABLE_CLASS( StateTree );

	public:

		StateTree();

		void						create( StateDefinition* pStateDefinitions, uint stateCount );
		void						dispose();

		void						startTransition( uint stateIndex );

		void						updateTree( TransitionState newState );

		uint						getActiveStates( uint* pTargetActiveStates, uint capacity ) const;

		bool						isInState( uint stateIndex ) const;
		uint						getCurrentState() const				{ return ( isInTransition() ? m_transition.currentState : m_currentState ); }
		uint						getTransitionState() const			{ return m_transition.getCurrentTransitionState(); }
		bool						isInTransition() const				{ return m_currentState == InvalidStateIndex; }

		uint						getCurrentStep() const	{ return m_transition.currentStep; }
		bool						isCreating() const		{ return m_transition.isInForwardTransition(); }
		bool						isInitial() const		{ return m_isInitial; }

	private:

		struct StateTreeTransition
		{
			TIKI_NONCOPYABLE_STRUCT( StateTreeTransition );

			StateTreeTransition()
			{
				clear();
			}
			
			typedef FixedSizedArray< uint, StateTreeLimits_MaxTransitionPathSize > TransitionPathArray;

			uint				sourceState;
			uint				destinationState;
			uint				currentState;

			TransitionPathArray	path;
			uint				pathIndex;

			uint				targetStep;
			uint				currentStep;

			bool				isInForwardTransition() const		{ return currentState < path[ pathIndex ] && currentState != path[ pathIndex ]; }
			uint				getCurrentTransitionState() const	{ return isInForwardTransition() ? path[ pathIndex ] : currentState; }

			void				clear()
			{
				sourceState			= 0u;
				destinationState	= 0u;
				currentState		= TIKI_SIZE_T_MAX;

				path.clear();
				pathIndex			= TIKI_SIZE_T_MAX;

				targetStep			= 0u;
				currentStep			= 0u;
			}
		};

		Array< StateDefinition >	m_stateDefinition;

		//int							m_transitionSourceState;
		//int							m_transitionNextState;
		//int							m_transitionPathDirection;
		//uint						m_transitionCurrentPathIndex;
		//int							m_transitionPath[ StateTree_MaxTransitionPathSize ];
		//uint						m_transitionPathSize;

		uint						m_currentState;
		bool						m_isInitial;

		StateTreeTransition			m_transition;

	};
}

#endif // TIKI_STATETREE_HPP__INCLUDED
