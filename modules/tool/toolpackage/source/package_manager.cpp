#include "tiki/toolpackage/package_manager.hpp"

#include "tiki/toolbase/directory_tool.hpp"
#include "tiki/toolpackage/package.hpp"

namespace tiki
{
	void PackageManager::create( const Path& contentPath )
	{
		m_contentPath = contentPath;

		m_packagePool.create( 4u );

		List< Path > packageFiles;
		directory::findFiles( packageFiles, contentPath, ".package", false );

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

	void PackageManager::dispose()
	{
		while( !m_packages.isEmpty() )
		{
			Package& package = m_packages.getFirst();
			m_packages.removeSortedByValue( package );
			m_packagePool.removeUnsortedByValue( package );
		}

		m_packagePool.dispose();
	}

	bool PackageManager::writeToFile()
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

	Package* PackageManager::addPackage( const string& packageName )
	{
		Package* pPackage = m_packagePool.push();
		pPackage->create( m_contentPath, packageName );
		return pPackage;
	}

	Package* PackageManager::findPackage( const string& packageName )
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

	const Package* PackageManager::findPackage( const string& packageName ) const
	{
		return const_cast< PackageManager* >( this )->findPackage( packageName );
	}

	const LinkedList< Package >& PackageManager::getPackages() const
	{
		return m_packages;
	}
}