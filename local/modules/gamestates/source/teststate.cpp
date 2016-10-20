#include "tiki/gamestates/teststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/game/game.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/graphicstypes.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/math/axisalignedrectangle.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

namespace tiki
{
	struct FallbackVertex
	{
		float3	position;
		float4	color;
		float2	texCoord;
	};
	
	TestState::TestState()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;

		vector::clear( m_mousePosition );
	}

	TestState::~TestState()
	{
		TIKI_ASSERT( m_pGame == nullptr );
		TIKI_ASSERT( m_pParentState == nullptr );
	}

	void TestState::create( Game* pGame, ApplicationState* pParentState )
	{
		m_pGame			= pGame;
		m_pParentState	= pParentState;

		m_pFont					= nullptr;
		m_pFontBig				= nullptr;

		m_enableBloom			= true;
		m_enablePhysicsDebug	= false;
	}

	void TestState::dispose()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;
	}

	TransitionState TestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case TestStateTransitionSteps_Initialize:
			if ( isCreating )
			{
				PostProcessBloomParameters bloomParameters;	
				bloomParameters.width		= m_pGame->getGraphicsSystem().getBackBuffer().getWidth() / 2u;
				bloomParameters.height		= m_pGame->getGraphicsSystem().getBackBuffer().getHeight() / 2u;
				bloomParameters.passCount	= 6u;
				TIKI_VERIFY( m_bloom.create(
					m_pGame->getGraphicsSystem(),
					m_pGame->getResourceManager(),
					bloomParameters
				) );

				return TransitionState_Finish;
			}
			else
			{
				m_bloom.dispose(
					m_pGame->getGraphicsSystem(),
					m_pGame->getResourceManager()
				);

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
					resourceRequestPool.beginLoadResource< Font >( &m_pFontBig,									"big.font" );
					resourceRequestPool.beginLoadResource< EntityTemplateGenericDataResource >( &m_pIsland1,	"island1.entity" );
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
				resourceManager.unloadResource( m_pFontBig );
				resourceManager.unloadResource( m_pFont );
				
				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_CreateGameClient:
			if ( isCreating )
			{
				TIKI_VERIFY( m_gameClient.create( m_pGame->getGraphicsSystem() ) );

				m_boxesEntityId		= m_gameClient.createPlayerEntity( Vector2::zero );
				m_planeEntityId		= m_gameClient.createEntityFromTemplate( m_pIsland1->getData() );

				return TransitionState_Finish;
			}
			else
			{
				m_gameClient.disposeEntity( m_boxesEntityId );
				m_gameClient.disposeEntity( m_planeEntityId );

				m_gameClient.dispose();

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_SetRendererValues:
			{
				if ( isCreating )
				{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )					
					m_testWindow.create( m_pGame->getDebugGui() );
#endif
					//m_lightingWindow.create( m_pGame->getDebugGui() );

					m_testWindow.setRectangle( createAxisAlignedRectangle( 500.0, 40.0f, 200.0f, 400.0f ) );
					//m_lightingWindow.setRectangle( Rectangle( 1000.0, 100.0f, 250.0f, 100.0f ) );

					return TransitionState_Finish;
				}
				else
				{
					//m_lightingWindow.dispose();
					m_testWindow.dispose();

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
		const float timeDelta		= float( m_pGame->getFrameTimer().getElapsedTime() );
		const float totalGameTime	= float( m_pGame->getFrameTimer().getTotalTime() );
		const float timeValue		= (float)m_pGame->getFrameTimer().getTotalTime() / 10.0f;

		GameClientUpdateContext gameClientUpdateContext;
		gameClientUpdateContext.timeDelta		= timeDelta;
		gameClientUpdateContext.totalGameTime	= totalGameTime;
		m_gameClient.update( gameClientUpdateContext );
	}

	void TestState::render( GraphicsContext& graphicsContext )
	{
		m_gameClient.render( graphicsContext );
	}

	void TestState::postRender( GraphicsContext& graphicsContext )
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const ImmediateRenderer& immediateRenderer = m_pGame->getImmediateRenderer();

		if ( m_enableBloom )
		{
			//m_bloom.render( graphicsContext, m_pGameRenderer->getAccumulationBuffer(), m_pGameRenderer->getGeometryBufferBxIndex( 2u ) );
		}

		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_BLACK );

		immediateRenderer.beginRenderPass();

		if ( m_enableBloom )
		{
			const AxisAlignedRectangle rect = createAxisAlignedRectangle( 0.0f, 0.0f, (float)m_bloom.getResultData().getWidth() * 2.0f, (float)m_bloom.getResultData().getHeight() * 2.0f );
			immediateRenderer.drawTexturedRectangle( m_bloom.getResultData(), rect );
		}

		//const Rectangle rect2 = Rectangle( 50.0f, 50.0f, (float)m_pFont->getTextureData().getWidth(), (float)m_pFont->getTextureData().getHeight() );
		//immediateRenderer.drawTexture( &m_pFont->getTextureData(), rect2 );

		const float timeDelta = (float)m_pGame->getFrameTimer().getElapsedTime();

		char buffer[ 128u ];
		formatStringBuffer( buffer, TIKI_COUNT( buffer ), " FPS: %.2f", 1.0f / timeDelta );

		immediateRenderer.drawText( Vector2::zero, *m_pFont, buffer, TIKI_COLOR_GREEN );

		immediateRenderer.endRenderPass();

		if ( m_enablePhysicsDebug )
		{
			m_gameClient.getPhysicsWorld().renderDebug();
		}
#endif
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

		if ( inputEvent.eventType == InputEventType_Keyboard_Down )
		{
			switch ( inputEvent.data.keybaordKey.key )
			{
			case KeyboardKey_B:
				m_enableBloom = !m_enableBloom;
				return true;

			case KeyboardKey_F2:
				m_enablePhysicsDebug = !m_enablePhysicsDebug;
				break;

			default:
				break;
			}
		}
		//else if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Right )
		//{
		//	m_freeCamera.setMouseControl( !m_freeCamera.getMouseControl() );
		//}

		return false;
	}

	const Camera& TestState::getCamera() const
	{
		return m_gameClient.getCamera();
	}
}