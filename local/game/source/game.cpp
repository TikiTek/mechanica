#include "tiki/game/game.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/debug_renderer/debug_renderer.hpp"
#include "tiki/game_states/application_state.hpp"
#include "tiki/game_states/basic_test_state.hpp"
#include "tiki/game_states/credits_state.hpp"
#include "tiki/game_states/intro_state.hpp"
#include "tiki/game_states/menu_state.hpp"
#include "tiki/game_states/physics_test_state.hpp"
#include "tiki/game_states/play_state.hpp"
#include "tiki/game_states/test_state.hpp"

namespace tiki
{
	struct States
	{
		ApplicationState	applicationState;
		IntroState			introState;
		MenuState			menuState;
		PlayState			playState;
		CreditsState		creditsState;
		TestState			testState;
		BasicTestState		basicTestState;
		PhysicsTestState	physicsTestState;
	};

	static GameStates getStartState()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		char userName[ 32u ];
		platform::getUserName( userName, TIKI_COUNT( userName ) );

		if ( isStringEquals( userName, "Tim") ||
			 isStringEquals( userName, "tim.boden" ) ||
			 isStringEquals( userName, "mail" ) )
		{
			//return GameStates_PhysicsTest;
			//return GameStates_Test;
			//return GameStates_BasicTest;
			//return GameStates_Play;
			return GameStates_Menu;
		}
#endif

		return GameStates_Play;
	}

	void Game::fillGameParameters( GameApplicationParamters& parameters )
	{
		parameters.screenWidth		= 1280u;
		parameters.screenHeight		= 720u;
		parameters.backBufferFormat	= PixelFormat_R10G10B10A2;

#if TIKI_ENABLED( TIKI_BUILD_MASTER )
		parameters.pGamebuildPath = "./gamebuild/";
#elif TIKI_ENABLED( TIKI_BUILD_DEBUG )
		if ( !debug::isDebuggerActive() )
		{
			parameters.assetBuildPath = Path( "../../../asset_build/" );
		}
#endif
	}

	bool Game::initializeGame()
	{
		GraphicsSystem& graphicsSystem		= getGraphicsSystem();
		ResourceManager& resourceManager	= getResourceManager();

		EntityTemplateGenericDataResource::registerResourceType( resourceManager );

		m_resourceRequestPool.create( resourceManager );

		m_pStates = TIKI_NEW_ZERO( States );
		m_pStates->applicationState.create( this );
		m_pStates->introState.create( &m_pStates->applicationState );
		m_pStates->menuState.create( this, &m_pStates->applicationState );
		m_pStates->playState.create( this, &m_pStates->applicationState );
		m_pStates->testState.create( this, &m_pStates->applicationState );
		m_pStates->basicTestState.create( this );
		m_pStates->physicsTestState.create( this );

		GameStateDefinition gameDefinition[] =
		{
			{ nullptr,								0u,	0u,										"Root" },
				{ &m_pStates->applicationState,		0u,	ApplicationStateTransitionSteps_Count,	"ApplicationState" },
					{ &m_pStates->introState,		1u,	IntroStateTransitionSteps_Count,		"IntroState" },			// F6
					{ &m_pStates->menuState,		1u,	MenuStateTransitionSteps_Count,			"MenuState" },			// F7
					{ &m_pStates->playState,		1u,	PlayStateTransitionSteps_Count,			"PlayState" },			// F8
					{ &m_pStates->creditsState,		1u,	CreditsStateTransitionSteps_Count,		"CreditsState" },		// F9
					{ &m_pStates->testState,		1u,	TestStateTransitionSteps_Count,			"TestState" },			// F10
				{ &m_pStates->basicTestState,		0u,	BasicTestStateTransitionSteps_Count,	"BasicTestState" },		// F11
				{ &m_pStates->physicsTestState,		0u,	PhysicsTestStateTransitionSteps_Count,	"PhysicsTestState" }	// F12
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( gameDefinition ) == GameStates_Count );

		m_gameFlow.create( gameDefinition, TIKI_COUNT( gameDefinition ) );
		m_gameFlow.startTransition( getStartState() );

		if ( !m_touchSystem.create( graphicsSystem, resourceManager ) )
		{
			return false;
		}

		return true;
	}

	void Game::shutdownGame()
	{
		GraphicsSystem& graphicsSystem		= getGraphicsSystem();
		ResourceManager& resourceManager	= getResourceManager();

		m_touchSystem.dispose( graphicsSystem, resourceManager );

		if ( m_gameFlow.isCreated() )
		{
			while ( m_gameFlow.isInTransition() )
			{
				m_gameFlow.update();
			}

			m_gameFlow.startTransition( GameStates_Root );

			while ( m_gameFlow.isInTransition() )
			{
				m_gameFlow.update();
				Sleep( 50 );
			}

			m_gameFlow.dispose();
		}

		if ( m_pStates != nullptr )
		{
			m_pStates->applicationState.dispose();
			m_pStates->introState.dispose();
			m_pStates->menuState.dispose();
			m_pStates->playState.dispose();
			m_pStates->creditsState.dispose();
			m_pStates->testState.dispose();
			m_pStates->basicTestState.dispose();
			m_pStates->physicsTestState.dispose();

			TIKI_DELETE( m_pStates );
		}

		EntityTemplateGenericDataResource::unregisterResourceType( resourceManager );

		m_resourceRequestPool.dispose();
	}

	void Game::updateGame( bool wantToShutdown )
	{
		if ( wantToShutdown && !m_gameFlow.isInTransition() )
		{
			m_gameFlow.startTransition( 0u );
		}

		m_touchSystem.update( (float)getFrameTimer().getElapsedTime() );
		for (uint i = 0u; i < m_touchSystem.getInputEventCount(); ++i)
		{
			processGameInputEvent( m_touchSystem.getInputEventByIndex( i ) );
		}

		m_resourceRequestPool.update();
		m_gameFlow.update();

		if ( !m_gameFlow.isInTransition() && m_gameFlow.getCurrentState() == 0u )
		{
			m_running = false;
		}
	}

	void Game::renderGame( GraphicsContext& graphicsContext ) const
	{
		m_gameFlow.render( graphicsContext );

		m_touchSystem.render( getImmediateRenderer() );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if( m_gameFlow.isInState( GameStates_PhysicsTest ) )
		{
			debugrenderer::flush( getImmediateRenderer(), m_pStates->physicsTestState.getCamera() );
		}
		else if( m_gameFlow.isInState( GameStates_Application ) )
		{
			debugrenderer::flush( getImmediateRenderer(), m_pStates->applicationState.getRenderer().getCamera() );
		}
#endif
	}

	bool Game::processGameInputEvent( const InputEvent& inputEvent )
	{
		if ( m_touchSystem.processInputEvent( inputEvent ) )
		{
			return true;
		}

		//if ( !m_gameFlow.isInTransition() )
		//{
		//	for (int i = 0; i < (int)m_gameFlow.getStateCount() - 2; ++i)
		//	{
		//		if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == (KeyboardKey)( KeyboardKey_F6 + i ) )
		//		{
		//			m_gameFlow.startTransition( i + 2 );
		//			return true;
		//		}
		//	}
		//}

		return m_gameFlow.processInputEvent( inputEvent );
	}

	void Game::processGameWindowEvent( const WindowEvent& windowEvent )
	{
		m_gameFlow.processWindowEvent( windowEvent );
	}

	GameApplication& framework::getGame()
	{
		static Game game;
		return game;
	}
}