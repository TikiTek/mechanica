#pragma once
#ifndef TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
#define TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED

#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QTextBox;

namespace tiki
{
	class GenericDataValueBaseWidget : QWidget
	{
		Q_OBJECT

	public:

		GenericDataValueBaseWidget( GenericDataValue* pValue );
		~GenericDataValueBaseWidget();

	protected:

		GenericDataValue*	m_pValue;

		virtual QString		getValueText() const TIKI_PURE;
		virtual bool		getValueText( const QString& valueText ) TIKI_PURE;

	private:

		QHBoxLayout*		m_pLayout;
		QTextLine*			m_pTextBox;
		QPushButton*		m_pTagEditButton;
	};
}

#endif // TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
