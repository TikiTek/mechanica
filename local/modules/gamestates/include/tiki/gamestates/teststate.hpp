#pragma once
#ifndef TIKI_TESTSTATE_HPP__INCLUDED
#define TIKI_TESTSTATE_HPP__INCLUDED

#include "tiki/gameflow/gamestate.hpp"

namespace tiki
{
	class ApplicationState;
	class Model;
	class SamplerState;
	class ShaderSet;
	class Texture;
	class VertexInputBinding;

	enum TestStateTransitionSteps
	{
		TestStateTransitionSteps_LoadResources,

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

		const ShaderSet*			m_pShaderSet;
		const Model*				m_pModel;

		const Texture*				m_pTexture;
		const SamplerState*			m_pSampler;

		const VertexInputBinding*	m_pInputBinding;

	};
}

#endif // TIKI_TESTSTATE_HPP__INCLUDED
