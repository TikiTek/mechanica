#include "tiki/gamestates/teststate.hpp"

#include "tiki/animation/animation.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/debugrenderer/debugrenderer.hpp"
#include "tiki/framework/mainwindow.hpp"
#include "tiki/game/game.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/graphicstypes.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/intersection.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/ray.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/physics3d/physics3dboxshape.hpp"
#include "tiki/renderer/renderview.hpp"
#include "tiki/resource/resourcerequestpool.hpp"
#include "tiki/voxelworld/voxeldebugdraw.hpp"

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

				TIKI_VERIFY( m_skybox.create(
					m_pGame->getGraphicsSystem(),
					m_pGame->getResourceManager()
				) );

				VoxelWorldParameters worldParameters;
				worldParameters.worldSize = vector::create( 32.0f, 32.0f, 32.0f );
				worldParameters.maxTreeDepth = 8u;
				
				TIKI_VERIFY( m_world.create( worldParameters ) );

				return TransitionState_Finish;
			}
			else
			{
				m_world.dispose();

				m_skybox.dispose(
					m_pGame->getGraphicsSystem(),
					m_pGame->getResourceManager()
				);

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
					resourceRequestPool.beginLoadResource< Font >( &m_pFont,					"debug.font" );
					resourceRequestPool.beginLoadResource< Font >( &m_pFontBig,					"big.font" );
					resourceRequestPool.beginLoadResource< Model >( &m_pModelBox,				"box.model" );
					resourceRequestPool.beginLoadResource< Model >( &m_pModelBoxes,				"spaceship.model" );
					resourceRequestPool.beginLoadResource< Model >( &m_pModelPlane,				"plane.model" );
					resourceRequestPool.beginLoadResource< Model >( &m_pModelPlayer,			"player.model" );
					resourceRequestPool.beginLoadResource< Animation >( &m_pAnimationPlayer,	"player.run.animation" );
				}

				if ( resourceRequestPool.isFinish() )
				{
					if ( resourceRequestPool.hasError() )
					{
						return TransitionState_Error;
					}

					m_animationData.create( m_pModelPlayer->getHierarchy()->getJointCount() );
					m_skinningData.matrices.create( m_pGame->getGraphicsSystem(), sizeof( GraphicsMatrix44 ) * 256u, "SkinningConstants" );

					return TransitionState_Finish;
				}

				return TransitionState_InProcess;
			}
			else
			{
				TIKI_ASSERT( isInital );

				ResourceManager& resourceManager = m_pGame->getResourceManager();

				resourceManager.unloadResource( m_pAnimationPlayer );
				resourceManager.unloadResource( m_pModelPlayer );
				resourceManager.unloadResource( m_pModelPlane );
				resourceManager.unloadResource( m_pModelBoxes );
				resourceManager.unloadResource( m_pModelBox );
				resourceManager.unloadResource( m_pFontBig );
				resourceManager.unloadResource( m_pFont );
				
				m_skinningData.matrices.dispose( m_pGame->getGraphicsSystem() );
				m_animationData.dispose();

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_CreateGameClient:
			if ( isCreating )
			{
				TIKI_VERIFY( m_gameClient.create() );

				m_boxesEntityId		= m_gameClient.createModelEntity( m_pModelBoxes, Vector3::zero );
				m_planeEntityId		= m_gameClient.createTerrainEntity( m_pModelPlane, vector::create( 0.0f, -0.1f, 0.0f ) );

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
					m_freeCamera.create( vector::create( 0.0f, 0.0f, 1.0f ), Quaternion::identity );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )					
					m_testWindow.create( m_pGame->getDebugGui() );
#endif
					//m_lightingWindow.create( m_pGame->getDebugGui() );

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
		const float timeDelta		= float( m_pGame->getFrameTimer().getElapsedTime() );
		const float totalGameTime	= float( m_pGame->getFrameTimer().getTotalTime() );
		const float timeValue		= (float)m_pGame->getFrameTimer().getTotalTime() / 10.0f;

		Transform3dComponentState* pState = (Transform3dComponentState*)m_gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_boxesEntityId, 0u );

		Vector3 position ={ 0.0f, 3.0f, 0.0f };
		m_gameClient.getTransformComponent().setPosition( pState, position );

		Quaternion rotation;
		quaternion::fromYawPitchRoll( rotation, timeValue, 0.0f, 0.0f );
		m_gameClient.getTransformComponent().setRotation( pState, rotation );

		GameClientUpdateContext gameClientUpdateContext;
		gameClientUpdateContext.timeDelta		= timeDelta;
		gameClientUpdateContext.totalGameTime	= totalGameTime;
		m_gameClient.update( gameClientUpdateContext );

		RenderScene& scene = m_gameClient.getScene();

		DirectionalLightData& directionalLight = scene.addDirectionalLight();
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

		m_lightingWindow.fillFrameData( scene );

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

		m_freeCamera.update( m_gameClient.getView().getCamera(), timeDelta );

		//const WindowEvent* pEvent = m_pGame->getMainWindow().getEventBuffer().getEventByType( WindowEventType_SizeChanged );
		//if ( pEvent != nullptr )
		//{
		//	TIKI_VERIFY( m_bloom.resize(
		//		m_pGame->getGraphicsSystem(),
		//		pEvent->data.sizeChanged.size.x / 2u,
		//		pEvent->data.sizeChanged.size.y / 2u
		//	) );
		//}

		{
			const Camera& camera = m_gameClient.getView().getCamera();

			Plane bottomPlane;
			bottomPlane.create( vector::create( 0.0f, m_world.getWorldSize().y / -2.0f, 0.0f ), Vector3::unitY );

			Ray cameraRay;
			camera.getCameraRay( cameraRay, m_mousePosition );

			Vector3 planePoint;
			if( intersection::intersectRayPlane( cameraRay, bottomPlane, planePoint ) )
			{
				AxisAlignedBox box;
				box.createFromCenterExtends( planePoint, Vector3::one );

				debugrenderer::drawLineRay( cameraRay, 5.0f );

				debugrenderer::drawLineAxisAlignedBox( box, TIKI_COLOR_RED );
				debugrenderer::drawText( vector::create( 50.0f, 100.0f ), TIKI_COLOR_WHITE, "%.4f, %.4f, %.4f", planePoint.x, planePoint.y, planePoint.z );
				debugrenderer::drawText( vector::create( 50.0f, 120.0f ), TIKI_COLOR_WHITE, "%.4f, %.4f, %.4f", cameraRay.origin.x, cameraRay.origin.y, cameraRay.origin.z );
				debugrenderer::drawText( vector::create( 50.0f, 140.0f ), TIKI_COLOR_WHITE, "%.4f, %.4f, %.4f", cameraRay.direction.x, cameraRay.direction.y, cameraRay.direction.z );

				debugrenderer::drawText3D( Vector3::zero, TIKI_COLOR_WHITE, "%.4f, %.4f, %.4f", planePoint.x, planePoint.y, planePoint.z );

				m_mouseWorldPosition = planePoint;
			}
		}

		m_world.update();
		voxel::drawVoxel( m_world.getRootVoxel() );
	}

	void TestState::render( GraphicsContext& graphicsContext )
	{
		m_gameClient.render( *m_pGameRenderer, graphicsContext );
	}

	void TestState::postRender( GraphicsContext& graphicsContext )
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		const ImmediateRenderer& immediateRenderer = m_pGame->getImmediateRenderer();

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

		immediateRenderer.beginRenderPass();

		if ( m_gbufferIndex != -1 )
		{
			const TextureData& texture = m_pGameRenderer->getGeometryBufferBxIndex( m_gbufferIndex );
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)texture.getWidth(), (float)texture.getHeight() );
			immediateRenderer.drawTexturedRectangle( texture, rect );
		}
		else
		{
			const TextureData& texture = m_pGameRenderer->getAccumulationBuffer();
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)texture.getWidth(), (float)texture.getHeight() );
			immediateRenderer.drawTexturedRectangle( texture, rect );
		}

		if ( m_enableBloom )
		{
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)m_bloom.getResultData().getWidth() * 2.0f, (float)m_bloom.getResultData().getHeight() * 2.0f );
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
			case KeyboardKey_Space:
				{
					VoxelWorldTranformCommand command;
					command.type = VoxelWorldTransformTypes_FillSphere;
					command.data.sphere.center = m_mouseWorldPosition;
					command.data.sphere.radius = 2.0f;

					m_world.queueCommand( command );
				}
				return true;

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

			case KeyboardKey_P:
				m_drawPlayer = !m_drawPlayer;
				break;

			case KeyboardKey_F2:
				m_enablePhysicsDebug = !m_enablePhysicsDebug;
				break;

			case KeyboardKey_Z:
				{
					Physics3dBoxShape shape;
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

	const tiki::RenderView& TestState::getRenderView() const
	{
		return m_gameClient.getView();
	}
}