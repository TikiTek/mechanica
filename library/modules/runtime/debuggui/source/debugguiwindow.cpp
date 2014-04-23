
#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/base/string.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/input/inputevent.hpp"

namespace tiki
{
	DebugGuiWindow::DebugGuiWindow()
	{
		m_pDebugGui = nullptr;
	}

	DebugGuiWindow::~DebugGuiWindow()
	{
		TIKI_ASSERT( m_pDebugGui == nullptr );
	}

	const char* DebugGuiWindow::getTitle() const
	{
		return m_aTitle;
	}

	void DebugGuiWindow::create( DebugGui& debugGui, const char* pTitle )
	{
		TIKI_ASSERT( m_pDebugGui == nullptr );

		m_pDebugGui	= &debugGui;
		m_isMoving	= false;
		m_isVisible	= true;

		setTitle( pTitle );

		m_pDebugGui->addWindow( *this );
	}

	void DebugGuiWindow::dispose()
	{
		TIKI_ASSERT( m_pDebugGui != nullptr );

		m_pDebugGui->removeWindow( *this );

		m_pDebugGui = nullptr;
		m_isMoving	= false;
		m_isVisible	= false;
	}

	void DebugGuiWindow::setTitle( const char* pTitle )
	{
		TIKI_ASSERT( pTitle != nullptr );

		const uint titleLenght = getStringLength( pTitle );
		TIKI_ASSERT( titleLenght < MaxTitleLength );

		if ( titleLenght < MaxTitleLength )
		{
			memory::copy( m_aTitle, pTitle, titleLenght );
			m_aTitle[ titleLenght ] = '\0';
		}
	}

	bool DebugGuiWindow::getVisibility() const
	{
		return m_isVisible;
	}

	void DebugGuiWindow::setVisibility( bool visible )
	{
		m_isVisible = visible;
		m_isMoving &= visible;
	}

	void DebugGuiWindow::setRectangle( const Rectangle& boundingRectangle )
	{
		m_fullRectangle = boundingRectangle;

		m_titleRectangle.x		= boundingRectangle.x + WindowMargin;
		m_titleRectangle.y		= boundingRectangle.y + WindowMargin;
		m_titleRectangle.width	= boundingRectangle.width - ( WindowMargin * 2.0f );
		m_titleRectangle.height = TitleHeight;

		m_clientRectangle.x			= boundingRectangle.x + WindowMargin;
		m_clientRectangle.y			= m_titleRectangle.y + m_titleRectangle.height + WindowMargin;
		m_clientRectangle.width		= boundingRectangle.width - ( WindowMargin * 2.0f );
		m_clientRectangle.height	= boundingRectangle.height - m_titleRectangle.height - ( WindowMargin * 3.0f );		
	}

	void DebugGuiWindow::update()
	{
		if ( !m_isVisible )
		{
			return;
		}

		// todo

		DebugGuiControl::update();
	}

	void DebugGuiWindow::render( ImmediateRenderer& renderer )
	{
		if ( !m_isVisible )
		{
			return;
		}

		renderer.drawTexture( nullptr, m_fullRectangle, TIKI_COLOR( 64, 64, 64, 128 ) );
		renderer.drawTexture( nullptr, m_titleRectangle, TIKI_COLOR( 128, 128, 128, 196 ) );
		renderer.drawTexture( nullptr, m_clientRectangle, TIKI_COLOR( 196, 196, 196, 128 ) );

		Vector2 textPosition = { WindowMargin, WindowMargin };
		vector::add( textPosition, m_titleRectangle.xy() );
		renderer.drawText( textPosition, *getDefaultFont(), m_aTitle, TIKI_COLOR_WHITE );

		DebugGuiControl::render( renderer );
	}

	bool DebugGuiWindow::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		if ( !m_isVisible )
		{
			return false;
		}

		if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown )
		{
			if ( m_titleRectangle.contains( state.mousePosition ) )
			{
				m_isMoving = true;
				return true;
			}
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && m_isMoving )
		{
			m_isMoving = false;
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_Moved && m_isMoving )
		{
			Rectangle rectangle = m_fullRectangle;
			rectangle.x = m_fullRectangle.x + inputEvent.data.mouseMoved.xOffset;
			rectangle.y = m_fullRectangle.y + inputEvent.data.mouseMoved.yOffset;
			setRectangle( rectangle );
			return true;
		}

		return DebugGuiControl::processInputEvent( inputEvent, state );
	}

}