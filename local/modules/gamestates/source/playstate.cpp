
#include "tiki/gamestates/playstate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/renderer/gamerenderer.hpp"
#include "tiki/renderer/renderercontext.hpp"

namespace tiki
{
	PlayState::PlayState()
	{
		m_pParentState	= nullptr;
		m_pGameRenderer	= nullptr;
	}

	PlayState::~PlayState()
	{
		TIKI_ASSERT( m_pParentState		== nullptr );
		TIKI_ASSERT( m_pGameRenderer	== nullptr );
	}

	void PlayState::create( ApplicationState* pParentState )
	{
		m_pParentState = pParentState;
	}

	void PlayState::dispose()
	{
		m_pParentState = nullptr;
	}

	TransitionState PlayState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case PlayStateTransitionSteps_InitializeGraphics:
			TIKI_ASSERT( isInital );

			if ( isCreating )
			{
				PostProcessAsciiParameters asciiParameters;	
				if ( !m_ascii.create( framework::getGraphicsSystem(), framework::getResourceManager(), asciiParameters ) )
				{
					return TransitionState_Error;
				}

				PostProcessBloomParameters bloomParameters;	
				bloomParameters.width		= framework::getGraphicsSystem().getBackBuffer().getWidth() / 2u;
				bloomParameters.height		= framework::getGraphicsSystem().getBackBuffer().getHeight() / 2u;
				bloomParameters.passCount	= 6u;
				if ( !m_bloom.create( framework::getGraphicsSystem(), framework::getResourceManager(), bloomParameters ) )
				{
					return TransitionState_Error;
				}

				if ( !m_immediateRenderer.create( framework::getGraphicsSystem(), framework::getResourceManager() ) )
				{
					return TransitionState_Error;
				}

				return TransitionState_Finish;
			}
			else
			{
				m_bloom.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );
				m_ascii.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

				m_immediateRenderer.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

				return TransitionState_Finish;
			}
			//break;

		case PlayStateTransitionSteps_CreateClientSystems:
			TIKI_ASSERT( isInital );

			if ( isCreating )
			{
				if( !m_gameClient.create() )
				{
					return TransitionState_Error;
				}

				if( !m_gameState.create( m_gameClient, framework::getResourceManager() ) )
				{
					return TransitionState_Error;
				}

				return TransitionState_Finish;
			}
			else
			{
				m_gameState.dispose( framework::getResourceManager() );
				m_gameClient.dispose();

				return TransitionState_Finish;
			}
			//break;
			
		case PlayStateTransitionSteps_SetRendererData:
			TIKI_ASSERT( isInital );

			if ( isCreating )
			{
				m_pGameRenderer = &m_pParentState->getGameRenderer();

				FrameData& frameData = m_pGameRenderer->getFrameData();
				frameData.nearPlane		= 0.001f;
				frameData.farPlane		= 100.0f;
				frameData.mainCamera.create( vector::create( 0.0f, 0.0f, 1.0f ), Quaternion::identity );

				m_playerCamera.create(
					m_gameState.getPlayerEntityId(),
					(const PlayerControlComponentState*)m_gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_gameState.getPlayerEntityId(), m_gameClient.getPlayerControlComponent().getTypeId() ),
					m_gameClient.getPlayerControlComponent()
				);

				return TransitionState_Finish;
			}
			else
			{
				m_playerCamera.dispose();

				m_pGameRenderer = nullptr;

				return TransitionState_Finish;
			}
			//break;

		default:
			break;
		}

		return TransitionState_Error;
	}

	void PlayState::update()
	{
		const float timeDelta = float( framework::getFrameTimer().getElapsedTime() );
		const float totalGameTime = float( framework::getFrameTimer().getTotalTime() );

		FrameData& frameData = m_pGameRenderer->getFrameData();

		DirectionalLightData& directionalLight = frameData.directionalLights.push();
		vector::set( directionalLight.direction, 0.941176471f, 0.235294118f, 0.0f );
		directionalLight.color = TIKI_COLOR_WHITE;

		const PhysicsCharacterControllerComponentState* pPlayerControllerState = (const PhysicsCharacterControllerComponentState*)m_gameClient.getEntitySystem().getFirstComponentOfEntityAndType( m_gameState.getPlayerEntityId(), m_gameClient.getPhysicsCharacterControllerComponent().getTypeId() );

		GameClientUpdateContext gameClientUpdateContext;
		gameClientUpdateContext.timeDelta		= timeDelta;
		gameClientUpdateContext.totalGameTime	= totalGameTime;		
		gameClientUpdateContext.pPlayerCollider	= &m_gameClient.getPhysicsCharacterControllerComponent().getPhysicsObject( pPlayerControllerState );
		m_gameClient.update( gameClientUpdateContext );

		m_gameState.processCollectedCoins( gameClientUpdateContext.collectedCoins );
		m_gameState.update( frameData, timeDelta, totalGameTime );

		m_gameClient.render( *m_pGameRenderer );

		m_playerCamera.update( frameData.mainCamera );
	}

	void PlayState::render( GraphicsContext& graphicsContext )
	{
		m_ascii.render( graphicsContext, m_pGameRenderer->getFrameData(), m_pGameRenderer->getRendererContext() );
		m_bloom.render( graphicsContext, m_pGameRenderer->getAccumulationBuffer(), m_pGameRenderer->getGeometryBufferBxIndex( 2u ) );

		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_BLACK );

		m_immediateRenderer.beginRendering( graphicsContext );
		m_immediateRenderer.beginRenderPass();

		// ascii
		{
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)m_ascii.getResultData().getWidth(), (float)m_ascii.getResultData().getHeight() );
			m_immediateRenderer.drawTexturedRectangle( m_ascii.getResultData(), rect );
		}

		// bloom
		{
			const Rectangle rect = Rectangle( 0.0f, 0.0f, (float)m_bloom.getResultData().getWidth() * 2.0f, (float)m_bloom.getResultData().getHeight() * 2.0f );
			m_immediateRenderer.drawTexturedRectangle( m_bloom.getResultData(), rect );
		}

		m_immediateRenderer.endRenderPass();
		m_immediateRenderer.endRendering();
	}

	bool PlayState::processInputEvent( const InputEvent& inputEvent )
	{
		if ( m_gameClient.processInputEvent( inputEvent ) )
		{
			return true;
		}

		return false;
	}
}