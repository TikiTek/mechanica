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

			float					scale;
			float					alpha;

			Vector2					position;
		};

		using CircleArray = FixedSizedArray< Circle, MaxCircleCount >;

		struct MenuEntry
		{
			using CircleArray = FixedArray< Circle, 6u >;

			float					angle;
			CircleArray				circles;
		};

		using MenuEntryArray = FixedSizedArray< MenuEntry, MaxCircleCount >;

		Game*									m_pGame;
		ApplicationState*						m_pParentState;

		const MenuBundleGenericDataResource*	m_pBundle;

		uint2									m_mouseState;
		float									m_alpha;

		CircleArray								m_mainCircles;
		CircleArray								m_connectionCircles;
		MenuEntryArray							m_menuEntries;

		void									clearCircle( Circle& circle );

		void									createMainCircles();
		void									createMenuCircles( const ResArray< MenuElement >& elements );
		void									createConnectionCircles( uint count );

		void									updateCircle( Circle& circle, GameTime time );
		void									renderCircle( Renderer2d& renderer, const Circle& circle );
	};
}
