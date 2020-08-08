#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/fixed_sized_array.hpp"

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
		uintreg		stateHierarchy[ StateTreeLimits_MaxHierarchyDepth ];
		uintreg		hierarchyLength;

		uintreg		transitionStepCount;

		const char*	pName;
	};

	class StateTree
	{
		TIKI_NONCOPYABLE_CLASS( StateTree );

	public:

		StateTree();

		void						create( StateDefinition* pStateDefinitions, uintreg stateCount );
		void						dispose();

		void						startTransition( uintreg stateIndex );

		void						updateTree( TransitionState newState );

		uintreg						getActiveStates( uintreg* pTargetActiveStates, uintreg capacity ) const;

		bool						isInState( uintreg stateIndex ) const;
		uintreg						getCurrentState() const				{ return ( isInTransition() ? m_transition.currentState : m_currentState ); }
		uintreg						getTransitionState() const			{ return m_transition.getCurrentTransitionState(); }
		bool						isInTransition() const				{ return m_currentState == InvalidStateIndex; }

		uintreg						getCurrentStep() const	{ return m_transition.currentStep; }
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

			typedef FixedSizedArray< uintreg, StateTreeLimits_MaxTransitionPathSize > TransitionPathArray;

			uintreg				sourceState;
			uintreg				destinationState;
			uintreg				currentState;

			TransitionPathArray	path;
			uintreg				pathIndex;

			uintreg				targetStep;
			uintreg				currentStep;

			bool				isInForwardTransition() const		{ return currentState < path[ pathIndex ] && currentState != path[ pathIndex ]; }
			uintreg				getCurrentTransitionState() const	{ return isInForwardTransition() ? path[ pathIndex ] : currentState; }

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
		//uintreg						m_transitionCurrentPathIndex;
		//int							m_transitionPath[ StateTree_MaxTransitionPathSize ];
		//uintreg						m_transitionPathSize;

		uintreg						m_currentState;
		bool						m_isInitial;

		StateTreeTransition			m_transition;
	};
}
