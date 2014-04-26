
#include "tiki/game/debuggui_testwindow.hpp"

namespace tiki
{
	void DebugGuiTestWindow::create( DebugGui& debugGui )
	{
		DebugGuiWindow::create( debugGui, "Test Window", m_layout );

		for (uint i = 0u; i < TIKI_COUNT( m_buttons ); ++i)
		{
			char text[] = "Button0";
			text[ 6u ] = (char)( '0' + i );

			m_buttons[ i ].create( text );
			m_layout.addChildControl( &m_buttons[ i ] );
		} 
	}

	void DebugGuiTestWindow::dispose()
	{
		for (uint i = 0u; i < TIKI_COUNT( m_buttons ); ++i)
		{
			m_layout.removeChildControl( &m_buttons[ i ] );
			m_buttons[ i ].dispose();
		}

		DebugGuiWindow::dispose();
	}
}