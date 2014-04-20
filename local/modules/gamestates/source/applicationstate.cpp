
#include "tiki/gamestates/applicationstate.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texture.hpp"

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
					Font::registerResourceType( resourceManager, graphicsSystem );
					Material::registerResourceType( resourceManager );
					Model::registerResourceType( resourceManager, graphicsSystem );
					ShaderSet::registerResourceType( resourceManager, graphicsSystem );
					Texture::registerResourceType( resourceManager, graphicsSystem );
				}
				else
				{
					Font::unregisterResourceType( resourceManager );
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

					if ( m_renderer.create( framework::getGraphicsSystem(), framework::getResourceManager(), params ) == false )
					{
						TIKI_TRACE_ERROR( "[applicationstate] Could not create GameRenderer.\n" );
						return TransitionState_Error;
					}
				}
				else
				{
					m_renderer.dispose( framework::getResourceManager() );
				}

				return TransitionState_Finish;
			}
			break;

		}

		return TransitionState_Error;
	}

	void ApplicationState::update()
	{
		const WindowEvent* pEvent = framework::getMainWindow().getEventBuffer().getEventByType( WindowEventType_SizeChanged );
		if ( pEvent != nullptr )
		{
			framework::getGraphicsSystem().resize( pEvent->data.sizeChangedEvent.size.x, pEvent->data.sizeChangedEvent.size.y );
			if ( !m_renderer.resize( pEvent->data.sizeChangedEvent.size.x, pEvent->data.sizeChangedEvent.size.y ) )
			{
				m_renderer.dispose( framework::getResourceManager() );				
			}
		}

		m_renderer.update();
	}

	void ApplicationState::render( GraphicsContext& graphicsContext )
	{
		m_renderer.render( graphicsContext );
	}

	bool ApplicationState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}
}