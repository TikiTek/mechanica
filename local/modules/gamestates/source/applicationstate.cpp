
#include "tiki/gamestates/applicationstate.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphicsresources/material.hpp"
#include "tiki/graphicsresources/model.hpp"
#include "tiki/graphicsresources/shaderset.hpp"
#include "tiki/graphicsresources/texture.hpp"

namespace tiki
{
	void ApplicationState::create()
	{
	}

	void ApplicationState::dispose()
	{
	}

	TransitionState ApplicationState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case ApplicationStateTransitionSteps_RegisterResourceTypes:
			{
				ResourceManager& resourceManager = framework::getResourceManager();
				GraphicsSystem& graphicsSystem = framework::getGraphicsSystem();

				if ( isInital )
				{
					Material::registerResourceType( resourceManager );
					Model::registerResourceType( resourceManager, graphicsSystem );
					ShaderSet::registerResourceType( resourceManager, graphicsSystem );
					Texture::registerResourceType( resourceManager, graphicsSystem );
				}
				else
				{
					Material::unregisterResourceType( resourceManager );
					Model::unregisterResourceType( resourceManager );
					ShaderSet::unregisterResourceType( resourceManager );
					Texture::unregisterResourceType( resourceManager );
				}

				return TransitionState_Finish;
			}
			//break;
		case ApplicationStateTransitionSteps_CreateGameRenderer:
			{
				TIKI_ASSERT( isInital );

				if ( isCreating )
				{
					GameRendererParamaters params;

					if ( m_renderer.create( framework::getGraphicsSystem(), params ) == false )
					{
						TIKI_TRACE_ERROR( "[applicationstate] Could not create GameRenderer.\n" );
						return TransitionState_Error;
					}
				}
				else
				{
					m_renderer.dispose();
				}

				return TransitionState_Finish;
			}
			break;

		}

		return TransitionState_Error;
	}

	void ApplicationState::update()
	{
		m_renderer.update();
	}

	void ApplicationState::render( GraphicsContext& graphicsContext ) const
	{
		m_renderer.render( graphicsContext );
	}
}