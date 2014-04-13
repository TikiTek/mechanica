
#include "tiki/debugmenu/debugmenu.hpp"

#include "tiki/debugmenu/debugmenupage.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	bool DebugMenu::create( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager, uint maxPageCount )
	{
		if ( !m_pages.create( maxPageCount ) )
		{
			return false;
		}

		if ( !m_renderer.create( grahicsSystem, resourceManager ) )
		{
			dispose( grahicsSystem, resourceManager );
			return false;
		}

		m_isActive			= false;
		m_activePageIndex	= 0u;

		Vector2 screenSize;
		screenSize.x = (float)grahicsSystem.getBackBuffer().getWidth();
		screenSize.y = (float)grahicsSystem.getBackBuffer().getHeight();
		setScreenSize( screenSize );

		m_pFont = resourceManager.loadResource< Font >( "debug.font" );

		return true;
	}

	void DebugMenu::dispose( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_pages.getCount() == 0u );
		m_pages.dispose();

		m_renderer.dispose( grahicsSystem, resourceManager );

		resourceManager.unloadResource( m_pFont );
		m_pFont = nullptr;
	}

	void DebugMenu::addPage( DebugMenuPage& page )
	{
		m_pages.push( &page );
	}

	void DebugMenu::removePage( DebugMenuPage& page )
	{
		m_pages.removeUnsortedByValue( &page );
	}

	void DebugMenu::setScreenSize( const Vector2& screenSize )
	{
		m_screenSize = screenSize;

		Projection projection;
		projection.createOrthographicCenter( screenSize.x, -screenSize.y, 0.0f, 1.0f );
		m_renderer.setProjection( projection );

		Rectangle boundingRectangle;
		boundingRectangle.x			= 25.0f;
		boundingRectangle.y			= 25.0f;
		boundingRectangle.width		= m_screenSize.x - 50.0f;
		boundingRectangle.height	= m_screenSize.y - 50.0f;

		for (uint i = 0u; i < m_pages.getCount(); ++i)
		{
			m_pages[ i ]->setRectangle( boundingRectangle );
		} 
	}

	void DebugMenu::update()
	{
		if ( !m_isActive )
		{
			return;
		}

		m_pages[ m_activePageIndex ]->update();
	}

	void DebugMenu::render( GraphicsContext& graphicsContext )
	{
		if ( !m_isActive )
		{
			return;
		}

		m_renderer.drawTexture( nullptr, Rectangle( 0.0f, 0.0f, m_screenSize.x, m_screenSize.y ), TIKI_COLOR( 128, 128, 128, 128 ) );
		
		float xOffset = 5.0f;
		for (uint i = 0u; i < m_pages.getCount(); ++i)
		{
			const DebugMenuPage* pPage = m_pages[ i ];
			
			Vector2 textSize;
			m_pFont->calcuateTextSize( textSize, pPage->getTitle(), getStringLength( pPage->getTitle() ) );

			const uint alpha = ( m_activePageIndex == i ? 128 : 64 );
			m_renderer.drawTexture( nullptr, Rectangle( xOffset, 5.0f, textSize.x + 10.0f, 20.0f ), TIKI_COLOR( 64, 64, 64, alpha ) );

			const Vector2 textPosition = { xOffset + 5.0f, 6.0f };
			m_renderer.drawText( textPosition, *m_pFont, pPage->getTitle(), TIKI_COLOR_WHITE );
			
			xOffset += textSize.x + 15.0f;
		}
		m_renderer.drawTexture( nullptr, Rectangle( 5.0f, 25.0f, m_screenSize.x - 10.0f, m_screenSize.y - 30.0f ), TIKI_COLOR( 64, 64, 64, 128 ) );

		m_pages[ m_activePageIndex ]->render( m_renderer );

		m_renderer.flush( graphicsContext );
	}
}