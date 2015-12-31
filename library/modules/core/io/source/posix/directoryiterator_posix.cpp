
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

		return m_platformData.pDirHandle != nullptr;
	}

	void DirectoryIterator::dispose()
	{
		closedir( m_platformData.pDirHandle );
		m_platformData.pDirHandle = nullptr;
	}

	bool DirectoryIterator::findNextFile()
	{
		m_platformData.pDirEntry = readdir( m_platformData.pDirHandle );
		return m_platformData.pDirEntry != nullptr;
	}

	const char* DirectoryIterator::getCurrentFileName() const
	{
		return m_platformData.pDirEntry->d_name;
	}
}
