#include "tiki/qtapplication/qtribbonbutton.hpp"

namespace tiki
{
	QtRibbonButton::QtRibbonButton( const QString& text )
		: QPushButton( text )
	{
		setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
	}

	QtRibbonButton::~QtRibbonButton()
	{
	}
}
