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

							GenericDataValueWidget( GenericDataTypeCollection& collection, GenericDataValue* pValue );
							~GenericDataValueWidget();

		void				loadFromValue();
		void				saveToValue();

	private slots:

		void				onTagChanged( int index );
		void				onTagEnableChanged( bool checked );

	private:

		GenericDataTypeCollection&	m_collection;
		GenericDataTagHandler&		m_tagHandler;

		GenericDataValue*			m_pValue;

		QHBoxLayout*				m_pLayout;

		QPushButton*				m_pTagEnableButton;
		bool						m_isInTagMode;

		QComboBox*					m_pTag;
		QComboBox*					m_pEnum;
		QLineEdit*					m_pText;

		void						selectTag();
	};
}

#endif // TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
