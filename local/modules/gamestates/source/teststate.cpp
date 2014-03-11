
#include "tiki/gamestates/teststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/components/staticmodelcomponent_initdata.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/graphicsbase/graphicstypes.hpp"
#include "tiki/input/inputsystem.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/quaternion.hpp"
#include "tiki/math/rectangle.hpp"
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
				m_typeRegister.create( 32u );
				m_staticModelComponentTypeId =  m_typeRegister.registerType( &m_staticModelComponent );

				m_storage.create( 2048u, 128u, &m_typeRegister );

				return TransitionState_Finish;
			}
			else
			{
				m_storage.dispose();
				m_typeRegister.dispose();

				return TransitionState_Finish;
			}
			//break;

		case TestStateTransitionSteps_LoadResources:
			if ( isCreating )
			{
				if ( isInital )
				{
					m_pModel		= framework::getResourceManager().loadResource< Model >( "test.model" );
					m_pTexture		= framework::getResourceManager().loadResource< Texture >( "checker.texture" );
					m_pFont			= framework::getResourceManager().loadResource< Font >( "debug.font" );
					TIKI_ASSERT( m_pModel != nullptr );
					TIKI_ASSERT( m_pTexture != nullptr );
					TIKI_ASSERT( m_pFont != nullptr );

					m_immediateRenderer.create( framework::getGraphicsSystem(), framework::getResourceManager() );

					{
						ComponentState* pState = m_storage.allocateState( m_staticModelComponentTypeId );

						StaticModelComponentInitData initData;
						initData.pModel = m_pModel;
						m_staticModelComponent.initializeState( pState, &initData );
					}

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

				framework::getResourceManager().unloadResource< Texture >( m_pTexture );
				framework::getResourceManager().unloadResource< Model >( m_pModel );
				framework::getResourceManager().unloadResource< Font >( m_pFont );

				m_immediateRenderer.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

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

					RendererContext& rendererContext = m_pGameRenderer->getRendererContext();

					const Vector3 cameraPosition = { 0.0f, 0.0f, 1.0f };
					frameData.mainCamera.create( cameraPosition, Quaternion::identity );

					m_fallbackRenderEffect.create( m_pGameRenderer->getRendererContext(), framework::getGraphicsSystem(), framework::getResourceManager() );

					m_pGameRenderer->registerRenderEffect( &m_fallbackRenderEffect );

					return TransitionState_Finish;
				}
				else
				{
					m_pGameRenderer->unregisterRenderEffect( &m_fallbackRenderEffect );
					m_fallbackRenderEffect.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

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
		FrameData& frameData = m_pGameRenderer->getFrameData();
		InputSystem& inputSystem = framework::getInputSystem();

		Vector3 cameraPosition		= frameData.mainCamera.getPosition();
		Quaternion cameraRotation	= frameData.mainCamera.getRotation();

		// move camera
		{
			Vector3 cameraForward;
			Vector3 cameraRight;
			quaternion::getForward( cameraForward, cameraRotation );
			quaternion::getRight( cameraRight, cameraRotation );

			Vector3 cameraBackward	= cameraForward;
			Vector3 cameraLeft		= cameraRight;
			vector::negate( cameraBackward );
			vector::negate( cameraLeft );

			const float timeDelta = (float)framework::getFrameTimer().getElapsedTime();
			//vector::scale( cameraForward,	( inputSystem.isKeyDown( KEY_W ) ? 1.0f : 0.0f ) * timeDelta );
			//vector::scale( cameraBackward,	( inputSystem.isKeyDown( KEY_S ) ? 1.0f : 0.0f ) * timeDelta );
			//vector::scale( cameraLeft,		( inputSystem.isKeyDown( KEY_A ) ? 1.0f : 0.0f ) * timeDelta );
			//vector::scale( cameraRight,		( inputSystem.isKeyDown( KEY_D ) ? 1.0f : 0.0f ) * timeDelta );

			//vector::add( cameraPosition, cameraForward );
			//vector::add( cameraPosition, cameraBackward );
			//vector::add( cameraPosition, cameraLeft );
			//vector::add( cameraPosition, cameraRight );
		}

		// rotate camera
		{
			//const Vector2& mouseDelta = inputSystem.getMouseDeltaNormalized();

			//Quaternion cameraRotationDelta;
			//quaternion::fromYawPitchRoll( cameraRotationDelta, mouseDelta.x, -mouseDelta.y, 0.0f );

			//quaternion::mul( cameraRotation, cameraRotationDelta );
		}

		frameData.mainCamera.setTransform( cameraPosition, cameraRotation );

		//Matrix43 mtx;
		//matrix::clear( mtx );
		//matrix::createRotationY( mtx.rot, (float)framework::getFrameTimer().getTotalTime() / 10.0f );


		m_staticModelComponent.render( *m_pGameRenderer );
	}

	void TestState::render( GraphicsContext& graphicsContext )
	{
		const float timeDelta = (float)framework::getFrameTimer().getElapsedTime();
		const string frameRate = formatString( " FPS: %.2f", 1.0f / timeDelta );

		graphicsContext.beginRenderPass( graphicsContext.getBackBuffer() );
		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_BLACK );

		const TextureData& texture = m_pGameRenderer->getGeometryBufferBxIndex( 0u );

		const Rectangle rect = Rectangle( 0.0f, 0.0f, 1.0f, 1.0f );
		m_immediateRenderer.drawTexture( &texture, rect );

		const Rectangle rect2 = Rectangle( 0.25f, 0.25f, 0.1f, 0.1f );
		m_immediateRenderer.drawTexture( nullptr, rect2, TIKI_COLOR( 0u, 50, 200, 128 ) );

		m_immediateRenderer.drawText( Vector2::zero, *m_pFont, frameRate.cStr(), TIKI_COLOR_GREEN );
		
		m_immediateRenderer.flush( graphicsContext );
		
		graphicsContext.endRenderPass();
	}
}