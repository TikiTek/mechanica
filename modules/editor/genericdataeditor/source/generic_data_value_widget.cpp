#include "generic_data_value_widget.hpp"

#include "tiki/toolgenericdata/generic_data_tag.hpp"
#include "tiki/toolgenericdata/genericdatatypeenum.hpp"

#include <QLineEdit>
#include <QPushButton>

namespace tiki
{
	GenericDataValueWidget::GenericDataValueWidget( GenericDataTypeCollection& collection, GenericDataValue* pValue )
		: m_collection( collection )
		, m_tagHandler( collection.getTagHandler() )
		, m_pValue( pValue )
	{
		m_pTagEnableButton = new QPushButton( QIcon( ":/genericdata-editor/tag-editor.png" ), "" );
		m_pTagEnableButton->setCheckable( true );
		m_pTagEnableButton->setChecked( m_pValue->getValueTag() != nullptr );
		connect( m_pTagEnableButton, &QPushButton::toggled, this, &GenericDataValueWidget::onTagEnableChanged );

		// set to other state to initialize in correct mode
		m_isInTagMode = !m_pTagEnableButton->isChecked();

		m_pTag = new QComboBox();
		for( uint i = 0u; i < GenericDataValueTag_Count; ++i )
		{
			const char* pName = m_tagHandler.getValueTag( (GenericDataValueTag)i );
			m_pTag->addItem( pName, i );
		}
		connect( m_pTag, static_cast<void (QComboBox::*)(int index)>( &QComboBox::currentIndexChanged ), this, &GenericDataValueWidget::onTagChanged );

		m_pEnum = new QComboBox();
		m_pEnum->setEditable( false );

		m_pText = new QLineEdit();

		m_pLayout = new QHBoxLayout();
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
		const bool willBeInTagMode	= m_pTagEnableButton->isChecked();
		if( m_isInTagMode == willBeInTagMode )
		{
			return;
		}
		m_isInTagMode = willBeInTagMode;

		m_pLayout->removeWidget( m_pTag );
		m_pLayout->removeWidget( m_pEnum );
		m_pLayout->removeWidget( m_pText );
		m_pLayout->removeWidget( m_pTagEnableButton );

		if( willBeInTagMode )
		{
			m_pLayout->addWidget( m_pTag );

			selectTag();
		}
		else
		{
			m_pLayout->addWidget( m_pText );
		}

		m_pLayout->addWidget( m_pTagEnableButton );

		if( willBeInTagMode )
		{
			const GenericDataTag* pValueTag = m_pValue->getValueTag();

			if( pValueTag->getTag() == m_tagHandler.getValueTag( GenericDataValueTag_Enum ) )
			{
				//TIKI_ASSERT( m_pValue->getType()->getType() == GenericDataTypeType_Enum );
				//GenericDataTypeEnum* pEnumType = (GenericDataTypeEnum*)m_pValue->getType();

				if( m_pValue->getValueType() == GenericDataValueType_Enum )
				{
					sint64 enumValue;
					string enumName;
					TIKI_VERIFY( m_pValue->getEnum( enumName, enumValue ) );

					m_pEnum->setCurrentText( enumName.cStr() );
				}
				else
				{
					m_pEnum->setCurrentText( pValueTag->getContent().cStr() );
				}
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

	void GenericDataValueWidget::onTagChanged( int index )
	{
		TIKI_ASSERT( m_isInTagMode );

		GenericDataTag* pValueTag = m_pValue->getValueTag();
		if( pValueTag == nullptr )
		{
			TIKI_TRACE_ERROR( "[GenericDataValueWidget::onTagChanged] value has no tag.\n" );
			return;
		}

		const char* pTagName = m_tagHandler.getValueTag( (GenericDataValueTag)index );
		if( pTagName == pValueTag->getTag() )
		{
			return;
		}

		pValueTag->setTag( pTagName );

		selectTag();
	}

	void GenericDataValueWidget::onTagEnableChanged( bool checked )
	{
		loadFromValue();
	}

	void GenericDataValueWidget::selectTag()
	{
		m_pLayout->removeWidget( m_pEnum );
		m_pLayout->removeWidget( m_pText );

		const GenericDataTag* pValueTag = m_pValue->getValueTag();

		m_pTag->setCurrentText( pValueTag->getTag().cStr() );

		if( pValueTag->getTag() == m_tagHandler.getValueTag( GenericDataValueTag_Enum ) )
		{
			m_pEnum->clear();

			const GenericDataTypeEnum* pEnumType = nullptr;
			string valueName;
			m_tagHandler.parseEnum( &pEnumType, valueName, pValueTag->getContent() );

			for( const GenericDataEnumValue& value : pEnumType->getValues() )
			{
				m_pEnum->addItem( value.name.cStr(), &value );

				if( value.name == valueName )
				{
					m_pEnum->setCurrentIndex( m_pEnum->count() - 1 );
				}
			}

			m_pLayout->insertWidget( m_isInTagMode ? 1 : 0, m_pEnum );
		}
		else
		{
			m_pLayout->insertWidget( m_isInTagMode ? 1 : 0, m_pText );
		}
	}
}