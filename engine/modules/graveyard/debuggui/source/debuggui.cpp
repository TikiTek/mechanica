
#include "tiki/debuggui/debuggui.hpp"

#include "tiki/debuggui/debugguiwindow.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	DebugGui::DebugGui()
	{
		m_pDefaultFont = nullptr;
	}

	DebugGui::~DebugGui()
	{
		TIKI_ASSERT( m_pDefaultFont == nullptr );
	}

	bool DebugGui::create( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager )
	{
		if ( !m_renderer.create( grahicsSystem, resourceManager ) )
		{
			dispose( grahicsSystem, resourceManager );
			return false;
		}

		m_isActive = false;

		Vector2 screenSize;
		screenSize.x = (float)grahicsSystem.getBackBuffer().getWidth();
		screenSize.y = (float)grahicsSystem.getBackBuffer().getHeight();
		setScreenSize( screenSize );

		m_pDefaultFont = resourceManager.loadResource< Font >( "debug.font" );
		DebugGuiControl::initialize( m_pDefaultFont, this );

		vector::clear( m_inputState.mousePosition );
		m_inputState.mouseWheel = 0.0f;
		for (uint i = 0u; i < TIKI_COUNT( m_inputState.mouseButtonState ); ++i)
		{
			m_inputState.mouseButtonState[ i ] = false;
		} 

		return true;
	}

	void DebugGui::dispose( GraphicsSystem& grahicsSystem, ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_windows.getCount() == 0u );

		m_renderer.dispose( grahicsSystem, resourceManager );

		if ( m_pDefaultFont != nullptr )
		{
			DebugGuiControl::shutdown();
			resourceManager.unloadResource( m_pDefaultFont );
			m_pDefaultFont = nullptr;
		}
	}

	void DebugGui::addWindow( DebugGuiWindow& window )
	{
		m_windows.push( window );
	}

	void DebugGui::removeWindow( DebugGuiWindow& window )
	{
		if ( !window.getVisibility() )
		{
			m_minimizedLayout.removeChildControl( &window.getMinimizedButton() );
		}

		m_windows.removeSortedByValue( window );
	}

	void DebugGui::setScreenSize( const Vector2& screenSize )
	{
		m_screenSize = screenSize;

		const AxisAlignedRectangle minimizeLayoutRect = createAxisAlignedRectangle( 0.0f, screenSize.y - 35.0f, screenSize.x, 35.0f );
		m_minimizedLayout.setRectangle( minimizeLayoutRect );
	}

	void DebugGui::update( double elapsedTime )
	{
		if ( !m_isActive )
		{
			return;
		}

		for ( LinkedIterator< DebugGuiControl > it = m_windows.getBegin(); it != m_windows.getEnd(); ++it )
		{
			it->update( elapsedTime );
		}

		m_minimizedLayout.update( elapsedTime );
	}

	void DebugGui::render( GraphicsContext& graphicsContext )
	{
		if ( !m_isActive )
		{
			return;
		}

		m_renderer.beginRendering( graphicsContext );
		m_renderer.beginRenderPass();

		for ( LinkedIterator< DebugGuiControl > it = m_windows.getBegin(); it != m_windows.getEnd(); ++it )
		{
			it->render( m_renderer );
		}

		m_renderer.drawRectangle( m_minimizedLayout.getRectangle(), TIKI_COLOR( 255, 255, 255, 128 ) );
		m_minimizedLayout.render( m_renderer );

		const AxisAlignedRectangle mouseRectangle = createAxisAlignedRectangle( m_inputState.mousePosition.x, m_inputState.mousePosition.y, 10.0f, 10.0f );
		m_renderer.drawRectangle( mouseRectangle, TIKI_COLOR_RED );

		m_renderer.endRenderPass();
		m_renderer.endRendering();
	}

	bool DebugGui::processInputEvent( const InputEvent& inputEvent )
	{
		if ( inputEvent.deviceType == InputDeviceType_Mouse )
		{
			switch ( inputEvent.eventType )
			{
			case InputEventType_Mouse_Moved:
				m_inputState.mousePosition.x = inputEvent.data.mouseMoved.xState;
				m_inputState.mousePosition.y = inputEvent.data.mouseMoved.yState;
				break;

			case InputEventType_Mouse_ButtonDown:
				m_inputState.mouseButtonState[ inputEvent.data.mouseButton.button ] = true;
				break;

			case InputEventType_Mouse_ButtonUp:
				m_inputState.mouseButtonState[ inputEvent.data.mouseButton.button ] = false;
				break;

			case InputEventType_Mouse_Wheel:
				m_inputState.mouseWheel += inputEvent.data.mouseWheel.offset;
				break;
			}
		}

		if ( !m_isActive )
		{
			return false;
		}

		for ( LinkedIterator< DebugGuiControl > it = m_windows.getBegin(); it != m_windows.getEnd(); ++it )
		{
			if ( it->processInputEvent( inputEvent, m_inputState ) )
			{
				return true;
			}
		}

		return m_minimizedLayout.processInputEvent( inputEvent, m_inputState );
	}

	void DebugGui::pushEvent( const DebugGuiEvent& guiEvent )
	{
		for ( LinkedIterator< DebugGuiControl > it = m_windows.getBegin(); it != m_windows.getEnd(); ++it )
		{
			DebugGuiWindow& window = static_cast< DebugGuiWindow& >( *it );

			if ( window.processGuiEvent( guiEvent ) )
			{
				return;
			}

			if ( guiEvent.eventType == DebugGuiEventType_Click && guiEvent.pControl == &window.getMinimizedButton() )
			{
				window.setVisibility( !window.getVisibility() );
				m_minimizedLayout.removeChildControl( &window.getMinimizedButton() );
			}
		}
		
		TIKI_TRACE_INFO( "[DebugGui] event of type '%u' was not handled.\n", guiEvent.eventType );
	}

	void DebugGui::minimizeWindow( DebugGuiWindow& window )
	{
		m_minimizedLayout.addChildControl( &window.getMinimizedButton() );
	}
}