#include "tiki/package_editor/package_file.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	PackageFile::PackageFile( const Path& filename, PackageEditor* pEditor )
		: EditableFile( filename, pEditor )
	{
		m_author[ 0u ] = '\0';
		m_desciption[ 0u ] = '\0';
		m_templates[ 0u ] = '\0';
		m_genericDataTypes[ 0u ] = '\0';
		TIKI_VERIFY( load() );
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
			copyString( m_author, sizeof( m_author ), pAuthorElement->GetText() );
		}

		tinyxml2::XMLElement* pDescriptionElement = pRoot->FirstChildElement( "description" );
		if( pDescriptionElement != nullptr )
		{
			copyString( m_desciption, sizeof( m_desciption ), pDescriptionElement->GetText() );
		}

		tinyxml2::XMLElement* pTemplatesElement = pRoot->FirstChildElement( "templates" );
		if( pTemplatesElement != nullptr )
		{
			copyString( m_templates, sizeof( m_templates ), pTemplatesElement->GetText() );
		}

		tinyxml2::XMLElement* pGenericDataTypesElement = pRoot->FirstChildElement( "genericdatatypes" );
		if( pGenericDataTypesElement != nullptr )
		{
			copyString( m_genericDataTypes, sizeof( m_genericDataTypes ), pGenericDataTypesElement->GetText() );
		}

		return true;
	}

	bool PackageFile::save()
	{
		tinyxml2::XMLElement* pRoot = m_document.RootElement();
		TIKI_ASSERT( pRoot != nullptr );

		pRoot->SetName( "package" );

		findOrCreateElement( "author" )->SetText( m_author );
		findOrCreateElement( "description" )->SetText( m_desciption );
		findOrCreateElement( "templates" )->SetText( m_templates );
		findOrCreateElement( "genericdatatypes" )->SetText( m_genericDataTypes );

		return m_document.SaveFile( getFileName().getCompletePath() ) == tinyxml2::XML_SUCCESS;
	}

	void PackageFile::doUi()
	{
		ImGui::Text( "Author:" );
		m_isDirty |= ImGui::InputText( " ", m_author, sizeof( m_author ) );

		ImGui::Text( "Description:" );
		m_isDirty |= ImGui::InputText( "  ", m_desciption, sizeof( m_desciption ) );

		ImGui::Text( "Templates folder:" );
		m_isDirty |= ImGui::InputText( "   ", m_templates, sizeof( m_templates ) );

		ImGui::Text( "Generic data types folder:" );
		m_isDirty |= ImGui::InputText( "    ", m_genericDataTypes, sizeof( m_genericDataTypes ) );
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
