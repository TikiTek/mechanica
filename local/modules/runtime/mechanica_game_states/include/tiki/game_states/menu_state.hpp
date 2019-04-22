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
		MenuStateTransitionSteps_Initialize,

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

		static const uint MaxCircleCount = 8u;

		struct Circle
		{
			const TextureData*		pTexture;

			float					angle;

			float					angleMin;
			float					angleMax;
			float					angleSpeed;

			Vector2					position;
		};

		using CircleArray = FixedSizedArray< Circle, MaxCircleCount >;

		using MenuEntry = FixedArray< Circle, 6u >;
		using MenuEntryArray = FixedSizedArray< MenuEntry, MaxCircleCount >;


		Game*									m_pGame;
		ApplicationState*						m_pParentState;

		const MenuBundleGenericDataResource*	m_pBundle;

		uint2									m_mouseState;

		CircleArray								m_mainCircles;
		MenuEntryArray							m_menuEntries;

		void									createMainCircles();
		void									createMenuCircles( const ResArray< MenuElement >& elements );

		void									updateCircle( Circle& circle, GameTime time );
		void									renderCircle( Renderer2d& renderer, const Circle& circle );
	};
}
