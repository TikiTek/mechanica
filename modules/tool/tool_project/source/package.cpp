#include "tiki/tool_project/package.hpp"

#include "tiki/base/path.hpp"
#include "tiki/io/file.hpp"
#include "tiki/tool_base/directory_tool.hpp"
#include "tiki/tool_project/project.hpp"
#include "tiki/tool_xml/xml_document.hpp"
#include "tiki/tool_xml/xml_element.hpp"

namespace tiki
{
	void Package::addDependency( Package* pPackage )
	{
		m_dependencies.add( pPackage );
	}

	void Package::removeDependency( Package* pPackage )
	{
		m_dependencies.removeSortedByValue( pPackage );
	}

	void Package::setGenericTypesString( const DynamicString& value )
	{
		m_genericTypesString = value;
		m_genericTypesPath.setCombinedPath( m_genericDataPath, m_genericTypesString.cStr() );
	}

	void Package::setGenericDataString( const DynamicString& value )
	{
		m_genericDataString = value;
		m_genericDataPath.setCombinedPath( m_basePath, m_genericDataString.cStr() );
		m_genericTypesPath.setCombinedPath( m_genericDataPath, m_genericTypesString.cStr() );
	}

	void Package::setAssetTemplatesString( const DynamicString& value )
	{
		m_assetTemplatesString = value;
		m_assetTemplatesPath.setCombinedPath( m_basePath, m_assetTemplatesString.cStr() );
	}

	void Package::findGenericDataTypeFiles( List< Path >& targetList ) const
	{
		directory::findFiles( targetList, m_genericTypesPath, ".generic_types" );
	}

	void Package::findAssetTemplateFiles( List< Path >& targetList ) const
	{
		directory::findFiles( targetList, m_assetTemplatesPath, ".template" );
	}

	void Package::create( const Path& filePath )
	{
		m_name		= filePath.getBasename();
		m_filename	= filePath;
		m_basePath	= filePath;
		m_basePath.setExtension( "" );
	}

	bool Package::load( Project& project )
	{
		bool hasGenericDataString = false;
		bool hasGenericTypesString = false;
		bool hasAssetTemplatesString = false;
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

			const XmlElement* pDependenciesNode = pRootNode->findFirstChild( "dependencies" );
			for( const XmlElement* pDependencyNode = pDependenciesNode->findFirstChild( "package" ); pDependencyNode != nullptr; pDependencyNode = pDependencyNode->findNextSiblingWithSameName() )
			{
				Package* pPackage = project.findPackage( pDependencyNode->getValue() );
				if( pPackage == nullptr )
				{
					TIKI_TRACE_ERROR( "[package] Could not find Package dependency with name '%s' to load Package '%s'.\n", pDependencyNode->getValue(), m_name.cStr() );
					continue;
				}

				m_dependencies.pushBack( pPackage );
			}

			const XmlElement* pGenericDataNode = pRootNode->findFirstChild( "generic_data" );
			if( pGenericDataNode != nullptr )
			{
				const XmlElement* pGenericDataBaseNode = pGenericDataNode->findFirstChild( "base_path" );
				if( pGenericDataBaseNode != nullptr )
				{
					setGenericDataString( pGenericDataBaseNode->getValue() );
					hasGenericDataString = true;

					const XmlElement* pGenericDataTypesNode = pGenericDataNode->findFirstChild( "types_path" );
					if( pGenericDataTypesNode != nullptr )
					{
						setGenericTypesString( pGenericDataTypesNode->getValue() );
						hasGenericTypesString = true;
					}
				}
			}

			const XmlElement* pAssetTemplatesNode = pRootNode->findFirstChild( "asset_templates" );
			if( pAssetTemplatesNode != nullptr )
			{
				setAssetTemplatesString( pAssetTemplatesNode->getValue() );
				hasAssetTemplatesString = true;
			}
		}

		if( !hasGenericDataString )
		{
			setGenericDataString( "generic_data" );
		}

		if( !hasGenericTypesString )
		{
			setGenericTypesString( "types" );
		}

		if( !hasAssetTemplatesString )
		{
			setAssetTemplatesString( "templates" );
		}

		return true;
	}

	bool Package::writeToFile() const
	{
		XmlDocument document;

		XmlElement* pRootNode = document.createChild( "package" );

		pRootNode->createChild( "description" )->setValue( m_description.cStr() );
		pRootNode->createChild( "author" )->setValue( m_author.cStr() );

		XmlElement* pDependenciesNode = pRootNode->createChild( "dependencies" );
		for( const Package* pPackge : m_dependencies )
		{
			pDependenciesNode->createChild( "package" )->setValue( pPackge->getName().cStr() );
		}

		XmlElement* pGenericDataNode = pRootNode->createChild( "generic_data" );
		pGenericDataNode->createChild( "base_path" )->setValue( m_genericDataString.cStr() );
		pGenericDataNode->createChild( "types_path" )->setValue( m_genericTypesString.cStr() );

		pRootNode->createChild( "asset_templates" )->setValue( m_assetTemplatesString.cStr() );

		if( !document.saveToFile( m_filename.getCompletePath() ) )
		{
			TIKI_TRACE_ERROR( "[project] Failed to write package to '%s'.\n", m_filename.getCompletePath() );
			return false;
		}

		return true;
	}
}