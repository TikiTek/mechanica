
#include "tiki/gamestates/teststate.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphicsresources/model.hpp"
#include "tiki/graphicsresources/shaderset.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	void TestState::create( ApplicationState* pParentState )
	{
		m_pParentState	= pParentState;
	}

	void TestState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState TestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case TestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				if ( isInital )
				{
					m_pShaderSet	= framework::getResourceManager().loadResource< ShaderSet >( "fallback.shader" );
					m_pModel		= framework::getResourceManager().loadResource< Model >( "replaceme_cube.model" );

					return TransitionState_Finish;
				}
				else
				{
					// TODO: make resource manager async

					return TransitionState_Error;
				}
			}
			else
			{
				TIKI_ASSERT( isInital );

				return TransitionState_Finish;
			}
			break;
		default:
			break;
		}

		TIKI_BREAK( "no result\n" );
		return TransitionState_Error;
	}

	void TestState::update()
	{
	}

	void TestState::render( GraphicsContext* pGraphicsContext ) const
	{
	}
}