
#include "tiki/game/debuggui_testwindow.hpp"

namespace tiki
{
	void DebugGuiTestWindow::create( DebugGui& debugGui )
	{
		m_baseLayout.create();
		m_bottomLayout.create();
		m_labelLayout.create();

		DebugGuiWindow::create( debugGui, "Test Window", m_bottomLayout );
		
		for (uint i = 0u; i < TIKI_COUNT( m_labels ); ++i)
		{
			char text[] = "Label0";
			text[ 5u ] = (char)( '0' + i );

			m_labels[ i ].create( text );
			m_labelLayout.addChildControl( &m_labels[ i ] );
		} 

		for (uint i = 0u; i < TIKI_COUNT( m_buttons ); ++i)
		{
			char text[] = "Button0";
			text[ 6u ] = (char)( '0' + i );

			if ( i == 3 )
			{
				m_bottomLayout.addChildControl( &m_labelLayout );
			}

			m_buttons[ i ].create( text );
			m_bottomLayout.addChildControl( &m_buttons[ i ] );
		} 

		m_slider.create( 5.0f, 15.0f, 10.0f );
		m_bottomLayout.addChildControl( &m_slider );

		m_checkBox.create( "Test Box", true );
		m_bottomLayout.addChildControl( &m_checkBox );

		m_baseLayout.addChildControl( &m_bottomLayout );
	}

	void DebugGuiTestWindow::dispose()
	{
		m_baseLayout.dispose();
		m_bottomLayout.dispose();
		m_labelLayout.dispose();

		for (uint i = 0u; i < TIKI_COUNT( m_labels ); ++i)
		{
			m_labels[ i ].dispose();
		}

		for (uint i = 0u; i < TIKI_COUNT( m_buttons ); ++i)
		{
			m_buttons[ i ].dispose();
		}

		DebugGuiWindow::dispose();
	}
}