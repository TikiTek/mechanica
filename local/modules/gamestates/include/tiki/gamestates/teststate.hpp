#pragma once
#ifndef TIKI_TESTSTATE_HPP__INCLUDED
#define TIKI_TESTSTATE_HPP__INCLUDED

#include "tiki/gameflow/gamestate.hpp"

#include "tiki/components/componentstorage.hpp"
#include "tiki/components/staticmodelcomponent.hpp"
#include "tiki/components/typeregister.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/renderer/fallbackrendereffect.hpp"

namespace tiki
{
	class ApplicationState;
	class Font;
	class GameRenderer;
	class Model;
	class Texture;

	enum TestStateTransitionSteps
	{
		TestStateTransitionSteps_Initialize,
		TestStateTransitionSteps_LoadResources,
		TestStateTransitionSteps_SetRendererValues,

		TestStateTransitionSteps_Count
	};

	class TestState : public GameState
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
		const Model*				m_pModel;
		const Texture*				m_pTexture;

		GameRenderer*				m_pGameRenderer;
		FallbackRenderEffect		m_fallbackRenderEffect;

		ComponentStorage			m_storage;
		ComponentTypeRegister		m_typeRegister;

		ComponentTypeId				m_staticModelComponentTypeId;
		StaticModelComponent		m_staticModelComponent;

		ImmediateRenderer			m_immediateRenderer;

		Vector2						m_leftStickState;
		Vector2						m_rightStickState;

	};
}

#endif // TIKI_TESTSTATE_HPP__INCLUDED
