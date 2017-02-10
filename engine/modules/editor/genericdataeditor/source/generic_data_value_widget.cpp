#include "generic_data_value_widget.hpp"

#include "tiki/toolgenericdata/generic_data_tag.hpp"

#include <QLineEdit>
#include <QPushButton>

namespace tiki
{
	GenericDataValueWidget::GenericDataValueWidget( GenericDataValue* pValue )
		: m_pValue( pValue )
	{
		m_pTextBox = new QLineEdit();
		m_pTagEditButton = new QPushButton( QIcon( ":/genericdata-editor/tag-editor.png" ), "" );

		m_pLayout = new QHBoxLayout();
		m_pLayout->addWidget( m_pTextBox );
		m_pLayout->addWidget( m_pTagEditButton );

		setLayout( m_pLayout );

		applyValue();
	}

	GenericDataValueWidget::~GenericDataValueWidget()
	{

	}

	void GenericDataValueWidget::applyValue()
	{
		const GenericDataTag* pValueTag = m_pValue->getValueTag();
		if( pValueTag != nullptr )
		{
			m_pTextBox->setEnabled( false );
			m_pTextBox->setText( pValueTag->writeTagString().cStr() );
		}
		else
		{
			m_pTextBox->setEnabled( true );
			m_pTextBox->setText( m_pValue->toString().cStr() );
		}
	}
}