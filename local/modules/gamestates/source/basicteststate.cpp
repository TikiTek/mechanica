#include "tiki/gamestates/basicteststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/game/game.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/math/axisalignedrectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	BasicTestState::BasicTestState()
	{
		m_pGame = nullptr;
	}

	BasicTestState::~BasicTestState()
	{
		TIKI_ASSERT( m_pGame == nullptr );
	}

	void BasicTestState::create( Game* pGame )
	{
		TIKI_ASSERT( pGame != nullptr );
		TIKI_ASSERT( m_pGame == nullptr );

		m_pGame			= pGame;

		m_pTestTexture	= nullptr;
		m_pTestFont		= nullptr;
	}

	void BasicTestState::dispose()
	{
		m_pGame = nullptr;
	}

	TransitionState BasicTestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
		switch ( currentStep )
		{
		case BasicTestStateTransitionSteps_CreateRenderer:
			{
				TIKI_ASSERT( isInital );

				if ( isCreating )
				{
					if( !m_renderer.create( m_pGame->getGraphicsSystem(), m_pGame->getResourceManager() ) )
					{
						return TransitionState_Error;
					}
				}
				else
				{
					m_renderer.dispose( m_pGame->getGraphicsSystem(), m_pGame->getResourceManager() );
				}
			}
			break;

		case BasicTestStateTransitionSteps_LoadResources:
			{
				TIKI_ASSERT( isInital );

				if ( isCreating )
				{
					m_pTestTexture = m_pGame->getResourceManager().loadResource< Texture >( "checker.texture" );
					m_pTestFont = m_pGame->getResourceManager().loadResource< Font >( "debug.font" );

					bool ok = true;
					ok &= ( m_pTestTexture != nullptr );
					ok &= ( m_pTestFont != nullptr );

					if ( !ok )
					{
						return TransitionState_Error;
					}
				}
				else
				{
					m_pGame->getResourceManager().unloadResource( m_pTestTexture );
					m_pGame->getResourceManager().unloadResource( m_pTestFont );
					m_pTestTexture	= nullptr;
					m_pTestFont		= nullptr;
				}
			}
			break;
		}

		return TransitionState_Finish;
	}

	void BasicTestState::update()
	{
	}

	void BasicTestState::render( GraphicsContext& graphicsContext )
	{
		const Color aColors[] =
		{
			TIKI_COLOR_RED,
			TIKI_COLOR_GREEN,
			TIKI_COLOR_BLUE,
			TIKI_COLOR_YELLOW,
			TIKI_COLOR_PINK,
			TIKI_COLOR_PURPLE,
			TIKI_COLOR_ORANGE
		};
		const uint colorIndex = uint( m_pGame->getFrameTimer().getTotalTime() / 5.0 );

		graphicsContext.clear( graphicsContext.getBackBuffer(), aColors[ colorIndex % TIKI_COUNT( aColors ) ] );

		m_renderer.beginRendering( graphicsContext );
		m_renderer.beginRenderPass();

		m_renderer.drawRectangle( createAxisAlignedRectangle( 50.0f, 50.0f, 100.0f, 100.0f ), aColors[ (colorIndex + 1u) % TIKI_COUNT( aColors ) ] );
		m_renderer.drawTexturedRectangle( m_pTestTexture->getTextureData(), createAxisAlignedRectangle( 200.0f, 50.0f, 100.0f, 100.0f ), TIKI_COLOR_WHITE );
		m_renderer.drawTexturedRectangle( m_pTestFont->getTextureData(), createAxisAlignedRectangle( 350.0f, 50.0f, 256.0f, 256.0f ), TIKI_COLOR_WHITE );
		m_renderer.drawTexturedRectangle( m_pTestFont->getTextureData(), createAxisAlignedRectangle( 350.0f, 350.0f, 20, 20), createAxisAlignedRectangle( 140.0f, 20.0f, 20.0f, 20.0f ), TIKI_COLOR_WHITE );
		m_renderer.drawTexturedRectangle( m_pTestFont->getTextureData(), createAxisAlignedRectangle( 350.0f, 400.0f, 8, 9 ), createAxisAlignedRectangle( 147.0f, 21.0f, 8.0f, 9.0f ), TIKI_COLOR_WHITE );

		m_renderer.drawText( Vector2::zero, *m_pTestFont, "Hello World!", aColors[ (colorIndex + 1u) % TIKI_COUNT( aColors ) ] );
		m_renderer.drawText( vector::create( 0.0f, 10.0f ), *m_pTestFont, "-.,;:+-!\"$%&/()=?", aColors[ (colorIndex + 1u) % TIKI_COUNT( aColors ) ] );

		m_renderer.endRenderPass();
		m_renderer.endRendering();
	}

	bool BasicTestState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}
}