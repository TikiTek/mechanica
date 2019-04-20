#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "tiki/math/camera.hpp"
#include "tiki/physics_2d/physics_2d_body.hpp"
#include "tiki/physics_2d/physics_2d_collider.hpp"
#include "tiki/physics_2d/physics_2d_joint.hpp"
#include "tiki/physics_2d/physics_2d_world.hpp"

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

		Physics2dBody			m_playerBox;
		Physics2dBody			m_playerCircle;
		Physics2dJoint			m_playerJoint;

		Physics2dBody			m_body1;
		Physics2dBody			m_body2;
		Physics2dJoint			m_joint;
		Physics2dCollider		m_collider;

		Physics2dCollider		m_colliders[ 4u ];
		Physics2dJoint			m_joints[ 4u ];

		bool					m_mouseRightState;
	};
}
