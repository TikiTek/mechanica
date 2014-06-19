
#include "tiki/game/tikiarenagame.hpp"

#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/gamestates/basicteststate.hpp"
#include "tiki/gamestates/introstate.hpp"
#include "tiki/gamestates/playstate.hpp"
#include "tiki/gamestates/teststate.hpp"

#include <windows.h>

namespace tiki
{
	struct TikiArenaStates
	{
		ApplicationState	applicationState;
		BasicTestState		basicTestState;
		IntroState			introState;
		PlayState			playState;
		TestState			testState;
	};

	static TikiArenaGameStates getStartState()
	{
		char buffer[ 32u ];
		DWORD length = sizeof( buffer );
		GetUserNameA( buffer, &length );

		const string userName = buffer;

		if ( userName == "Tim" || userName == "tim.boden" )
		{
			//return TikiArenaGameStates_BasicTest;
			return TikiArenaGameStates_Test;
		}

		return TikiArenaGameStates_Play;
	}

	void TikiArenaGame::fillParameters( GameFrameworkParamters& parameters )
	{
		parameters.screenWidth	= 1280;
		parameters.screenHeight	= 720;

		parameters.graphicsMode = GraphicsRendererMode_Hardware;

#if TIKI_ENABLED( TIKI_BUILD_MASTER )
		parameters.pGamebuildPath = "./gamebuild/";
#endif
	}

	bool TikiArenaGame::initialize()
	{
		m_factories.create( framework::getResourceManager(), framework::getGraphicsSystem() );

		m_pStates = memory::newAlign< TikiArenaStates >();
		m_pStates->applicationState.create();
		m_pStates->basicTestState.create();
		m_pStates->introState.create();
		m_pStates->playState.create( &m_pStates->applicationState );
		m_pStates->testState.create( &m_pStates->applicationState );

		GameStateDefinition gameDefinition[] =
		{
			{ nullptr,								0u,	0u,										"Root" },
				{ &m_pStates->introState,			0u,	IntroStateTransitionSteps_Count,		"IntroState" },			// F5
				{ &m_pStates->applicationState,		0u,	ApplicationStateTransitionSteps_Count,	"ApplicationState" },	// F6
					{ &m_pStates->playState,		2u,	PlayStateTransitionSteps_Count,			"PlayState" },			// F7
					{ &m_pStates->testState,		2u,	TestStateTransitionSteps_Count,			"TestState" },			// F8
				{ &m_pStates->basicTestState,		0u,	BasicTestStateTransitionSteps_Count,	"BasicTestState" }		// F9
		};
		TIKI_COMPILETIME_ASSERT( TIKI_COUNT( gameDefinition ) == TikiArenaGameStates_Count );

		m_gameFlow.create( gameDefinition, TIKI_COUNT( gameDefinition ) );
		m_gameFlow.startTransition( getStartState() );

		if ( !m_touchSystem.create( framework::getGraphicsSystem(), framework::getResourceManager() ) )
		{
			return false;
		}

		return true;
	}

	void TikiArenaGame::shutdown()
	{
		m_touchSystem.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );

		if ( m_gameFlow.isCreated() )
		{
			while ( m_gameFlow.isInTransition() )
			{
				m_gameFlow.update();
			}

			m_gameFlow.startTransition( TikiArenaGameStates_Root );

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
			m_pStates->basicTestState.dispose();
			m_pStates->introState.dispose();
			m_pStates->playState.dispose();
			m_pStates->testState.dispose();

			memory::deleteAlign( m_pStates );
		}
	}

	void TikiArenaGame::update()
	{
		m_touchSystem.update( float( framework::getFrameTimer().getElapsedTime() ), framework::getGraphicsSystem() );
		for (uint i = 0u; i < m_touchSystem.getInputEventCount(); ++i)
		{
			processInputEvent( m_touchSystem.getInputEventByIndex( i ) );
		} 

		m_gameFlow.update();
	}

	void TikiArenaGame::render( GraphicsContext& graphicsContext ) const
	{
		m_gameFlow.render( graphicsContext );

		m_touchSystem.render( graphicsContext );
	}

	bool TikiArenaGame::processInputEvent( const InputEvent& inputEvent )
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

	GameFramework& framework::getGame()
	{
		static TikiArenaGame game;
		return game;
	}
}