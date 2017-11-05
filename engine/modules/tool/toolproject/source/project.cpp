#include "tiki/toolproject/project.hpp"

#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolproject/package.hpp"

namespace tiki
{
	Project::Project()
	{
		Path path = platform::getExecutablePath();

		m_packagePool.create( 4u );
		loadFromFile();
	}

	Project::Project( const Path& projectPath )
	{
		m_projectPath = projectPath;
		m_contentPath = projectPath;
		m_contentPath.push( "content" );
		m_gamebuildPath = projectPath;
		m_gamebuildPath.push( "assetbuild" );

		m_packagePool.create( 4u );
		loadFromFile();
	}

	Project::~Project()
	{
		while( !m_packages.isEmpty() )
		{
			Package& package = m_packages.getFirst();
			m_packages.removeSortedByValue( package );
			m_packagePool.removeUnsortedByValue( package );
		}

		m_packagePool.dispose();
	}

	Package* Project::addPackage( const string& packageName )
	{
		Package* pPackage = m_packagePool.push();
		pPackage->create( m_contentPath, packageName );
		return pPackage;
	}

	Package* Project::findPackage( const string& packageName )
	{
		for( Package& package : m_packages )
		{
			if( package.getName() == packageName )
			{
				return &package;
			}
		}

		return nullptr;
	}

	const Package* Project::findPackage( const string& packageName ) const
	{
		return const_cast< Project* >( this )->findPackage( packageName );
	}

	const LinkedList< Package >& Project::getPackages() const
	{
		return m_packages;
	}

	bool Project::writeToFile()
	{
		for( Package& package : m_packages )
		{
			if( !package.writeToFile() )
			{
				return false;
			}
		}

		return true;
	}

	void Project::loadFromFile()
	{
		List< Path > packageFiles;
		directory::findFiles( packageFiles, m_contentPath, ".package", false );

		for( const Path& packageFilePath : packageFiles )
		{
			Package* pPackage = m_packagePool.push();
			if( !pPackage->create( packageFilePath ) )
			{
				m_packagePool.removeUnsortedByValue( *pPackage );
				continue;
			}

			m_packages.push( pPackage );
		}
	}
}