#include "tiki/io/directory_iterator.hpp"

#include "tiki/base/string.hpp"
#include "tiki/io/path.hpp"

namespace tiki
{
	DirectoryIterator::DirectoryIterator()
	{
	}

	DirectoryIterator::~DirectoryIterator()
	{
	}

	bool DirectoryIterator::create( const Path& path )
	{
		return create( path.getCompletePath() );
	}

	bool DirectoryIterator::create( const char* pPath )
	{
		string finalPath = pPath;
		if ( !finalPath.endsWith( '/' ) )
		{
			finalPath += "/";
		}
		finalPath += "*.*";

		if ( !convertToPlatformPath( m_platformData.aPathBuffer, TIKI_COUNT( m_platformData.aPathBuffer ), finalPath.cStr() ) )
		{
			return false;
		}

		return true;
	}

	void DirectoryIterator::dispose()
	{
		FindClose( m_platformData.searchHandle );
		m_platformData.searchHandle = INVALID_HANDLE_VALUE;
	}

	bool DirectoryIterator::findNextFile()
	{
		bool result = false;
		if ( m_platformData.searchHandle == INVALID_HANDLE_VALUE )
		{
			m_platformData.searchHandle = FindFirstFileW( m_platformData.aPathBuffer, &m_platformData.searchData );
			result = m_platformData.searchHandle != INVALID_HANDLE_VALUE;
		}
		else
		{
			result = (FindNextFileW( m_platformData.searchHandle, &m_platformData.searchData ) != 0);

			if ( !result )
			{
				dispose();
			}
		}

		if ( result )
		{
			const uint length = wcslen( m_platformData.searchData.cFileName );
			if( (length == 1 && m_platformData.searchData.cFileName[ 0u ] == L'.') ||
				(length == 2 && m_platformData.searchData.cFileName[ 0u ] == L'.' && m_platformData.searchData.cFileName[ 1u ] == L'.') )
			{
				return findNextFile();
			}
			else
			{
				convertWidecharToUtf8String( m_platformData.aCurrentFileBuffer, TIKI_COUNT( m_platformData.aCurrentFileBuffer ), m_platformData.searchData.cFileName );
			}
		}

		return result;
	}

	bool DirectoryIterator::isCurrentFile() const
	{
		return !isBitSet( m_platformData.searchData.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY );
	}

	bool DirectoryIterator::isCurrentDirectory() const
	{
		return isBitSet( m_platformData.searchData.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY );
	}

	const char* DirectoryIterator::getCurrentFileName() const
	{
		return m_platformData.aCurrentFileBuffer;
	}
}
