#pragma once
#ifndef TIKI_TESTSTATE_HPP__INCLUDED
#define TIKI_TESTSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/animation/animationjoint.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/runtimeshared/freecamera.hpp"
#include "tiki/gameplay/gameclient.hpp"
#include "tiki/graphics/skinningdata.hpp"
#include "tiki/renderer/postbloom.hpp"
#include "tiki/renderer/postskybox.hpp"

#include "tiki/game/debuggui_lighting.hpp"
#include "tiki/game/debuggui_testwindow.hpp"

namespace tiki
{
	class Animation;
	class ApplicationState;
	class Font;
	class Game;
	class GameRenderer;
	class Model;
	class Texture;

	enum TestStateTransitionSteps
	{
		TestStateTransitionSteps_Initialize,
		TestStateTransitionSteps_LoadResources,
		TestStateTransitionSteps_CreateGameClient,
		TestStateTransitionSteps_SetRendererValues,

		TestStateTransitionSteps_Count
	};

	class TestState : public GameFlowState
	{
		TIKI_NONCOPYABLE_CLASS( TestState );

	public:

								TestState();
		virtual					~TestState();

		void					create( Game* pGame, ApplicationState* pParentState );
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital ) TIKI_OVERRIDE TIKI_FINAL;

		virtual void			update() TIKI_OVERRIDE TIKI_FINAL;
		virtual void			render( GraphicsContext& graphicsContext ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			postRender( GraphicsContext& graphicsContext ) TIKI_OVERRIDE TIKI_FINAL;

		virtual bool			processInputEvent( const InputEvent& inputEvent ) TIKI_OVERRIDE TIKI_FINAL;

	private:
		
		Game*						m_pGame;
		ApplicationState*			m_pParentState;

		const Font*					m_pFont;
		const Font*					m_pFontBig;
		const Model*				m_pModelBox;
		const Model*				m_pModelBoxes;
		const Model*				m_pModelPlane;
		const Model*				m_pModelPlayer;
		const Animation*			m_pAnimationPlayer;

		bool						m_drawPlayer;
		Array< AnimationJoint >		m_animationData;
		SkinningData				m_skinningData;

		GameRenderer*				m_pGameRenderer;
		FreeCamera					m_freeCamera;

		DebugGuiTestWindow			m_testWindow;
		DebugGuiLightingWindow		m_lightingWindow;

		int							m_gbufferIndex;

		bool						m_enablePhysicsDebug;
		bool						m_enableBloom;
		PostProcessBloom			m_bloom;
		PostProcessSkybox			m_skybox;

		GameClient					m_gameClient;

		EntityId					m_boxesEntityId;
		EntityId					m_planeEntityId;

	};
}

#endif // TIKI_TESTSTATE_HPP__INCLUDED
