
#include "tiki/debuggui/debuggui.hpp"

#include "tiki/debuggui/debugguiwindow.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	bool DebugGui::create( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager, uint maxPageCount )
	{
		if ( !m_windows.create( maxPageCount ) )
		{
			return false;
		}

		if ( !m_renderer.create( grahicsSystem, resourceManager ) )
		{
			dispose( grahicsSystem, resourceManager );
			return false;
		}

		m_isActive = true;

		Vector2 screenSize;
		screenSize.x = (float)grahicsSystem.getBackBuffer().getWidth();
		screenSize.y = (float)grahicsSystem.getBackBuffer().getHeight();
		setScreenSize( screenSize );

		m_pDefaultFont = resourceManager.loadResource< Font >( "debug.font" );

		return true;
	}

	void DebugGui::dispose( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_windows.getCount() == 0u );
		m_windows.dispose();

		m_renderer.dispose( grahicsSystem, resourceManager );

		resourceManager.unloadResource( m_pDefaultFont );
		m_pDefaultFont = nullptr;
	}

	void DebugGui::addWindow( DebugGuiWindow& window )
	{
		m_windows.push( &window );
	}

	void DebugGui::removeWindow( DebugGuiWindow& window )
	{
		m_windows.removeUnsortedByValue( &window );
	}

	void DebugGui::setScreenSize( const Vector2& screenSize )
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

		for ( uint i = 0u; i < m_windows.getCount( ); ++i )
		{
			m_windows[ i ]->setRectangle( boundingRectangle );
		} 
	}

	void DebugGui::update()
	{
		if ( !m_isActive )
		{
			return;
		}

		for (uint i = 0u; i < m_windows.getCount(); ++i)
		{
			m_windows[ i ]->update();
		} 
	}

	void DebugGui::render( GraphicsContext& graphicsContext )
	{
		if ( !m_isActive )
		{
			return;
		}

		m_renderer.drawTexture( nullptr, Rectangle( 0.0f, 0.0f, m_screenSize.x, m_screenSize.y ), TIKI_COLOR( 128, 128, 128, 128 ) );
		
		float xOffset = 5.0f;
		for ( uint i = 0u; i < m_windows.getCount( ); ++i )
		{
			const DebugGuiWindow* pWindow = m_windows[ i ];
			
			Vector2 textSize;
			m_pDefaultFont->calcuateTextSize( textSize, pWindow->getTitle(), getStringLength( pWindow->getTitle() ) );

			const uint alpha = ( i == 0u ? 128 : 64 );
			m_renderer.drawTexture( nullptr, Rectangle( xOffset, 5.0f, textSize.x + 10.0f, 20.0f ), TIKI_COLOR( 64, 64, 64, alpha ) );

			const Vector2 textPosition = { xOffset + 5.0f, 6.0f };
			m_renderer.drawText( textPosition, *m_pDefaultFont, pWindow->getTitle( ), TIKI_COLOR_WHITE );
			
			xOffset += textSize.x + 15.0f;
		}
		m_renderer.drawTexture( nullptr, Rectangle( 5.0f, 25.0f, m_screenSize.x - 10.0f, m_screenSize.y - 30.0f ), TIKI_COLOR( 64, 64, 64, 128 ) );

		for ( uint i = 0u; i < m_windows.getCount(); ++i )
		{
			m_windows[ i ]->render( m_renderer );
		}

		m_renderer.flush( graphicsContext );
	}
}