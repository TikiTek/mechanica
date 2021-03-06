#include "tiki/game_states/test_state.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/debug_renderer/debug_renderer.hpp"
#include "tiki/framework/main_window.hpp"
#include "tiki/game/game.hpp"
#include "tiki/game_states/application_state.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphics_context.hpp"
#include "tiki/graphics/graphics_system.hpp"
#include "tiki/graphics/shader/graphicstypes.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/input/input_system.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/resource/resource_request_pool.hpp"

namespace tiki
{
	TestState::TestState()
	{
		m_pGame			= nullptr;
		m_pApplicationState	= nullptr;

		vector::clear( m_mousePosition );
	}

	TestState::~TestState()
	{
		TIKI_ASSERT( m_pGame == nullptr );
		TIKI_ASSERT( m_pApplicationState == nullptr );
	}

	void TestState::create( Game* pGame, ApplicationState* pApplicationState )
	{
		m_pGame				= pGame;
		m_pApplicationState	= pApplicationState;

		m_pFont				= nullptr;
	}

	void TestState::dispose()
	{
		m_pGame = nullptr;
		m_pApplicationState = nullptr;
	}

	TransitionState TestState::processTransitionStep( uintreg currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case TestStateTransitionSteps_Initialize:
			if ( isCreating )
			{
				m_pRenderer = &m_pApplicationState->getRenderer();

				return TransitionState_Finish;
			}
			else
			{
				m_pRenderer = nullptr;

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				ResourceRequestPool& resourceRequestPool = m_pGame->getResourceRequestPool();

				if ( isInital )
				{
					resourceRequestPool.beginLoadResource< Font >( &m_pFont,									"debug.font" );
					resourceRequestPool.beginLoadResource< EntityTemplateGenericDataResource >( &m_pIsland1,	"island8_p.entity" );
					resourceRequestPool.beginLoadResource< EntityTemplateGenericDataResource >( &m_pIsland2,	"island2.entity" );
					resourceRequestPool.beginLoadResource< EntityTemplateGenericDataResource >( &m_pIsland3,	"island4.entity" );
				}

				if ( resourceRequestPool.isFinish() )
				{
					if ( resourceRequestPool.hasError() )
					{
						return TransitionState_Error;
					}

					return TransitionState_Finish;
				}

				return TransitionState_InProcess;
			}
			else
			{
				TIKI_ASSERT( isInital );

				ResourceManager& resourceManager = m_pGame->getResourceManager();

				resourceManager.unloadResource( m_pIsland1 );
				resourceManager.unloadResource( m_pIsland2 );
				resourceManager.unloadResource( m_pIsland3 );
				resourceManager.unloadResource( m_pFont );

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_CreateGameClient:
			if ( isCreating )
			{
				TIKI_VERIFY( m_gameClient.create() );

				m_playerEntityId = m_gameClient.createPlayerEntity( vector::create( 0.0, -2.0f ) );
				m_islandEntityId = m_gameClient.createEntityFromTemplate( m_pIsland1->getData() );

				return TransitionState_Finish;
			}
			else
			{
				m_gameClient.disposeEntity( m_playerEntityId );
				m_gameClient.disposeEntity( m_islandEntityId );

				m_gameClient.dispose();

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_SetRendererValues:
			{
				if ( isCreating )
				{


					return TransitionState_Finish;
				}
				else
				{

					return TransitionState_Finish;
				}
			}
			//break;

		default:
			break;
		}

		TIKI_BREAK( "no result\n" );
		return TransitionState_Error;
	}

	void TestState::update()
	{
		const float timeDelta = float( m_pGame->getFrameTimer().getElapsedTime() );

		MechanicaUpdateContext gameClientUpdateContext;
		gameClientUpdateContext.gameTime = m_pGame->getFrameTimer().getTime();
		m_gameClient.update( gameClientUpdateContext );

#if TIKI_ENABLED( TIKI_DEBUG_RENDERER )
		debugrenderer::drawText( Vector2::zero, TIKI_COLOR_GREEN, " Zoom: %.2f, FPS: %.2f", m_pRenderer->getTargetZoom(), 1.0f / timeDelta );
#endif
	}

	void TestState::render( GraphicsContext& graphicsContext )
	{
		m_gameClient.render( *m_pRenderer );
	}

	void TestState::postRender( GraphicsContext& graphicsContext )
	{
//#if TIKI_DISABLED( TIKI_BUILD_MASTER )
//		const ImmediateRenderer& immediateRenderer = m_pGame->getImmediateRenderer();
//
//
//
//		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_BLACK );
//
//		immediateRenderer.beginRenderPass();
//
//		if ( m_enableBloom )
//		{
//			const AxisAlignedRectangle rect = createAxisAlignedRectangle( 0.0f, 0.0f, (float)m_bloom.getResultData().getWidth() * 2.0f, (float)m_bloom.getResultData().getHeight() * 2.0f );
//			immediateRenderer.drawTexturedRectangle( m_bloom.getResultData(), rect );
//		}
//
//		//const Rectangle rect2 = Rectangle( 50.0f, 50.0f, (float)m_pFont->getTextureData().getWidth(), (float)m_pFont->getTextureData().getHeight() );
//		//immediateRenderer.drawTexture( &m_pFont->getTextureData(), rect2 );
//
//		const float timeDelta = (float)m_pGame->getFrameTimer().getElapsedTime();
//
//		char buffer[ 128u ];
//		formatStringBuffer( buffer, TIKI_COUNT( buffer ), " FPS: %.2f", 1.0f / timeDelta );
//
//		immediateRenderer.drawText( Vector2::zero, *m_pFont, buffer, TIKI_COLOR_GREEN );
//
//		immediateRenderer.endRenderPass();
//
//
//#endif
	}

	bool TestState::processInputEvent( const InputEvent& inputEvent )
	{
		if( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			vector::set( m_mousePosition, float( inputEvent.data.mouseMoved.xState ), float( inputEvent.data.mouseMoved.yState ) );
		}

		if ( m_gameClient.processInputEvent( inputEvent ) )
		{
			return true;
		}

		if (inputEvent.eventType == InputEventType_Mouse_Wheel)
		{
			const float wheelDelta = (float)inputEvent.data.mouseWheel.offset / 1200.0f;
			const float targetZoom = m_pRenderer->getTargetZoom() + wheelDelta;
			m_pRenderer->setTargetZoom( targetZoom );
		}
		if( inputEvent.eventType == InputEventType_Keyboard_Down )
		{
			switch( inputEvent.data.keyboardKey.key )
			{
			case KeyboardKey_X:
				{
					BreakableComponent& breakableComponent = m_gameClient.getBreakableComponent();

					BreakableComponentState* pBreakableState = (BreakableComponentState*)m_gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_islandEntityId, breakableComponent.getTypeId() );
					breakableComponent.breakBody( pBreakableState );
				}
				break;

			default:
				break;
			}
		}
		//else if( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Right )
		//{
		//	m_freeCamera.setMouseControl( !m_freeCamera.getMouseControl() );
		//}

		return false;
	}
}