#pragma once
#ifndef TIKI_PHYSICSTESTSTATE_HPP_INCLUDED
#define TIKI_PHYSICSTESTSTATE_HPP_INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/physics2d/physics2dbody.hpp"
#include "tiki/physics2d/physics2dcollider.hpp"
#include "tiki/physics2d/physics2dworld.hpp"

namespace tiki
{
	class Game;

	enum PhysicsTestStateTransitionSteps
	{
		PhysicsTestStateTransitionSteps_Initialize,
		PhysicsTestStateTransitionSteps_LoadResources,

		PhysicsTestStateTransitionSteps_Count
	};

	class PhysicsTestState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsTestState );

	public:

								PhysicsTestState();
		virtual					~PhysicsTestState();

		void					create( Game* pGame );
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital ) TIKI_OVERRIDE_FINAL;

		virtual void			update() TIKI_OVERRIDE_FINAL;
		virtual void			render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;
		
		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;

		const Camera&			getCamera() const { return m_camera; }

	private:
		
		Game*					m_pGame;

		Camera					m_camera;

		Physics2dWorld			m_world;

		Physics2dBody			m_body;
		Physics2dCollider		m_collider;

		bool					m_mouseRightState;
	};
}

#endif // TIKI_PHYSICSTESTSTATE_HPP_INCLUDED
