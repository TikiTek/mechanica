#include "tiki/qtapplication/qtribbonbutton.hpp"

namespace tiki
{
	QtRibbonButton::QtRibbonButton( const QString& text, QIcon* pIcon )
		: QPushButton( text )
	{
		setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );

		if( pIcon != nullptr )
		{
			setIcon( *pIcon );
			setIconSize( QSize( 24, 24 ) );
		}
	}

	QtRibbonButton::~QtRibbonButton()
	{
	}
}
