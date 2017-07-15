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

	private slots:

		void				onTagChanged( int index );
		void				onEnumTypeChanged( int index );
		void				onEnumNameChanged( int index );
		void				onTextChanged( const QString& text );
		void				onTagEnableChanged( bool checked );

	private:

		GenericDataTypeCollection&	m_collection;
		GenericDataTagHandler&		m_tagHandler;

		GenericDataValue*			m_pValue;

		QHBoxLayout*				m_pLayout;

		QPushButton*				m_pTagEnableButton;
		bool						m_isInTagMode;

		QComboBox*					m_pTags;
		QComboBox*					m_pEnumType;
		QComboBox*					m_pEnumName;
		QLineEdit*					m_pText;

		GenericDataTag*				getTag();

		void						loadFromValue();

		void						selectTag();
		void						refillEnumNames( const string& enumName );

		void						storeEnumIntoTag();
	};
}

#endif // TIKI_GENERICDATAVALUEBASEWIDGET_HPP_INCLUDED
