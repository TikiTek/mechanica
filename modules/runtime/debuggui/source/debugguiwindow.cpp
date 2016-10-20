
#include "tiki/debuggui/debugguiwindow.hpp"

#include "tiki/base/string.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/input/inputevent.hpp"

namespace tiki
{
	DebugGuiWindow::DebugGuiWindow()
	{
		m_pDebugGui	= nullptr;
		m_pLayout	= nullptr;
	}

	DebugGuiWindow::~DebugGuiWindow()
	{
		TIKI_ASSERT( m_pDebugGui == nullptr );
		TIKI_ASSERT( m_pLayout == nullptr );
	}

	void DebugGuiWindow::create( DebugGui& debugGui, const char* pTitle, DebugGuiLayout& layout )
	{
		m_pDebugGui		= &debugGui;
		m_pLayout		= &layout;
		m_isMoving		= false;
		m_isVisible		= true;
		m_resizeMode	= WindowResizeMask_None;

		m_pLayout->setParent( this );

		setTitle( pTitle );
		m_minimizeButton.create( "_" );
		m_minimizedButton.create( pTitle );

		m_pDebugGui->addWindow( *this );
	}

	void DebugGuiWindow::dispose()
	{
		if ( m_pDebugGui != nullptr )
		{
			m_pDebugGui->removeWindow( *this );

			m_minimizeButton.dispose();
			m_minimizedButton.dispose();

			m_pLayout->setParent( nullptr );
		}

		m_pDebugGui		= nullptr;
		m_pLayout		= nullptr;
		m_isMoving		= false;
		m_isVisible		= false;
		m_resizeMode	= WindowResizeMask_None;
	}

	const char* DebugGuiWindow::getTitle() const
	{
		return m_aTitle;
	}

	void DebugGuiWindow::setTitle( const char* pTitle )
	{
		TIKI_ASSERT( pTitle != nullptr );
		copyString( m_aTitle, TIKI_COUNT( m_aTitle ), pTitle );
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

	void DebugGuiWindow::handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle )
	{
		m_titleRectangle = createAxisAlignedRectangle(
			boundingRectangle.getLeft() + DebugGui_DefaultPadding,
			boundingRectangle.getTop() + DebugGui_DefaultPadding,
			boundingRectangle.getWidth() - (DebugGui_DefaultPadding * 2.0f + 25.0f),
			TitleHeight
		);

		m_clientRectangle = createAxisAlignedRectangle(
			boundingRectangle.getLeft() + DebugGui_DefaultPadding,
			m_titleRectangle.getBottom() + DebugGui_DefaultPadding,
			boundingRectangle.getWidth() - (DebugGui_DefaultPadding * 2.0f),
			boundingRectangle.getWidth() - m_titleRectangle.getHeight() - (DebugGui_DefaultPadding * 3.0f)
		);

		AxisAlignedRectangle minimizeRect = createAxisAlignedRectangle(
			boundingRectangle.getRight() - (25.0f + DebugGui_DefaultPadding),
			boundingRectangle.getTop() + DebugGui_DefaultPadding,
			25.0f,
			25.0f
		);
		m_minimizeButton.setRectangle( minimizeRect );

		m_pLayout->setRectangle( m_clientRectangle );
	}

	bool DebugGuiWindow::isInHierarchy() const
	{
		return m_isVisible;
	}

	Vector2 DebugGuiWindow::getMinimumSize()
	{
		Vector2 minSize;
		getDefaultFont()->calcuateTextSize( minSize, m_aTitle, getStringSize( m_aTitle ) );
		minSize.y = ( 2.0f * DebugGui_DefaultPadding ) + TitleHeight;

		const Vector2 layoutMinSize = m_pLayout->getMinimumSize();
		minSize.x = TIKI_MAX( minSize.x, layoutMinSize.x );
		minSize.x += 3.0f * DebugGui_DefaultPadding + 25.0f;
		minSize.y += layoutMinSize.y;

		return minSize;
	}

	void DebugGuiWindow::update( double elapsedTime )
	{
		if ( !m_isVisible )
		{
			return;
		}

		m_pLayout->update( elapsedTime );
		m_minimizeButton.update( elapsedTime );
	}

