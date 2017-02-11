#include "generic_data_value_widget.hpp"

#include "tiki/toolgenericdata/generic_data_tag.hpp"
#include "tiki/toolgenericdata/genericdatatypeenum.hpp"

#include <QLineEdit>
#include <QPushButton>

namespace tiki
{
	GenericDataValueWidget::GenericDataValueWidget( GenericDataValue* pValue )
		: m_pValue( pValue )
	{
		m_pTagEnableButton = new QPushButton( QIcon( ":/genericdata-editor/tag-editor.png" ), "" );
		m_pTagEnableButton->setCheckable( true );

		m_pTag = new QComboBox();
		m_pEnum = nullptr;
		m_pText = new QLineEdit();

		if( m_pValue->getValueType() == GenericDataValueType_Enum )
		{
			m_pEnum = new QComboBox();
			m_pEnum->setEditable( false );

			TIKI_ASSERT( m_pValue->getType()->getType() == GenericDataTypeType_Enum );
			GenericDataTypeEnum* pEnumType = (GenericDataTypeEnum*)m_pValue->getType();

			for( const GenericDataEnumValue& value : pEnumType->getValues() )
			{
				m_pEnum->addItem( value.name.cStr(), &value );
			}
		}

		m_pLayout = new QHBoxLayout();
		m_pLayout->addWidget( m_pText );
		m_pLayout->addWidget( m_pTagEnableButton );
		m_pLayout->setMargin( 0 );

		setLayout( m_pLayout );

		loadFromValue();
	}

	GenericDataValueWidget::~GenericDataValueWidget()
	{
		delete m_pLayout;

		delete m_pText;
		delete m_pEnum;
		delete m_pTag;

		delete m_pTagEnableButton;
	}

	void GenericDataValueWidget::loadFromValue()
	{
		const GenericDataTag* pValueTag = m_pValue->getValueTag();

		const bool isInTagMode		= m_pTagEnableButton->isChecked();
		const bool willBeInTagMode	= pValueTag != nullptr;

		m_pTagEnableButton->setChecked( willBeInTagMode );

		if( isInTagMode != willBeInTagMode )
		{
			m_pLayout->removeWidget( m_pTag );
			m_pLayout->removeWidget( m_pEnum );
			m_pLayout->removeWidget( m_pText );
			m_pLayout->removeWidget( m_pTagEnableButton );

			if( willBeInTagMode )
			{
				m_pLayout->addWidget( m_pTag );

				if( m_pValue->getValueType() == GenericDataValueType_Enum )
				{
					m_pLayout->addWidget( m_pEnum );
				}
				else
				{
					m_pLayout->addWidget( m_pText );
				}
			}
			else
			{
				m_pLayout->addWidget( m_pText );
			}

			m_pLayout->addWidget( m_pTagEnableButton );
		}

		if( willBeInTagMode )
		{
			if( m_pValue->getValueType() == GenericDataValueType_Enum )
			{
				TIKI_ASSERT( m_pValue->getType()->getType() == GenericDataTypeType_Enum );
				GenericDataTypeEnum* pEnumType = (GenericDataTypeEnum*)m_pValue->getType();

				sint64 enumValue;
				string enumName;
				TIKI_VERIFY( m_pValue->getEnum( enumName, enumValue ) );

				m_pEnum->setCurrentText( enumName.cStr() );
				//QString qtEnumName = enumName.cStr();
				//for( const GenericDataEnumValue& value :  )
				//{
				//	if( qtEnumName == )
				//	m_pEnum->addItem( value.name.cStr(), &value );
				//}
				//
				//m_pEnum->inde
			}
			else
			{
				m_pText->setText( pValueTag->getContent().cStr() );
			}
		}
		else
		{
			m_pText->setText( m_pValue->toString().cStr() );
		}
	}

	void GenericDataValueWidget::saveToValue()
	{

	}
}