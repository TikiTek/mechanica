#include "genericdatavaluebasewidget.hpp"

#include "tiki/toolgenericdata/genericdatavaluetag.hpp"

#include <QLineEdit>
#include <QPushButton>

namespace tiki
{
	GenericDataValueBaseWidget::GenericDataValueBaseWidget( GenericDataValue* pValue )
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

	GenericDataValueBaseWidget::~GenericDataValueBaseWidget()
	{

	}

	void GenericDataValueBaseWidget::applyValue()
	{
		const GenericDataValueTag* pValueTag = m_pValue->getValueTag();
		if( pValueTag != nullptr )
		{
			m_pTextBox->setEnabled( false );
			m_pTextBox->setText( pValueTag->writeTagString().cStr() );
		}
		else
		{
			// ...
		}
	}
}