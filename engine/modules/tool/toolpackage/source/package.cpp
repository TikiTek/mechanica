#include "tiki/toolpackage/package.hpp"

#include "tiki/base/path.hpp"
#include "tiki/toolbase/directory_tool.hpp"

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
		genericDataPath.setCombinedPath( m_basepath.cStr(), m_genericDataTypesPath.cStr() );

		directory::findFiles( targetList, genericDataPath, ".generictypes" );
	}

	void Package::findAssetTemplateFiles( List< Path >& targetList ) const
	{
		Path templatePath;
		templatePath.setCombinedPath( m_basepath.cStr(), m_assetTemplatesPath.cStr() );

		directory::findFiles( targetList, templatePath, ".generictypes" );
	}

	bool Package::create( const Path& filePath )
	{
		m_filename = filePath;
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