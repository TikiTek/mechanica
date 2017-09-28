#include "tiki/toolpackage/package.hpp"

#include "tiki/base/path.hpp"
#include "tiki/io/file.hpp"
#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolxml/xml_document.hpp"

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

		directory::findFiles( targetList, templatePath, ".generictypes" );
	}

	bool Package::create( const Path& filePath )
	{
		m_filename = filePath;

		if( file::exists( m_filename.getCompletePath() ) )
		{
			XmlDocument document;
			if( !document.loadFromFile( m_filename.getCompletePath() ) )
			{
				return false;
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

	bool Package::writeToFile()
	{
		return false;
	}
}