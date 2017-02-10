#pragma once
#ifndef TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
#define TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED

#include <QWidget>

class QHBoxLayout;
class QLineEdit;
class QPushButton;

namespace tiki
{
	class GenericDataValueWidget : public QWidget
	{
		Q_OBJECT

	public:

		GenericDataValueWidget( GenericDataValue* pValue );
		~GenericDataValueWidget();

	private:

		GenericDataValue*	m_pValue;

		QHBoxLayout*		m_pLayout;
		QLineEdit*			m_pTextBox;
		QPushButton*		m_pTagEditButton;

		void				applyValue();
	};
}

#endif // TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
