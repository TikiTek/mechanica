#include "tiki/toolproject/package.hpp"

#include "tiki/base/path.hpp"
#include "tiki/io/file.hpp"
#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolxml/xml_document.hpp"
#include "tiki/toolxml/xml_element.hpp"

namespace tiki
{
	void Package::addDependencies( Package* pPackage )
	{
		m_dependencies.add( pPackage );
	}

	void Package::removeDependencies( Package* pPackage )
	{
		m_dependencies.removeSortedByValue( pPackage );
	}

	void Package::findGenericDataTypeFiles( List< Path >& targetList ) const
	{
		Path genericDataPath;
		genericDataPath.setCombinedPath( m_basepath, m_genericDataTypesPath );

		directory::findFiles( targetList, genericDataPath, ".generictypes" );
	}

	void Package::findAssetTemplateFiles( List< Path >& targetList ) const
	{
		Path templatePath;
		templatePath.setCombinedPath( m_basepath, m_assetTemplatesPath );

		directory::findFiles( targetList, templatePath, ".template" );
	}

	bool Package::create( const Path& filePath )
	{
		m_name		= filePath.getFilename();
		m_filename	= filePath;
		m_basepath	= filePath;
		m_basepath.setExtension( "" );

		if( file::exists( m_filename.getCompletePath() ) )
		{
			XmlDocument document;
			if( !document.loadFromFile( m_filename.getCompletePath() ) )
			{
				return false;
			}

			const XmlElement* pRootNode = document.findFirstChild( "package" );
			if( pRootNode == nullptr )
			{
				TIKI_TRACE_ERROR( "[package] Failed to find 'package' node in %s.\n", m_filename.getCompletePath() );
				return false;
			}

			const XmlElement* pDescriptionNode = pRootNode->findFirstChild( "description" );
			if( pDescriptionNode != nullptr )
			{
				m_description = pDescriptionNode->getValue();
			}
			else
			{
				TIKI_TRACE_WARNING( "[package] Package has no 'description' node in %s.\n", m_filename.getCompletePath() );
			}

			const XmlElement* pAuthorNode = pRootNode->findFirstChild( "author" );
			if( pAuthorNode != nullptr )
			{
				m_author = pAuthorNode->getValue();
			}
			else
			{
				TIKI_TRACE_WARNING( "[package] Package has no 'author' node in %s.\n", m_filename.getCompletePath() );
			}

			const XmlElement* pGenericDataTypesNode = pRootNode->findFirstChild( "genericdatatypes" );
			if( pGenericDataTypesNode != nullptr )
			{
				m_genericDataTypesPath.setCompletePath( pGenericDataTypesNode->getValue() );
			}
			else
			{
				TIKI_TRACE_WARNING( "[package] Package has no 'genericdatatypes' node in %s.\n", m_filename.getCompletePath() );
			}

			const XmlElement* pAssetTemplatesNode = pRootNode->findFirstChild( "assettemplates" );
			if( pAssetTemplatesNode != nullptr )
			{
				m_assetTemplatesPath.setCompletePath( pAssetTemplatesNode->getValue() );
			}
			else
			{
				TIKI_TRACE_WARNING( "[package] Package has no 'assettemplates' node in %s.\n", m_filename.getCompletePath() );
			}
		}

		return true;
	}

	void Package::create( const Path& contentPath, const string& packageName )
	{
		m_name = packageName;

		m_filename = contentPath;
		m_filename.setFilename( packageName.cStr() );
		m_filename.setExtension( ".package" );

		m_basepath = contentPath;
		m_basepath.push( packageName.cStr() );
	}

	bool Package::writeToFile() const
	{
		return false;
	}
}