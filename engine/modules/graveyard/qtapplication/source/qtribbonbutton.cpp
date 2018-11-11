#include "tiki/qtapplication/qtribbonbutton.hpp"

namespace tiki
{
	QtRibbonButton::QtRibbonButton( const QString& text, const QIcon* pIcon )
		: QToolButton()
	{
		setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
		setMinimumWidth( 75 );

		if( pIcon != nullptr )
		{
			setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
			setIcon( *pIcon );
			setIconSize( QSize( 32, 32 ) );
		}

		setText( text );
	}

	QtRibbonButton::~QtRibbonButton()
	{
	}
}
