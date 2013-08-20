#pragma once
#ifndef TIKI_PLAYSTATE_HPP__INCLUDED
#define TIKI_PLAYSTATE_HPP__INCLUDED

#include "tiki/gameflow/gamestate.hpp"

namespace tiki
{
	class ApplicationState;

	enum PlayStateTransitionSteps
	{
		PlayStateTransitionSteps_InitializeBase,
		PlayStateTransitionSteps_LoadResources,
		PlayStateTransitionSteps_CreateComponents,
		PlayStateTransitionSteps_InitializeComponents,
		PlayStateTransitionSteps_CreateEntities,
		PlayStateTransitionSteps_CreateLevel,

		PlayStateTransitionSteps_Count
	};

	class PlayState : public GameState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( PlayState );

	public:

		void							create( ApplicationState* pParentState );
		void							dispose();

		virtual TransitionState			processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void					update();
		virtual void					render( GpuContext* pCommandWriter ) const;

	private:
		
		ApplicationState*				m_pParentState;

	};
}

#endif // TIKI_PLAYSTATE_HPP__INCLUDED
