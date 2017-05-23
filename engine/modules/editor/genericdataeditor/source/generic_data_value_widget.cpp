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

		const GenericDataTag* pValueTag = m_pValue->getValueTag();

		const GenericDataTypeEnum* pEnumType = nullptr;
		string enumName;
		if( pValueTag != nullptr )
		{
			m_tagHandler.parseEnum( &pEnumType, enumName, pValueTag->getContent() );
		}

		m_pTags = new QComboBox();
		m_pTags->setEditable( false );
		for( uint i = 0u; i < GenericDataValueTag_Count; ++i )
		{
			const char* pName = m_tagHandler.getValueTag( (GenericDataValueTag)i );
			m_pTags->addItem( pName, i );
		}
		connect( m_pTags, static_cast<void (QComboBox::*)(int index)>( &QComboBox::currentIndexChanged ), this, &GenericDataValueWidget::onTagChanged );

		m_pEnumType = new QComboBox();
		m_pEnumType->setEditable( false );
		for( const GenericDataType& type : m_collection.getTypes() )
		{
			if( type.getType() != GenericDataTypeType_Enum )
			{
				continue;
			}

			const GenericDataTypeEnum& enumType = (const GenericDataTypeEnum&)type;
			m_pEnumType->addItem( enumType.getName().cStr(), qVariantFromValue( (void*)&enumType ) );

			if( &enumType == pEnumType )
			{
				m_pEnumType->setCurrentIndex( m_pEnumType->count() - 1 );
			}
		}
		connect( m_pEnumType, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &GenericDataValueWidget::onEnumTypeChanged );

		m_pEnumName = new QComboBox();
		m_pEnumName->setEditable( false );
		refillEnumNames( enumName );
		connect( m_pEnumName, static_cast<void (QComboBox::*)(int index)>( &QComboBox::currentIndexChanged ), this, &GenericDataValueWidget::onEnumNameChanged );

		m_pText = new QLineEdit();
		connect( m_pText, &QLineEdit::textChanged, this, &GenericDataValueWidget::onTextChanged );

		m_pLayout = new QHBoxLayout();
		m_pLayout->setMargin( 0 );

		m_pLayout->addWidget( m_pTags );
		m_pLayout->addWidget( m_pEnumType );
		m_pLayout->addWidget( m_pEnumName );
		m_pLayout->addWidget( m_pText );
		m_pLayout->addWidget( m_pTagEnableButton );

		setLayout( m_pLayout );

		loadFromValue();
	}

	GenericDataValueWidget::~GenericDataValueWidget()
	{
		delete m_pLayout;

		delete m_pTags;
		delete m_pEnumType;
		delete m_pEnumName;
		delete m_pText;
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

		if( willBeInTagMode )
		{
			m_pTags->show();
			selectTag();


		}
		else
		{
			m_pTags->hide();
			m_pEnumType->hide();
			m_pEnumName->hide();
			m_pText->show();
		}

		if( willBeInTagMode )
		{
			//const GenericDataTypeEnum* pEnumType = nullptr;
			//string valueName;
			//m_tagHandler.parseEnum( &pEnumType, valueName, pValueTag->getContent() );

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

					m_pEnumName->setCurrentText( enumName.cStr() );
				}
				else
				{
					//m_pEnum->setCurrentText( pValueTag->getContent().cStr() );
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

	void GenericDataValueWidget::onEnumTypeChanged( int index )
	{
		refillEnumNames( m_pEnumName->currentText().toUtf8().constData() );
		storeEnumIntoTag();
	}

	void GenericDataValueWidget::onEnumNameChanged( int index )
	{
		storeEnumIntoTag();
	}

	void GenericDataValueWidget::onTextChanged( const QString& text )
	{
		const string content = text.toUtf8().constData();
		if( m_isInTagMode )
		{
			getTag()->setContent( content );
		}
		else
		{
			if( !m_collection.parseValue( m_pValue, content, m_pValue->getType(), nullptr ) )
			{
				QPalette textPalette;
				textPalette.setColor( QPalette::Text, Qt::red );

				m_pText->setPalette( textPalette );
			}
			else
			{
				m_pText->setPalette( QPalette() );
			}
		}
	}

	void GenericDataValueWidget::onTagEnableChanged( bool checked )
	{
		loadFromValue();
	}

	GenericDataTag* GenericDataValueWidget::getTag()
	{
		GenericDataTag* pValueTag = m_pValue->getValueTag();
		if( pValueTag == nullptr )
		{
			GenericDataTag* pNewTag = TIKI_NEW( GenericDataTag );
			m_pValue->setValueTag( pNewTag );

			pValueTag = pNewTag;
		}

		return pValueTag;
	}

	void GenericDataValueWidget::selectTag()
	{
		m_pEnumType->hide();
		m_pEnumName->hide();
		m_pText->hide();

		const string& tag = getTag()->getTag();
		const QString tagQt = tag.cStr();
		for( int i = 0u; i < m_pTags->count(); ++i )
		{
			if( m_pTags->itemText( i ) == tagQt )
			{
				m_pTags->setCurrentIndex( i );
				break;
			}
		}

		if( tag == m_tagHandler.getValueTag( GenericDataValueTag_Enum ) )
		{
			refillEnumNames( m_pEnumName->currentText().toUtf8().constData() );

			m_pEnumType->show();
			m_pEnumName->show();
		}
		else
		{
			m_pText->show();
		}
	}

	void GenericDataValueWidget::refillEnumNames( const string& enumName )
	{
		m_pEnumName->clear();

		const int typeIndex = m_pEnumType->currentIndex();
		const QVariant typeValue = m_pEnumType->itemData( typeIndex, Qt::UserRole );
		const GenericDataTypeEnum* pEnumType = (const GenericDataTypeEnum*)typeValue.value< void* >();
		TIKI_ASSERT( pEnumType != nullptr );

		for( const GenericDataEnumValue& value : pEnumType->getValues() )
		{
			m_pEnumName->addItem( value.name.cStr(), qVariantFromValue( (void*)&value ) );

			if( enumName == value.name )
			{
				m_pEnumName->setCurrentIndex( m_pEnumName->count() - 1 );
			}
		}
	}

	void GenericDataValueWidget::storeEnumIntoTag()
	{
		TIKI_ASSERT( m_isInTagMode );

		const int typeIndex = m_pEnumType->currentIndex();
		if( typeIndex < 0 )
		{
			return;
		}

		const QVariant typeValue = m_pEnumType->itemData( typeIndex );
		const GenericDataTypeEnum* pEnumType = (const GenericDataTypeEnum*)typeValue.value< void* >();
		TIKI_ASSERT( pEnumType != nullptr );

		const int nameIndex = m_pEnumName->currentIndex();
		if( nameIndex < 0 )
		{
			return;
		}

		const QVariant nameValue = m_pEnumName->itemData( nameIndex );
		const GenericDataEnumValue* pEnumValue = (const GenericDataEnumValue*)nameValue.value< void* >();
		TIKI_ASSERT( pEnumValue != nullptr );

		const string content = m_tagHandler.encodeEnum( pEnumType, *pEnumValue );
		getTag()->setContent( content );
	}
}