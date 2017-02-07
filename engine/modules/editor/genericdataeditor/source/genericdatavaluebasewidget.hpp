#pragma once
#ifndef TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
#define TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED

#include <QWidget>

class QHBoxLayout;
class QLineEdit;
class QPushButton;

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
		virtual bool		setValueText( const QString& valueText ) TIKI_PURE;

	private:

		QHBoxLayout*		m_pLayout;
		QLineEdit*			m_pTextBox;
		QPushButton*		m_pTagEditButton;

		void				applyValue();
	};
}

#endif // TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
