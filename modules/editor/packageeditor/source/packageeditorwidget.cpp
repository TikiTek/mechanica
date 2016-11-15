#include "tiki/packageeditor/packageeditorwidget.hpp"

#include "tiki/base/assert.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QTextEdit>
#include <QVBoxLayout>

namespace tiki
{
	PackageEditorWidget::PackageEditorWidget()
		: m_pFile( nullptr )
	{
		m_pAuthorLabel = new QLabel( "Author:" );
		m_pAuthorText = new QLineEdit();

		m_pDescriptionLabel = new QLabel( "Description:" );
		m_pDescriptionText = new QTextEdit();

		m_pDependenciesLabel = new QLabel( "Dependencies:" );
		m_pDependenciesList = new QListView();

		m_pTemplatesLabel = new QLabel( "Templates folder:" );
		m_pTemplatesText = new QLineEdit();

		m_pGenericDataTypesLabel = new QLabel( "Generic data types folder:" );
		m_pGenericDataTypesText = new QLineEdit();

		m_pLayout = new QVBoxLayout();
		m_pLayout->addWidget( m_pAuthorLabel );
		m_pLayout->addWidget( m_pAuthorText );
		m_pLayout->addWidget( m_pDescriptionLabel );
		m_pLayout->addWidget( m_pDescriptionText );
		m_pLayout->addWidget( m_pDependenciesLabel );
		m_pLayout->addWidget( m_pDependenciesList );
		m_pLayout->addWidget( m_pTemplatesLabel );
		m_pLayout->addWidget( m_pTemplatesText );
		m_pLayout->addWidget( m_pGenericDataTypesLabel );
		m_pLayout->addWidget( m_pGenericDataTypesText );

		setLayout( m_pLayout );

		connect( m_pAuthorText, &QLineEdit::textChanged, this, &PackageEditorWidget::textChanged );
		connect( m_pDescriptionText, &QTextEdit::textChanged, this, &PackageEditorWidget::textChanged );
		connect( m_pTemplatesText, &QLineEdit::textChanged, this, &PackageEditorWidget::textChanged );
		connect( m_pGenericDataTypesText, &QLineEdit::textChanged, this, &PackageEditorWidget::textChanged );
	}

	PackageEditorWidget::~PackageEditorWidget()
	{
	}

	bool PackageEditorWidget::openPackage( IEditorFile* pFile )
	{
		m_pFile = pFile;

		const tinyxml2::XMLError error = m_document.LoadFile( pFile->getFileName().toUtf8().data() );
		if( error != tinyxml2::XML_SUCCESS )
		{
			return false;
		}

		m_isLoading = true;
		tinyxml2::XMLElement* pRoot = m_document.RootElement();
		TIKI_ASSERT( pRoot != nullptr );

		tinyxml2::XMLElement* pAuthorElement = pRoot->FirstChildElement( "author" );
		if( pAuthorElement != nullptr )
		{
			m_pAuthorText->setText( pAuthorElement->GetText() );
		}

		tinyxml2::XMLElement* pDescriptionElement = pRoot->FirstChildElement( "description" );
		if( pDescriptionElement != nullptr )
		{
			m_pDescriptionText->setText( pDescriptionElement->GetText() );
		}

		tinyxml2::XMLElement* pTemplatesElement = pRoot->FirstChildElement( "templates" );
		if( pTemplatesElement != nullptr )
		{
			m_pTemplatesText->setText( pTemplatesElement->GetText() );
		}

		tinyxml2::XMLElement* pGenericDataTypesElement = pRoot->FirstChildElement( "genericdatatypes" );
		if( pGenericDataTypesElement != nullptr )
		{
			m_pGenericDataTypesText->setText( pGenericDataTypesElement->GetText() );
		}

		m_isLoading = false;
		return true;
	}

	void PackageEditorWidget::savePackage()
	{
		tinyxml2::XMLElement* pRoot = m_document.RootElement();
		TIKI_ASSERT( pRoot != nullptr );

		pRoot->SetName( "tikipackage" );

		findOrCreateElement( "author" )->SetText( m_pAuthorText->text().toUtf8().data() );
		findOrCreateElement( "description" )->SetText( m_pDescriptionText->toPlainText().toUtf8().data() );
		findOrCreateElement( "templates" )->SetText( m_pTemplatesText->text().toUtf8().data() );
		findOrCreateElement( "genericdatatypes" )->SetText( m_pGenericDataTypesText->text().toUtf8().data() );

		m_document.SaveFile( m_pFile->getFileName().toUtf8().data() );
	}

	void PackageEditorWidget::textChanged()
	{
		if( m_isLoading )
		{
			return;
		}

		m_pFile->markAsDirty();
	}

	tinyxml2::XMLElement* PackageEditorWidget::findOrCreateElement( const char* pName )
	{
		tinyxml2::XMLElement* pElement = m_document.RootElement()->FirstChildElement( pName );
		if( pElement == nullptr )
		{
			pElement = (tinyxml2::XMLElement*)m_document.RootElement()->InsertEndChild( m_document.NewElement( pName ) );
		}
		return pElement;
	}
}
