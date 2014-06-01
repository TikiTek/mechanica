#pragma once
#ifndef TIKI_TESTSTATE_HPP__INCLUDED
#define TIKI_TESTSTATE_HPP__INCLUDED

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/animation/animationjoint.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/framework/freecamera.hpp"
#include "tiki/gameplay/gameclient.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/graphics/skinningdata.hpp"
#include "tiki/renderer/postbloom.hpp"

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
		ImmediateRenderer			m_immediateRenderer;
		FreeCamera					m_freeCamera;

		DebugGui					m_debugGui;
		DebugGuiTestWindow			m_testWindow;
		DebugGuiLightingWindow		m_lightingWindow;

		int							m_gbufferIndex;

		bool						m_enablePhysicsDebug;
		bool						m_enableBloom;
		PostProcessBloom			m_bloom;

		GameClient					m_gameClient;

		EntityId					m_boxesEntityId;
		EntityId					m_planeEntityId;

	};
}

#endif // TIKI_TESTSTATE_HPP__INCLUDED
