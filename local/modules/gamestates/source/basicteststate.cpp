
#include "tiki/gamestates/basicteststate.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	void BasicTestState::create()
	{
		m_renderer.create( framework::getGraphicsSystem(), framework::getResourceManager() );

		m_pTestTexture = framework::getResourceManager().loadResource< Texture >( "checker.texture" );
		m_pTestFont = framework::getResourceManager().loadResource< Font >( "debug.font" );
		TIKI_ASSERT( m_pTestTexture != nullptr );
		TIKI_ASSERT( m_pTestFont != nullptr );
	}

	void BasicTestState::dispose()
	{
		framework::getResourceManager().unloadResource( m_pTestTexture );
		framework::getResourceManager().unloadResource( m_pTestFont );

		m_renderer.dispose( framework::getGraphicsSystem(), framework::getResourceManager() );
	}

	TransitionState BasicTestState::processTransitionStep( size_t currentStep, bool isCreating, bool isInital )
	{
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
		const uint colorIndex = uint( framework::getFrameTimer().getTotalTime() / 5.0 );

		graphicsContext.clear( graphicsContext.getBackBuffer(), aColors[ colorIndex % TIKI_COUNT( aColors ) ] );

		m_renderer.beginRendering( graphicsContext );
		m_renderer.beginRenderPass();

		m_renderer.drawRectangle( Rectangle( 50.0f, 50.0f, 100.0f, 100.0f ), aColors[ (colorIndex + 1u) % TIKI_COUNT( aColors ) ] );
		m_renderer.drawTexturedRectangle( m_pTestTexture->getTextureData(), Rectangle( 200.0f, 50.0f, 100.0f, 100.0f ), TIKI_COLOR_WHITE );
		m_renderer.drawTexturedRectangle( m_pTestFont->getTextureData(), Rectangle( 350.0f, 50.0f, 100.0f, 100.0f ), TIKI_COLOR_WHITE );

		m_renderer.drawText( Vector2::zero, *m_pTestFont, "Hello World!", aColors[ (colorIndex + 1u) % TIKI_COUNT( aColors ) ] );

		m_renderer.endRenderPass();
		m_renderer.endRendering();
	}

	bool BasicTestState::processInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

}