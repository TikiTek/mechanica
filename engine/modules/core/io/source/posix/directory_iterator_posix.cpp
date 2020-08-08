
#include "tiki/io/directoryiterator.hpp"

#include "tiki/base/string.hpp"
#include "tiki/io/path.hpp"

#include <dirent.h>
#include <stdio.h>

namespace tiki
{
	DirectoryIterator::DirectoryIterator()
	{
	}

	DirectoryIterator::~DirectoryIterator()
	{
	}

	bool DirectoryIterator::create( const char* pPath )
	{
		m_platformData.pDirHandle = opendir( pPath );
		copyString( m_platformData.pathName, TIKI_COUNT( m_platformData.pathName ), pPath );

		return m_platformData.pDirHandle != nullptr;
	}

	void DirectoryIterator::dispose()
	{
		closedir( m_platformData.pDirHandle );
		m_platformData.pDirHandle = nullptr;
	}

	bool DirectoryIterator::findNextFile()
	{
		TIKI_ASSERT( m_platformData.pDirHandle != nullptr );
		
		m_platformData.pDirEntry = readdir( m_platformData.pDirHandle );
		while ( m_platformData.pDirEntry != nullptr )
		{
			if ( !isStringEquals( m_platformData.pDirEntry->d_name, "." ) &&
				 !isStringEquals( m_platformData.pDirEntry->d_name, ".." ) )
			{
				break;
			}
			
			m_platformData.pDirEntry = readdir( m_platformData.pDirHandle );
		}
		
		if( m_platformData.pDirEntry != nullptr )
		{
			formatStringBuffer(
				m_platformData.currentName,
				TIKI_COUNT( m_platformData.currentName ),
				"%s/%s",
				m_platformData.pathName,
				m_platformData.pDirEntry->d_name
			);
			
			return true;
		}
		
		return false;
	}

	bool DirectoryIterator::isCurrentFile() const
	{
		TIKI_ASSERT( m_platformData.pDirEntry != nullptr );
		return m_platformData.pDirEntry->d_type == DT_REG;
	}
	
	bool DirectoryIterator::isCurrentDirectory() const
	{
		TIKI_ASSERT( m_platformData.pDirEntry != nullptr );
		return m_platformData.pDirEntry->d_type == DT_DIR;
	}

	const char* DirectoryIterator::getCurrentFileName() const
	{
		TIKI_ASSERT( m_platformData.pDirEntry != nullptr );
		return m_platformData.pDirEntry->d_name;
	}
}
