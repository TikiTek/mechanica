
#include "tiki/gamestates/playstate.hpp"

#include "tiki/base/debugprop.hpp"
#include "tiki/base/timer.hpp"
#include "tiki/game/game.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/runtimeshared/windowevent.hpp"

namespace tiki
{
	PlayState::PlayState()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;
		m_pRenderer		= nullptr;
	}

	PlayState::~PlayState()
	{
		TIKI_ASSERT( m_pGame			== nullptr );
		TIKI_ASSERT( m_pParentState		== nullptr );
		TIKI_ASSERT( m_pRenderer		== nullptr );
	}

	void PlayState::create( Game* pGame, ApplicationState* pParentState )
	{
		m_pGame			= pGame;
		m_pParentState	= pParentState;
	}

	void PlayState::dispose()
	{
		m_pGame			= nullptr;
		m_pParentState	= nullptr;
	}

	TransitionState PlayState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		//switch ( currentStep )
		//{
		//case PlayStateTransitionSteps_InitializeGraphics:
		//	TIKI_ASSERT( isInital );

		//	ResourceRequestPool& resourceRequestPool = m_pGame->getResourceRequestPool();
		//	if ( isCreating )
		//	{
		//		PostProcessBloomParameters bloomParameters;	
		//		bloomParameters.width		= m_pGame->getGraphicsSystem().getBackBuffer().getWidth() / 2u;
		//		bloomParameters.height		= m_pGame->getGraphicsSystem().getBackBuffer().getHeight() / 2u;
		//		bloomParameters.passCount	= 6u;
		//		if ( !m_bloom.create( m_pGame->getGraphicsSystem(), resourceRequestPool, bloomParameters ) )
		//		{
		//			return TransitionState_Error;
		//		}

		//		if ( !m_immediateRenderer.create( m_pGame->getGraphicsSystem(), m_pGame->getResourceManager() ) )
		//		{
		//			return TransitionState_Error;
		//		}

		//		return TransitionState_Finish;
		//	}
		//	else
		//	{
		//		m_bloom.dispose( m_pGame->getGraphicsSystem(), m_pGame->getResourceManager() );

		//		m_immediateRenderer.dispose( m_pGame->getGraphicsSystem(), m_pGame->getResourceManager() );

		//		return TransitionState_Finish;
		//	}
		//	//break;

		//case PlayStateTransitionSteps_CreateClientSystems:
		//	TIKI_ASSERT( isInital );

		//	if ( isCreating )
		//	{
		//		if( !m_gameSession.create( m_pGame->getGraphicsSystem(), m_pGame->getResourceManager() ) )
		//		{
		//			return TransitionState_Error;
		//		}

		//		return TransitionState_Finish;
		//	}
		//	else
		//	{
		//		m_gameSession.dispose( m_pGame->getResourceManager() );

		//		return TransitionState_Finish;
		//	}
		//	//break;
		//	
		//case PlayStateTransitionSteps_SetRendererData:
		//	TIKI_ASSERT( isInital );

		//	if ( isCreating )
		//	{
		//		m_pRenderer = &m_pParentState->getRenderer();
		//		return TransitionState_Finish;
		//	}
		//	else
		//	{
		//		m_pRenderer = nullptr;

		//		return TransitionState_Finish;
		//	}
		//	//break;

		//default:
		//	break;
		//}

		return TransitionState_Error;
	}

	void PlayState::update()
	{
		const float timeDelta = float( m_pGame->getFrameTimer().getElapsedTime() );
		const float totalGameTime = float( m_pGame->getFrameTimer().getTotalTime() );

		m_gameSession.update( timeDelta, totalGameTime );
	}

	void PlayState::render( GraphicsContext& graphicsContext )
	{
		//m_gameSession.render( graphicsContext );

		//m_bloom.render( graphicsContext, m_pRenderer->getOffscreenColorData(), nullptr );

		//graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_BLACK );

		//m_immediateRenderer.beginRendering( graphicsContext );
		//m_immediateRenderer.beginRenderPass();

		//// renderer output
		//{
		//	const TextureData& texture = m_pGameRenderer->getAccumulationBuffer();
		//	const AxisAlignedRectangle rect = createAxisAlignedRectangle( 0.0f, 0.0f, (float)texture.getWidth(), (float)texture.getHeight() );
		//	m_immediateRenderer.drawTexturedRectangle( texture, rect );
		//}

		//// bloom
		//{
		//	const AxisAlignedRectangle rect = createAxisAlignedRectangle( 0.0f, 0.0f, (float)m_bloom.getResultData().getWidth() * 2.0f, (float)m_bloom.getResultData().getHeight() * 2.0f );
		//	m_immediateRenderer.drawTexturedRectangle( m_bloom.getResultData(), rect );
		//}

		//m_immediateRenderer.endRenderPass();
		//m_immediateRenderer.endRendering();
	}

	bool PlayState::processInputEvent( const InputEvent& inputEvent )
	{
		//if ( m_gameSession.processInputEvent( inputEvent ) )
		//{
		//	return true;
		//}

		return false;
	}

	void PlayState::processWindowEvent( const WindowEvent& windowEvent )
	{
		//if ( windowEvent.type == WindowEventType_SizeChanged )
		//{
		//	const bool ok = m_bloom.resize(
		//		m_pGame->getGraphicsSystem(),
		//		windowEvent.data.sizeChanged.size.x / 2u,
		//		windowEvent.data.sizeChanged.size.y / 2u
		//	);

		//	if ( !ok )
		//	{
		//		m_bloom.dispose( m_pGame->getGraphicsSystem(), m_pGame->getResourceManager() );
		//	}
		//}
	}

	const Camera& PlayState::getCamera() const
	{
		return m_gameSession.getGameClient().getCamera();
	}
}