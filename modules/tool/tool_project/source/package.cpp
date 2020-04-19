#include "tiki/tool_project/package.hpp"

#include "tiki/base/path.hpp"
#include "tiki/io/file.hpp"
#include "tiki/tool_base/directory_tool.hpp"
#include "tiki/tool_xml/xml_document.hpp"
#include "tiki/tool_xml/xml_element.hpp"

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
		directory::findFiles( targetList, m_genericTypesPath, ".generic_types" );
	}

	void Package::findAssetTemplateFiles( List< Path >& targetList ) const
	{
		directory::findFiles( targetList, m_assetTemplatesPath, ".template" );
	}

	bool Package::create( const Path& filePath )
	{
		m_name		= filePath.getBasename();
		m_filename	= filePath;
		m_basePath	= filePath;
		m_basePath.setExtension( "" );

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

			const XmlElement* pGenericDataNode = pRootNode->findFirstChild( "generic_data" );
			if( pGenericDataNode != nullptr )
			{
				const XmlElement* pGenericDataBaseNode = pGenericDataNode->findFirstChild( "base_path" );
				if( pGenericDataBaseNode != nullptr )
				{
					m_genericDataPath.setCombinedPath( m_basePath, pGenericDataBaseNode->getValue() );

					const XmlElement* pGenericDataTypesNode = pGenericDataNode->findFirstChild( "types_path" );
					if( pGenericDataTypesNode != nullptr )
					{
						m_genericTypesPath.setCombinedPath( m_genericDataPath, pGenericDataTypesNode->getValue() );
					}
					else
					{
						//TIKI_TRACE_WARNING( "[package] Package has no 'generic_data/types_path' node in %s.\n", m_filename.getCompletePath() );
						m_genericTypesPath.setCombinedPath( m_genericDataPath, "types" );
					}
				}
				else
				{
					//TIKI_TRACE_WARNING( "[package] Package has no 'generic_data/base_path' node in %s.\n", m_filename.getCompletePath() );
					m_genericDataPath.setCombinedPath( m_genericDataPath, "generic_data" );
					m_genericTypesPath.setCombinedPath( m_basePath, "types" );
				}
			}
			else
			{
				//TIKI_TRACE_WARNING( "[package] Package has no 'generic_data' node in %s.\n", m_filename.getCompletePath() );
				m_genericDataPath.setCombinedPath( m_basePath, "generic_data" );
				m_genericTypesPath.setCombinedPath( m_genericDataPath, "types" );
			}

			const XmlElement* pAssetTemplatesNode = pRootNode->findFirstChild( "asset_templates" );
			if( pAssetTemplatesNode != nullptr )
			{
				m_assetTemplatesPath.setCombinedPath( m_basePath, pAssetTemplatesNode->getValue() );
			}
			else
			{
				//TIKI_TRACE_WARNING( "[package] Package has no 'asset_templates' node in %s.\n", m_filename.getCompletePath() );
				m_assetTemplatesPath.setCombinedPath( m_basePath, "templates" );
			}
		}

		return true;
	}

	void Package::create( const Path& contentPath, const DynamicString& packageName )
	{
		m_name = packageName;

		m_filename = contentPath;
		m_filename.setBasename( packageName.cStr() );
		m_filename.setExtension( ".package" );

		m_basePath = contentPath;
		m_basePath.push( packageName.cStr() );
	}

	bool Package::writeToFile() const
	{
		return false;
	}
}