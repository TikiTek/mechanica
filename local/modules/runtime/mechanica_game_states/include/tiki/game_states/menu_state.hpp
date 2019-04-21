#pragma once

#include "tiki/game_flow/game_flow_state.hpp"

#include "mechanica_bundles.hpp"

namespace tiki
{
	class ApplicationState;
	class Game;

	enum MenuStateTransitionSteps
	{
		MenuStateTransitionSteps_LoadResources,

		MenuStateTransitionSteps_Count
	};

	class MenuState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( MenuState );

	public:

									MenuState();
		virtual						~MenuState();

		void						create( Game* pGame, ApplicationState* pParentState );
		void						dispose();

		virtual TransitionState		processCreationStep( size_t currentStep, bool isInital ) TIKI_OVERRIDE_FINAL;
		virtual TransitionState		processDestructionStep( size_t currentStep, bool isInital ) TIKI_OVERRIDE_FINAL;

		virtual void				update() TIKI_OVERRIDE;
		virtual void				render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE_FINAL;

		virtual bool				processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE_FINAL;

	private:

		static const uint MaxCircleCount = 24u;

		struct Circle
		{
			float					angleMin;
			float					angleMax;
			float					angleSpeed;

			Vector2					position;
		};

		using CircleArray = FixedSizedArray< Circle, MaxCircleCount >;

		Game*									m_pGame;
		ApplicationState*						m_pParentState;

		const MenuBundleGenericDataResource*	m_pBundle;

		CircleArray								m_circles;

		void									updateCircle( Circle& circle, double deltaTime );
		void									renderCircle( const ImmediateRenderer& renderer, const Circle& circle );
	};
}
