#include "tiki/package_editor/package_file.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	PackageFile::PackageFile( const Path& filename, PackageEditor* pEditor )
		: EditableFile( filename, pEditor )
	{
	}

	PackageFile::~PackageFile()
	{
	}

	bool PackageFile::load()
	{
		const tinyxml2::XMLError error = m_document.LoadFile( getFileName().getCompletePath() );
		if( error != tinyxml2::XML_SUCCESS )
		{
			return false;
		}

		tinyxml2::XMLElement* pRoot = m_document.RootElement();
		TIKI_ASSERT( pRoot != nullptr );

		tinyxml2::XMLElement* pAuthorElement = pRoot->FirstChildElement( "author" );
		if( pAuthorElement != nullptr )
		{
			//m_pAuthorText->setText( pAuthorElement->GetText() );
		}

		tinyxml2::XMLElement* pDescriptionElement = pRoot->FirstChildElement( "description" );
		if( pDescriptionElement != nullptr )
		{
			//m_pDescriptionText->setText( pDescriptionElement->GetText() );
		}

		tinyxml2::XMLElement* pTemplatesElement = pRoot->FirstChildElement( "templates" );
		if( pTemplatesElement != nullptr )
		{
			//m_pTemplatesText->setText( pTemplatesElement->GetText() );
		}

		tinyxml2::XMLElement* pGenericDataTypesElement = pRoot->FirstChildElement( "genericdatatypes" );
		if( pGenericDataTypesElement != nullptr )
		{
			//m_pGenericDataTypesText->setText( pGenericDataTypesElement->GetText() );
		}

		return true;
	}

	bool PackageFile::save()
	{
		tinyxml2::XMLElement* pRoot = m_document.RootElement();
		TIKI_ASSERT( pRoot != nullptr );

		pRoot->SetName( "package" );

		//findOrCreateElement( "author" )->SetText( m_pAuthorText->text().toUtf8().data() );
		//findOrCreateElement( "description" )->SetText( m_pDescriptionText->toPlainText().toUtf8().data() );
		//findOrCreateElement( "templates" )->SetText( m_pTemplatesText->text().toUtf8().data() );
		//findOrCreateElement( "genericdatatypes" )->SetText( m_pGenericDataTypesText->text().toUtf8().data() );

		return m_document.SaveFile( getFileName().getCompletePath() ) == tinyxml2::XML_SUCCESS;
	}

	void PackageFile::doUi()
	{

	}

	tinyxml2::XMLElement* PackageFile::findOrCreateElement( const char* pName )
	{
		tinyxml2::XMLElement* pElement = m_document.RootElement()->FirstChildElement( pName );
		if( pElement == nullptr )
		{
			pElement = (tinyxml2::XMLElement*)m_document.RootElement()->InsertEndChild( m_document.NewElement( pName ) );
		}

		return pElement;
	}
}
