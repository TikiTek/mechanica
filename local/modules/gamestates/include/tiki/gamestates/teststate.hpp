#pragma once
#ifndef TIKI_TESTSTATE_HPP__INCLUDED
#define TIKI_TESTSTATE_HPP__INCLUDED

#include "tiki/gameflow/gamestate.hpp"

#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/renderer/fallbackrendereffect.hpp"

namespace tiki
{
	class ApplicationState;
	class GameRenderer;
	class Model;
	class SamplerState;
	class ShaderSet;
	class Texture;
	class VertexFormat;
	class VertexInputBinding;

	enum TestStateTransitionSteps
	{
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
		virtual void			render( GraphicsContext& graphicsContext ) const;

	private:
		
		ApplicationState*			m_pParentState;

		const Model*				m_pModel;
		const ShaderSet*			m_pShaderSet;
		const Texture*				m_pTexture;

		GameRenderer*				m_pGameRenderer;
		FallbackRenderEffect		m_fallbackRenderEffect;

		//const VertexFormat*			m_pVertexFormat;
		const VertexInputBinding*	m_pInputBinding;
		const SamplerState*			m_pSampler;

	};
}

#endif // TIKI_TESTSTATE_HPP__INCLUDED
