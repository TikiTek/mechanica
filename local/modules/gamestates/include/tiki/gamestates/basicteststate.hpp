#pragma once
#ifndef __TIKI_BASICSTATE_HPP_INCLUDED__
#define __TIKI_BASICSTATE_HPP_INCLUDED__

#include "tiki/gameflow/gameflowstate.hpp"

#include "tiki/base/types.hpp"
#include "tiki/graphics/immediaterenderer.hpp"

namespace tiki
{
	class Font;
	class Texture;

	enum BasicTestStateTransitionSteps
	{
		BasicTestStateTransitionSteps_Count
	};

	class BasicTestState : public GameFlowState
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( BasicTestState );

	public:								

		void					create();
		void					dispose();

		virtual TransitionState	processTransitionStep( size_t currentStep, bool isCreating, bool isInital );

		virtual void			update();
		virtual void			render( GraphicsContext& graphicsContext );

		virtual bool			processInputEvent( const InputEvent& inputEvent );

	private:

		 ImmediateRenderer		m_renderer;

		 const Texture*			m_pTestTexture;
		 const Font*			m_pTestFont;

	};
}

#endif // __TIKI_BASICSTATE_HPP_INCLUDED__