	void DebugGuiWindow::render( ImmediateRenderer& renderer )
	{
		if ( !m_isVisible )
		{
			return;
		}

		renderer.drawRectangle( getRectangle(), TIKI_COLOR( 64, 64, 64, 128 ) );
		renderer.drawRectangle( m_titleRectangle, TIKI_COLOR( 128, 128, 128, 196 ) );
		renderer.drawRectangle( m_clientRectangle, TIKI_COLOR( 196, 196, 196, 128 ) );

		Vector2 textPosition = { DebugGui_DefaultPadding, DebugGui_DefaultPadding };
		vector::add( textPosition, m_titleRectangle.min );
		renderer.drawText( textPosition, *getDefaultFont(), m_aTitle, TIKI_COLOR_WHITE );

		m_pLayout->render( renderer );
		m_minimizeButton.render( renderer );
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
			else
			{
				const float leftDistance	= state.mousePosition.x - getRectangle().getLeft();
				const float rightDistance	= ( getRectangle().getLeft() + getRectangle().getWidth() ) - state.mousePosition.x;
				const float topDistance		= state.mousePosition.y - getRectangle().getTop();
				const float bottomDistance	= ( getRectangle().getTop() + getRectangle().getHeight() ) - state.mousePosition.y;

				m_resizeMode |= ( topDistance > 0.0f && bottomDistance > 0.0f && leftDistance > 0.0f && leftDistance < DebugGui_DefaultPadding ? WindowResizeMask_Left : WindowResizeMask_None );
				m_resizeMode |= ( topDistance > 0.0f && bottomDistance > 0.0f && rightDistance > 0.0f && rightDistance < DebugGui_DefaultPadding ? WindowResizeMask_Right : WindowResizeMask_None );
				m_resizeMode |= ( leftDistance > 0.0f && rightDistance > 0.0f && topDistance > 0.0f && topDistance < DebugGui_DefaultPadding ? WindowResizeMask_Top : WindowResizeMask_None );
				m_resizeMode |= ( leftDistance > 0.0f && rightDistance > 0.0f && bottomDistance > 0.0f && bottomDistance < DebugGui_DefaultPadding ? WindowResizeMask_Bottom : WindowResizeMask_None );
			}
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && ( m_isMoving || m_resizeMode != WindowResizeMask_None ) )
		{
			m_isMoving		= false;
			m_resizeMode	= WindowResizeMask_None;
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			if ( m_isMoving )
			{
				AxisAlignedRectangle rectangle = getRectangle();
				rectangle.translate( vector::create( inputEvent.data.mouseMoved.xOffset, inputEvent.data.mouseMoved.yOffset ) );
				setRectangle( rectangle );
				return true;
			}
			else if ( m_resizeMode != WindowResizeMask_None )
			{
				const InputEventMouseMovedData& moveData = inputEvent.data.mouseMoved;

				AxisAlignedRectangle rectangle = getRectangle();

				if ( isBitSet( m_resizeMode, WindowResizeMask_Left ) )
				{
					if ( getMinimumSize().x >= rectangle.getWidth() - moveData.xOffset )
					{
						return true;
					}

					rectangle.min.x += moveData.xOffset;
				}
				else if ( isBitSet( m_resizeMode, WindowResizeMask_Right ) )
				{
					rectangle.max.x	+= moveData.xOffset;
				}

				if ( isBitSet( m_resizeMode, WindowResizeMask_Top ) )
				{
					if ( getMinimumSize().y >= rectangle.getHeight() - moveData.yOffset )
					{
						return true;
					}

					rectangle.min.y += moveData.yOffset;
				}
				else if ( isBitSet( m_resizeMode, WindowResizeMask_Bottom ) )
				{
					rectangle.max.y += moveData.yOffset;
				}

				setRectangle( rectangle );
				return true;
			}
		}

		if ( m_pLayout->processInputEvent( inputEvent, state ) )
		{
			return true;
		}

		return m_minimizeButton.processInputEvent( inputEvent, state );
	}

	bool DebugGuiWindow::processGuiEvent( const DebugGuiEvent& guiEvent )
	{
		if ( guiEvent.eventType == DebugGuiEventType_Click && guiEvent.pControl == &m_minimizeButton )
		{
			m_pDebugGui->minimizeWindow( *this );

			m_isVisible = false;
			return true;
		}

		return false;
	}

	DebugGuiButton& DebugGuiWindow::getMinimizedButton()
	{
		return m_minimizedButton;
	}
}
