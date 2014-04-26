
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
		m_windows.dispose();

		m_renderer.dispose( grahicsSystem, resourceManager );

		DebugGuiControl::shutdown();
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

		for ( uint i = 0u; i < m_windows.getCount(); ++i )
		{
			m_windows[ i ]->render( m_renderer );
		}

		Rectangle mouseReactangle;
		mouseReactangle.x = m_inputState.mousePosition.x;
		mouseReactangle.y = m_inputState.mousePosition.y;
		mouseReactangle.width = 10.0f;
		mouseReactangle.height = 10.0f;
		m_renderer.drawTexture( nullptr, mouseReactangle, TIKI_COLOR_RED );

		m_renderer.flush( graphicsContext );
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

		for ( uint i = 0u; i < m_windows.getCount(); ++i )
		{
			if ( m_windows[ i ]->processInputEvent( inputEvent, m_inputState ) )
			{
				return true;
			}
		}

		return false;
	}

	void DebugGui::pushEvent( const DebugGuiEvent& guiEvent )
	{
		for (uint i = 0u; i < m_windows.getCount(); ++i)
		{
			if ( m_windows[ i ]->processGuiEvent( guiEvent ) )
			{
				return;
			}
		} 

		TIKI_TRACE_INFO( "[DebugGui] event of type '%u' was not handled.\n", guiEvent.eventType );
	}

}