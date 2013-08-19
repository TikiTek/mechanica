#pragma once
#ifndef TIKI_TESTSTATE_HPP__INCLUDED
#define TIKI_TESTSTATE_HPP__INCLUDED

#include "tiki/gameflow/gamestate.hpp"

#include "tiki/gamecore/freecamera.hpp"

namespace tiki
{
	class Animation;
	class AnimationComponent;
	class ApplicationState;
	class Font;
	class Model;
	class SkinnedModelComponent;
	class Transform;

	enum TestStateTransitionSteps
	{
		TestStateTransitionSteps_LoadResources,
		TestStateTransitionSteps_CreateConponents,
		TestStateTransitionSteps_InitializeConponents,

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
		virtual void			render( GpuContext* pCommandWriter ) const;

	private:
		
		ApplicationState*		m_pParentState;

		FreeCamera				m_freeCamera;

		enum 
		{
			testCount = 3
		};

		const Font*				m_pFont;

		Transform*				m_pTransform[ testCount ];
		SkinnedModelComponent*	m_pModelComponent[ testCount ];
		AnimationComponent*		m_pAnimationComponent[ testCount ];

		const Model*			m_pHelm;
		const Model*			m_pShoulderL;
		const Model*			m_pShoulderR;

		Transform*				m_pAttTrans[ testCount * 3 ];
		StaticModelComponent*	m_pAttModel[ testCount * 3];
		ItemAttachment*			m_pAtt[ testCount * 3 ];

		const Model*			m_pModel[ testCount ];
		const Animation*		m_pAnimation[ testCount ];

	};
}

#endif // TIKI_TESTSTATE_HPP__INCLUDED
