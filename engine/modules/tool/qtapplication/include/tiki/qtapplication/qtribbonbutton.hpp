#pragma once
#ifndef TIKI_QTRIBBONBUTTON_HPP_INCLUDED
#define TIKI_QTRIBBONBUTTON_HPP_INCLUDED

#include <QToolButton>

namespace tiki
{
	class QtRibbonButton : public QToolButton
	{
		Q_OBJECT

	public:

		QtRibbonButton( const QString& text, const QIcon* pIcon );
		~QtRibbonButton();
	};
}

#endif // TIKI_QTRIBBONBUTTON_HPP_INCLUDED
