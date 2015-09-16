
#include "tiki/gamestates/teststate.hpp"

#include "tiki/animation/animation.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/graphicstypes.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/physics/physicsboxshape.hpp"
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct FallbackVertex
	{
		float3	position;
		float4	color;
		float2	texCoord;
	};
	
	void TestState::create( ApplicationState* pParentState )
	{
		m_pParentState	= pParentState;

		m_pFont					= nullptr;
		m_pFontBig				= nullptr;
		m_pModelBox				= nullptr;
		m_pModelBoxes			= nullptr;
		m_pModelPlane			= nullptr;
		m_pModelPlayer			= nullptr;
		m_pAnimationPlayer		= nullptr;

		m_drawPlayer			= false;
		m_enableBloom			= true;
		m_gbufferIndex			= -1;
		m_enablePhysicsDebug	= false;
	}

	void TestState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState TestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case TestStateTransitionSteps_Initialize:
			if ( isCreating )
			{
				PostProcessBloomParameters bloomParameters;	
				bloomParameters.width		= framework::getGraphicsSystem().getBackBuffer().getWidth() / 2u;
				bloomParameters.height		= framework::getGraphicsSystem().getBackBuffer().getHeight() / 2u;
				bloomParameters.passCount	= 6u;
				TIKI_VERIFY( m_bloom.create(
					framework::getGraphicsSystem(),
					framework::getResourceManager(),
					bloomParameters
				) );

				TIKI_VERIFY( m_skybox.create(
					framework::getGraphicsSystem(),
					framework::getResourceManager()
				) );

				m_immediateRenderer.create( framework::getGraphicsSystem(), framework::getResourceManager() );

				return TransitionState_Finish;
			}
			else
			{
				m_immediateRenderer.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

				m_skybox.dispose(
					framework::getGraphicsSystem(),
					framework::getResourceManager()
				);

				m_bloom.dispose(
					framework::getGraphicsSystem(),
					framework::getResourceManager()
				);

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				if ( isInital )
				{
					m_pFont				= framework::getResourceManager().loadResource< Font >( "debug.font" );
					m_pFontBig			= framework::getResourceManager().loadResource< Font >( "big.font" );
					m_pModelBox			= framework::getResourceManager().loadResource< Model >( "box.model" );
					m_pModelBoxes		= framework::getResourceManager().loadResource< Model >( "spaceship.model" );
					m_pModelPlane		= framework::getResourceManager().loadResource< Model >( "plane.model" );
					m_pModelPlayer		= framework::getResourceManager().loadResource< Model >( "player.model" );
					m_pAnimationPlayer	= framework::getResourceManager().loadResource< Animation >( "player.run.animation" );
					
					bool ok = true;
					ok &= ( m_pFont != nullptr );
					ok &= ( m_pFontBig != nullptr );
					ok &= ( m_pModelBox != nullptr );
					ok &= ( m_pModelBoxes != nullptr );
					ok &= ( m_pModelPlane != nullptr );
					ok &= ( m_pModelPlayer != nullptr );
					ok &= ( m_pAnimationPlayer != nullptr );
					if ( !ok )
					{
						return TransitionState_Error;
					}

					m_animationData.create( m_pModelPlayer->getHierarchy()->getJointCount() );
					m_skinningData.matrices.create( framework::getGraphicsSystem(), sizeof( GraphicsMatrix44 ) * 256u );

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

				framework::getResourceManager().unloadResource( m_pAnimationPlayer );
				framework::getResourceManager().unloadResource( m_pModelPlayer );
				framework::getResourceManager().unloadResource( m_pModelPlane );
				framework::getResourceManager().unloadResource( m_pModelBoxes );
				framework::getResourceManager().unloadResource( m_pModelBox );
				framework::getResourceManager().unloadResource( m_pFontBig );
				framework::getResourceManager().unloadResource( m_pFont );
				m_pFont					= nullptr;
				m_pFontBig				= nullptr;
				m_pModelBox				= nullptr;
				m_pModelBoxes			= nullptr;
				m_pModelPlane			= nullptr;
				m_pModelPlayer			= nullptr;
				m_pAnimationPlayer		= nullptr;
				
				m_skinningData.matrices.dispose( framework::getGraphicsSystem() );
				m_animationData.dispose();

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_CreateGameClient:
			if ( isCreating )
			{
				TIKI_VERIFY( m_gameClient.create() );

				m_boxesEntityId		= m_gameClient.createModelEntity( m_pModelBoxes, Vector3::zero );
				m_planeEntityId		= m_gameClient.createPlaneEntity( m_pModelPlane, vector::create( 0.0f, -0.1f, 0.0f ) );

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
				m_pGameRenderer = &m_pParentState->getGameRenderer();

				if ( isCreating )
				{
					FrameData& frameData = m_pGameRenderer->getFrameData();
					frameData.nearPlane		= 0.001f;
					frameData.farPlane		= 100.0f;
					frameData.mainCamera.create( vector::create( 0.0f, 0.0f, 1.0f ), Quaternion::identity );

					m_freeCamera.create( frameData.mainCamera.getPosition(), frameData.mainCamera.getRotation() );
					
					m_testWindow.create( framework::getDebugGui() );
					//m_lightingWindow.create( framework::getDebugGui() );

					m_testWindow.setRectangle( Rectangle( 500.0, 40.0f, 200.0f, 400.0f ) );
					//m_lightingWindow.setRectangle( Rectangle( 1000.0, 100.0f, 250.0f, 100.0f ) );

					return TransitionState_Finish;
				}
				else
				{
					//m_lightingWindow.dispose();
					m_testWindow.dispose();

					m_freeCamera.dispose();

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
		const float timeDelta = float( framework::getFrameTimer().getElapsedTime() );
		const float totalGameTime = float( framework::getFrameTimer().getTotalTime() );

		FrameData& frameData = m_pGameRenderer->getFrameData();

		DirectionalLightData& directionalLight = frameData.directionalLights.push();
		vector::set( directionalLight.direction, 0.941176471f, 0.235294118f, 0.0f );
		directionalLight.color = TIKI_COLOR_WHITE;

		const uint pointLightCount = 4u;
		const Color s_colors[ 4u ] =
		{
			TIKI_COLOR_RED,
			TIKI_COLOR_GREEN,
			TIKI_COLOR_BLUE,
			TIKI_COLOR_YELLOW
		};

		const float timeValue = (float)framework::getFrameTimer().getTotalTime() / 10.0f;
		//for (uint i = 0u; i < pointLightCount; ++i)
		//{
		//	const float value = ( ( f32::twoPi / pointLightCount ) * i ) + ( timeValue * -5.0f );

		//	PointLightData& pointLight = frameData.pointLights.push();
		//	vector::set( pointLight.position, sinf( value ) * 0.5f, 0.0f, cosf( value ) * 0.5f );
		//	pointLight.color = s_colors[ i ];
		//	pointLight.range = 2.0f;
		//}

		//PointLightData& pointLight = frameData.pointLights.push();
		//vector::set( pointLight.position, 0.0f, 0.0f, 0.0f );
		//pointLight.color	= TIKI_COLOR_WHITE;
		//pointLight.range	= 2.0f;

		//SpotLightData& spotLight = frameData.spotLights.push();
		//vector::set( spotLight.position, 1.0f, 1.0f, 0.0f );
		//vector::set( spotLight.direction, 0.5f, 0.5f, 0.0f );
		//spotLight.color = TIKI_COLOR_BLUE;
		//spotLight.range = 5.0f;
		//spotLight.theta = 0.174532925f;
		//spotLight.phi	= 1.22173048f;

		m_pAnimationPlayer->sample( m_animationData.getBegin(), m_animationData.getCount(), *m_pModelPlayer->getHierarchy(), timeValue );

		m_lightingWindow.fillFrameData( frameData );

		//Matrix43 mtx = Matrix43::identity;
		//mtx.pos.y = -0.1f;
		//m_pGameRenderer->queueModel( m_pModelPlane, &mtx );

		//matrix::createIdentity( mtx );
		//matrix::createRotationY( mtx.rot, timeValue );
		//if ( m_drawPlayer )
		//{
		//	const SkinningData* pSkinningData = &m_skinningData;
		//	m_pGameRenderer->queueModel( m_pModelPlayer, &mtx, &pSkinningData );
		//}
		//else
		//{
		//	m_pGameRenderer->queueModel( m_pModelBox, &mtx );
		//}

		TransformComponentState* pState = (TransformComponentState*)m_gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_boxesEntityId, 0u );

		Vector3 position = { 0.0f, 3.0f, 0.0f };
		m_gameClient.getTransformComponent().setPosition( pState, position );

		Quaternion rotation;
		quaternion::fromYawPitchRoll( rotation, timeValue, 0.0f, 0.0f );
		m_gameClient.getTransformComponent().setRotation( pState, rotation );

		GameClientUpdateContext gameClientUpdateContext;
		gameClientUpdateContext.timeDelta		= timeDelta;
		gameClientUpdateContext.totalGameTime	= totalGameTime;		
		m_gameClient.update( gameClientUpdateContext );
		m_gameClient.render( *m_pGameRenderer );

		m_freeCamera.update( frameData.mainCamera, timeDelta );

		const WindowEvent* pEvent = framework::getMainWindow().getEventBuffer().getEventByType( WindowEventType_SizeChanged );
		if ( pEvent != nullptr )
		{
			TIKI_VERIFY( m_bloom.resize(
				framework::getGraphicsSystem(),
				pEvent->data.sizeChanged.size.x / 2u,
				pEvent->data.sizeChanged.size.y / 2u
			) );
		}
	}

	void TestState::render( GraphicsContext& graphicsContext )
	{
		//Matrix44 matrices[ 256u ];
		////AnimationJoint::fillJointArrayFromHierarchy( m_animationData.getData(), m_animationData.getCount(), *m_pModelPlayer->getHierarchy() );
		//AnimationJoint::buildPoseMatrices( matrices, TIKI_COUNT( matrices ), m_animationData.getBegin(), *m_pModelPlayer->getHierarchy() );

		//GraphicsMatrix44* pShaderConstants = static_cast< GraphicsMatrix44* >( graphicsContext.mapBuffer( m_skinningData.matrices ) );
		//for (uint i = 0u; i < m_animationData.getCount(); ++i)
		//{
		//	createGraphicsMatrix44( pShaderConstants[ i ], matrices[ i ] );
		//} 
		//graphicsContext.unmapBuffer( m_skinningData.matrices );

		m_skybox.render( graphicsContext, m_pGameRenderer->getGeometryTarget() );

		if ( m_enableBloom )
		{
			m_bloom.render( graphicsContext, m_pGameRenderer->getAccumulationBuffer(), m_pGameRenderer->getGeometryBufferBxIndex( 2u ) );
		}

		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_BLACK );

		m_immediateRenderer.beginRendering( graphicsContext );
		m_immediateRenderer.beginRenderPass();
			
		if ( m_gbufferIndex != -1 )
		{
			const TextureData& texture = m_pGameRenderer->getGeometryBufferBxIndex( m_gbufferIndex );
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)texture.getWidth(), (float)texture.getHeight() );
			m_immediateRenderer.drawTexturedRectangle( texture, rect );
		}
		else
		{
			const TextureData& texture = m_pGameRenderer->getAccumulationBuffer();
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)texture.getWidth(), (float)texture.getHeight() );
			m_immediateRenderer.drawTexturedRectangle( texture, rect );
		}

		if ( m_enableBloom )
		{
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)m_bloom.getResultData().getWidth() * 2.0f, (float)m_bloom.getResultData().getHeight() * 2.0f );
			m_immediateRenderer.drawTexturedRectangle( m_bloom.getResultData(), rect );
		}

		//const Rectangle rect2 = Rectangle( 50.0f, 50.0f, (float)m_pFont->getTextureData().getWidth(), (float)m_pFont->getTextureData().getHeight() );
		//m_immediateRenderer.drawTexture( &m_pFont->getTextureData(), rect2 );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const float timeDelta = (float)framework::getFrameTimer().getElapsedTime();

		char buffer[ 128u ];
		formatStringBuffer( buffer, TIKI_COUNT( buffer ), " FPS: %.2f", 1.0f / timeDelta );

		m_immediateRenderer.drawText( Vector2::zero, *m_pFont, buffer, TIKI_COLOR_GREEN );
