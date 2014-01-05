
#include "tiki/game/tikiarenagame.hpp"

#include "tiki/gamestates/applicationstate.hpp"
#include "tiki/gamestates/introstate.hpp"
#include "tiki/gamestates/playstate.hpp"
#include "tiki/gamestates/teststate.hpp"

#include <windows.h>

namespace tiki
{
	struct TikiArenaStates
	{
		IntroState			introState;
		ApplicationState	applicationState;
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
			return TikiArenaGameStates_Test;
		}

		return TikiArenaGameStates_Play;
	}

	void TikiArenaGame::fillParameters( GameFrameworkParamters& parameters )
	{
		parameters.screenWidth	= 1280;
		parameters.screenHeight	= 720;

		parameters.graphicsMode = GraphicsRendererMode_Wrapper;
	}

	void TikiArenaGame::initialize()
	{
		m_pStates = memory::newAlign< TikiArenaStates >();
		m_pStates->applicationState.create();
		m_pStates->introState.create();
		m_pStates->playState.create( &m_pStates->applicationState );
		m_pStates->testState.create( &m_pStates->applicationState );

		GameStateDefinition gameDefinition[] =
		{
			{ nullptr,								0u,	0u },
				{ &m_pStates->introState,			0u,	IntroStateTransitionSteps_Count },			// F5
				{ &m_pStates->applicationState,		0u,	ApplicationStateTransitionSteps_Count },	// F6
					{ &m_pStates->playState,		2u,	PlayStateTransitionSteps_Count },			// F7
					{ &m_pStates->testState,		2u,	TestStateTransitionSteps_Count }			// F8
		};

		m_gameFlow.create( gameDefinition, TIKI_COUNT( gameDefinition ) );
		m_gameFlow.startTransition( getStartState() );
	}

	void TikiArenaGame::shutdown()
	{
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
			}

			m_gameFlow.dispose();
		}

		if ( m_pStates != nullptr )
		{
			m_pStates->applicationState.dispose();
			m_pStates->introState.dispose();
			m_pStates->playState.dispose();
			m_pStates->testState.dispose();
			memory::deleteAlign( m_pStates );
		}
	}

	void TikiArenaGame::update()
	{
		m_gameFlow.update();

		if ( !m_gameFlow.isInTransition() )
		{
			InputSystem& input = framework::getInputSystem();

			for (int i = 0; i < (int)m_gameFlow.getStateCount() - 1; ++i)
			{
				if ( input.hasKeyPressed( (Keys)( KEY_F5 + i ) ) )
				{
					m_gameFlow.startTransition( i + 1 );
				}
			}
		}

		Sleep( 15 );
	}

	void TikiArenaGame::render( GraphicsContext& graphicsContext ) const
	{
		m_gameFlow.render( graphicsContext );
	}

	GameFramework& framework::getGame()
	{
		static TikiArenaGame game;
		return game;
	}
}