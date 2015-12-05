
#include "tiki/game/game.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/gamestates/basicteststate.hpp"
#include "tiki/gamestates/creditsstate.hpp"
#include "tiki/gamestates/introstate.hpp"
#include "tiki/gamestates/menustate.hpp"
#include "tiki/gamestates/playstate.hpp"
#include "tiki/gamestates/teststate.hpp"

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
			return GameStates_Test;
			//return GameStates_BasicTest;
			//return GameStates_Play;
		}
#endif

		return GameStates_Play;
	}

	void Game::fillParameters( GameApplicationParamters& parameters )
	{
		parameters.screenWidth	= 1280;
		parameters.screenHeight	= 720;

		parameters.graphicsMode = GraphicsRendererMode_Hardware;

#if TIKI_ENABLED( TIKI_BUILD_MASTER )
		parameters.pGamebuildPath = "./gamebuild/";
#elif TIKI_ENABLED( TIKI_BUILD_DEBUG )
		if ( !debug::isDebuggerActive() )
		{
			parameters.pGamebuildPath = "../../../gamebuild/";
		}
#endif
	}

	bool Game::initializeGame()
	{
		GraphicsSystem& graphicsSystem		= framework::getGraphicsSystem();
		ResourceManager& resourceManager	= framework::getResourceManager();

		m_factories.create( resourceManager, graphicsSystem );

		m_resourceRequestPool.create( resourceManager );
		m_immediateRenderer.create( graphicsSystem, resourceManager );

		m_pStates = TIKI_MEMORY_NEW_OBJECT( States );
		m_pStates->applicationState.create();
		m_pStates->introState.create( &m_pStates->applicationState );
		m_pStates->playState.create( &m_pStates->applicationState );
		m_pStates->testState.create( &m_pStates->applicationState );
		m_pStates->basicTestState.create();

		GameStateDefinition gameDefinition[] =
		{
			{ nullptr,								0u,	0u,										"Root" },
				{ &m_pStates->applicationState,		0u,	ApplicationStateTransitionSteps_Count,	"ApplicationState" },	// F5
					{ &m_pStates->introState,		1u,	IntroStateTransitionSteps_Count,		"IntroState" },			// F6
					{ &m_pStates->menuState,		1u,	MenuStateTransitionSteps_Count,			"MenuState" },			// F7
					{ &m_pStates->playState,		1u,	PlayStateTransitionSteps_Count,			"PlayState" },			// F8
					{ &m_pStates->creditsState,		1u,	CreditsStateTransitionSteps_Count,		"CreditsState" },		// F9
					{ &m_pStates->testState,		1u,	TestStateTransitionSteps_Count,			"TestState" },			// F10
				{ &m_pStates->basicTestState,		0u,	BasicTestStateTransitionSteps_Count,	"BasicTestState" }		// F11
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
		GraphicsSystem& graphicsSystem		= framework::getGraphicsSystem();
		ResourceManager& resourceManager	= framework::getResourceManager();

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

			TIKI_MEMORY_DELETE_OBJECT( m_pStates );
		}

		m_immediateRenderer.dispose( graphicsSystem, resourceManager );
		m_resourceRequestPool.dispose();
	}

	void Game::update( bool wantToShutdown )
	{
		if ( wantToShutdown )
		{
			m_gameFlow.startTransition( 0u );
		}

		m_touchSystem.update( (float)framework::getFrameTimer().getElapsedTime(), framework::getGraphicsSystem() );
		for (uint i = 0u; i < m_touchSystem.getInputEventCount(); ++i)
		{
			processInputEvent( m_touchSystem.getInputEventByIndex( i ) );
		} 

		m_resourceRequestPool.update();
		m_gameFlow.update();

		if ( !m_gameFlow.isInTransition() && m_gameFlow.getCurrentState() == 0u )
		{
			m_running = false;
		}
	}

	void Game::render( GraphicsContext& graphicsContext ) const
	{
		m_gameFlow.render( graphicsContext );

		m_touchSystem.render( graphicsContext );
	}

	bool Game::processInputEvent( const InputEvent& inputEvent )
	{
		if ( m_touchSystem.processInputEvent( inputEvent ) )
		{
			return true;
		}

		if ( !m_gameFlow.isInTransition() )
		{
			for (int i = 0; i < (int)m_gameFlow.getStateCount() - 1; ++i)
			{
				if ( inputEvent.eventType == InputEventType_Keyboard_Down && inputEvent.data.keybaordKey.key == (KeyboardKey)( KeyboardKey_F5 + i ) )
				{
					m_gameFlow.startTransition( i + 1 );
					return true;
				}
			}
		}

		return m_gameFlow.processInputEvent( inputEvent );
	}

	void Game::processWindowEvent( const WindowEvent& windowEvent )
	{
		m_gameFlow.processWindowEvent( windowEvent );
	}
	
	GameApplication& framework::getGame()
	{
		static Game game;
		return game;
	}
}