#endif
		
		m_immediateRenderer.endRenderPass();
		m_immediateRenderer.endRendering();

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( m_enablePhysicsDebug )
		{
			m_gameClient.getPhysicsWorld().renderDebug( graphicsContext, m_immediateRenderer, graphicsContext.getBackBuffer(), m_pGameRenderer->getFrameData().mainCamera );
		}
#endif
	}

	bool TestState::processInputEvent( const InputEvent& inputEvent )
	{

		if ( m_freeCamera.processInputEvent( inputEvent ) )
		{
			return true;
		}

		if ( m_gameClient.processInputEvent( inputEvent ) )
		{
			return true;
		}

		if ( inputEvent.eventType == InputEventType_Keyboard_Down )
		{
			switch ( inputEvent.data.keybaordKey.key )
			{
			case KeyboardKey_V:
				{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
					VisualizationMode visualizationMode = (VisualizationMode)( m_pGameRenderer->getVisualizationMode() + 1u );
					if ( visualizationMode == VisualizationMode_Count )
					{
						visualizationMode = VisualizationMode_Invalid;
					}
					m_pGameRenderer->setVisualizationMode( visualizationMode );
#endif
				}
				return true;

			case KeyboardKey_G:
				m_gbufferIndex++;
				if ( m_gbufferIndex > 2 ) m_gbufferIndex = -1;
				return true;

			case KeyboardKey_B:
				m_enableBloom = !m_enableBloom;
				return true;

			case KeyboardKey_Space:
				m_drawPlayer = !m_drawPlayer;
				break;

			case KeyboardKey_F2:
				m_enablePhysicsDebug = !m_enablePhysicsDebug;
				break;

			case KeyboardKey_Z:
				{
					PhysicsBoxShape shape;
					shape.create( vector::create( 0.5f, 0.5f, 0.5f ) );

					bool test = m_gameClient.getPhysicsWorld().checkIntersection( shape, vector::create( 0.0f, 0.25f, 0.0f ) );

					shape.dispose();
				}
				break;

			default:
				break;
			}
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && inputEvent.data.mouseButton.button == MouseButton_Right )
		{
			m_freeCamera.setMouseControl( !m_freeCamera.getMouseControl() );
		}

		return false;
	}
}