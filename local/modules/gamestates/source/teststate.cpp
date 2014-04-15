
#include "tiki/gamestates/teststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/graphics/graphicstypes.hpp"
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
				PostProcessAsciiParameters asciiParameters;	

				m_ascii.create(
					framework::getGraphicsSystem(),
					framework::getResourceManager(),
					asciiParameters
				);

				return TransitionState_Finish;
			}
			else
			{
				m_ascii.dispose(
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
					m_pModel		= framework::getResourceManager().loadResource< Model >( "test.model" );
					m_pTexture		= framework::getResourceManager().loadResource< Texture >( "checker.texture" );
					m_pTexture2		= framework::getResourceManager().loadResource< Texture >( "ascii.texture" );
					m_pFont			= framework::getResourceManager().loadResource< Font >( "debug.font" );
					TIKI_ASSERT( m_pModel != nullptr );
					TIKI_ASSERT( m_pTexture != nullptr );
					TIKI_ASSERT( m_pTexture2 != nullptr );
					TIKI_ASSERT( m_pFont != nullptr );

					Vector2 screenSize;
					screenSize.x = (float)framework::getGraphicsSystem().getBackBuffer().getWidth();
					screenSize.y = (float)framework::getGraphicsSystem().getBackBuffer().getHeight();

					Projection projection;
					projection.createOrthographicCenter( screenSize.x, -screenSize.y, 0.0f, 1.0f );

					m_immediateRenderer.create( framework::getGraphicsSystem(), framework::getResourceManager() );
					m_immediateRenderer.setProjection( projection );

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
				framework::getResourceManager().unloadResource< Texture >( m_pTexture2 );
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

					vector::clear( m_leftStickState );
					vector::clear( m_rightStickState );

					m_debugMenu.create( framework::getGraphicsSystem(), framework::getResourceManager(), 16u );
					m_debugMenuPageDebugProp.create( *(DebugPropManager*)nullptr );
					m_debugMenu.addPage( m_debugMenuPageDebugProp );
					m_debugMenu.addPage( m_debugMenuPageDebugProp );
					m_debugMenu.addPage( m_debugMenuPageDebugProp );
					m_debugMenu.addPage( m_debugMenuPageDebugProp );
					m_debugMenu.addPage( m_debugMenuPageDebugProp );

					return TransitionState_Finish;
				}
				else
				{
					m_debugMenu.removePage( m_debugMenuPageDebugProp );
					m_debugMenu.removePage( m_debugMenuPageDebugProp );
					m_debugMenu.removePage( m_debugMenuPageDebugProp );
					m_debugMenu.removePage( m_debugMenuPageDebugProp );
					m_debugMenu.removePage( m_debugMenuPageDebugProp );
					m_debugMenuPageDebugProp.dispose();
					m_debugMenu.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

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
		const float timeDelta = (float)framework::getFrameTimer().getElapsedTime();

		FrameData& frameData = m_pGameRenderer->getFrameData();

		Vector3 cameraPosition		= frameData.mainCamera.getPosition();
		Quaternion cameraRotation	= frameData.mainCamera.getRotation();

		// move camera
		{
			Vector3 cameraForward;
			Vector3 cameraRight;
			quaternion::getForward( cameraForward, cameraRotation );
			quaternion::getRight( cameraRight, cameraRotation );

			vector::scale( cameraForward,	m_leftStickState.y * timeDelta );
			vector::scale( cameraRight,		m_leftStickState.x * timeDelta );

			vector::add( cameraPosition, cameraForward );
			vector::add( cameraPosition, cameraRight );
		}

		// rotate camera
		{
			Vector2 rotation = m_rightStickState;
			vector::scale( rotation, timeDelta * 2.0f );

			Quaternion cameraRotationDelta;
			quaternion::fromYawPitchRoll( cameraRotationDelta, rotation.x, rotation.y, 0.0f );

			quaternion::mul( cameraRotation, cameraRotationDelta );
		}

		frameData.mainCamera.setTransform( cameraPosition, cameraRotation );

		Matrix43 mtx;
		matrix::clear( mtx );
		matrix::createRotationY( mtx.rot, (float)framework::getFrameTimer().getTotalTime() / 10.0f );

		m_pGameRenderer->queueModel( m_pModel, &mtx );		

		m_debugMenu.update();
	}

	void TestState::render( GraphicsContext& graphicsContext )
	{
		const TextureData& texture = m_pGameRenderer->getGeometryBufferBxIndex( 0u );
		m_ascii.render(
			graphicsContext,
			m_pGameRenderer->getFrameData(),
			m_pGameRenderer->getRendererContext()
		);

		const float timeDelta = (float)framework::getFrameTimer().getElapsedTime();
		const string frameRate = formatString( " FPS: %.2f", 1.0f / timeDelta );

		graphicsContext.beginRenderPass( graphicsContext.getBackBuffer() );
		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_BLACK );

		//const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)texture.getWidth(), (float)texture.getHeight() );
		//m_immediateRenderer.drawTexture( &texture, rect );

		const Rectangle rect2 = Rectangle( 0.0f, 0.0f, (float)m_ascii.getResultData().getWidth(), (float)m_ascii.getResultData().getHeight() );
		m_immediateRenderer.drawTexture( &m_ascii.getResultData(), rect2, TIKI_COLOR_WHITE );
		//const Rectangle rect2 = Rectangle( 50.0f, 50.0f, (float)m_pFont->getTextureData().getWidth(), (float)m_pFont->getTextureData().getHeight() );
		//m_immediateRenderer.drawTexture( &m_pFont->getTextureData(), rect2, TIKI_COLOR_WHITE );

		m_immediateRenderer.drawText( Vector2::zero, *m_pFont, frameRate.cStr(), TIKI_COLOR_GREEN );
		
		m_immediateRenderer.flush( graphicsContext );

		m_debugMenu.render( graphicsContext );
		
		graphicsContext.endRenderPass();
	}

	bool TestState::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.eventType == InputEventType_Controller_StickChanged )
		{
			switch ( inputEvent.data.controllerStick.stickIndex )
			{
			case 0u:
				m_leftStickState.x	= inputEvent.data.controllerStick.xState;
				m_leftStickState.y	= inputEvent.data.controllerStick.yState;
				break;

			case 1u:
				m_rightStickState.x	= inputEvent.data.controllerStick.xState;
				m_rightStickState.y	= inputEvent.data.controllerStick.yState;
				break;
			}
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == KeyboardKey_F1 )
		{
			m_debugMenu.setActive( !m_debugMenu.getActive() );
			return true;
		}

		return false;
	}
}