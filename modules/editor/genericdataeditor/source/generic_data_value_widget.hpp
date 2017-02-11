#pragma once
#ifndef TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
#define TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED

#include <QWidget>

class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QComboBox;

namespace tiki
{
	class GenericDataValueWidget : public QWidget
	{
		Q_OBJECT

	public:

							GenericDataValueWidget( GenericDataValue* pValue );
							~GenericDataValueWidget();

		void				loadFromValue();
		void				saveToValue();

	private:

		GenericDataValue*		m_pValue;

		QHBoxLayout*			m_pLayout;

		QComboBox*				m_pTag;
		QComboBox*				m_pEnum;
		QLineEdit*				m_pText;
		QPushButton*			m_pTagEnableButton;
	};
}

#endif // TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
