#pragma once
#ifndef TIKI_TESTSTATE_HPP__INCLUDED
#define TIKI_TESTSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/animation/animationjoint.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/framework/freecamera.hpp"
#include "tiki/gameplay/gameclient.hpp"
#include "tiki/gameplay/playercamera.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/graphics/skinningdata.hpp"
#include "tiki/renderer/fallbackrendereffect.hpp"
#include "tiki/renderer/postascii.hpp"
#include "tiki/renderer/postbloom.hpp"
#include "tiki/renderer/scenerendereffect.hpp"

#include "tiki/game/debuggui_lighting.hpp"
#include "tiki/game/debuggui_testwindow.hpp"

namespace tiki
{
	class Animation;
	class ApplicationState;
	class Font;
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
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( TestState );

	public:								

		void					create( ApplicationState* pParentState );
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext );

		virtual bool			processInputEvent( const InputEvent& inputEvent );

	private:
		
		ApplicationState*			m_pParentState;

		const Font*					m_pFont;
		const Model*				m_pModelBox;
		const Model*				m_pModelBoxes;
		const Model*				m_pModelPlane;
		const Model*				m_pModelPlayer;
		const Animation*			m_pAnimationPlayer;

		bool						m_drawPlayer;
		Array< AnimationJoint >		m_animationData;
		SkinningData				m_skinningData;

		GameRenderer*				m_pGameRenderer;
		FallbackRenderEffect		m_fallbackRenderEffect;
		SceneRenderEffect			m_sceneRenderEffect;

		ImmediateRenderer			m_immediateRenderer;
		
		bool						m_enableFreeCamera;
		FreeCamera					m_freeCamera;
		PlayerCamera				m_playerCamera;

		DebugGui					m_debugGui;
		DebugGuiTestWindow			m_testWindow;
		DebugGuiLightingWindow		m_lightingWindow;

		int							m_gbufferIndex;

		bool						m_enableAsciiMode;
		bool						m_enableBloom;
		PostProcessAscii			m_ascii;
		PostProcessBloom			m_bloom;

		GameClient					m_gameClient;
		
		EntityId					m_planeEntityId;
		EntityId					m_playerEntityId;
		EntityId					m_boxesEntityId;
		SizedArray< EntityId >		m_boxEntities;

	};
}

#endif // TIKI_TESTSTATE_HPP__INCLUDED
