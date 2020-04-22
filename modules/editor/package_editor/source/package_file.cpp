#include "tiki/package_editor/package_file.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	PackageFile::PackageFile( Package& package, PackageEditor* pEditor )
		: EditableFile( package.getFilename(), pEditor )
		, m_package( package )
	{
		m_desciption[ 0u ]		= '\0';
		m_author[ 0u ]			= '\0';
		m_genericData[ 0u ]		= '\0';
		m_genericTypes[ 0u ]	= '\0';
		m_assetTemplates[ 0u ]	= '\0';
	}

	PackageFile::~PackageFile()
	{
	}

	bool PackageFile::load()
	{
		copyString( m_desciption, sizeof( m_desciption ), m_package.getDescription().cStr() );
		copyString( m_author, sizeof( m_author ), m_package.getAuthor().cStr() );
		copyString( m_genericData, sizeof( m_genericData ), m_package.getGenericDataString().cStr() );
		copyString( m_genericTypes, sizeof( m_genericTypes ), m_package.getGenericTypesString().cStr() );
		copyString( m_assetTemplates, sizeof( m_assetTemplates ), m_package.getAssetTemplatesString().cStr() );

		return true;
	}

	bool PackageFile::save()
	{
		if( !m_isDirty )
		{
			return true;
		}

		m_package.setDescription( m_desciption );
		m_package.setAuthor( m_author );
		m_package.setGenericDataString( m_genericData );
		m_package.setGenericTypesString( m_genericTypes );
		m_package.setAssetTemplatesString( m_assetTemplates );

		if( !m_package.writeToFile() )
		{
			return false;
		}

		m_isDirty = false;
		return true;
	}

	void PackageFile::doUi()
	{
		ImGui::Text( "Author:" );
		m_isDirty |= ImGui::InputText( " ", m_author, sizeof( m_author ) );

		ImGui::Text( "Description:" );
		m_isDirty |= ImGui::InputText( "  ", m_desciption, sizeof( m_desciption ) );

		ImGui::Text( "Generic Data base Folder:" );
		m_isDirty |= ImGui::InputText( "   ", m_genericData, sizeof( m_genericData ) );

		ImGui::Text( "Generic Types Folder:" );
		m_isDirty |= ImGui::InputText( "    ", m_genericTypes, sizeof( m_genericTypes ) );

		ImGui::Text( "Templates Folder:" );
		m_isDirty |= ImGui::InputText( "     ", m_assetTemplates, sizeof( m_assetTemplates ) );
	}
}
