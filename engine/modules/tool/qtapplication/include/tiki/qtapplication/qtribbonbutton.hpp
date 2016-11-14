#pragma once
#ifndef TIKI_QTRIBBONBUTTON_HPP_INCLUDED
#define TIKI_QTRIBBONBUTTON_HPP_INCLUDED

#include <QPushButton>

namespace tiki
{
	class QtRibbonButton : public QPushButton
	{
		Q_OBJECT

	public:

		QtRibbonButton( const QString& text, QIcon* pIcon );
		~QtRibbonButton();
	};
}

#endif // TIKI_QTRIBBONBUTTON_HPP_INCLUDED
