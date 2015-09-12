
#include "tiki/io/directoryiterator.hpp"

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
		convertUtf8ToWidecharString( m_platformData.aPathBuffer, TIKI_COUNT( m_platformData.aPathBuffer ), pPath );

		return true;
	}

	void DirectoryIterator::dispose()
	{
		CloseHandle( m_platformData.searchHandle );
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
			result = FindNextFileW( m_platformData.searchHandle, &m_platformData.searchData );

			if ( !result )
			{
				dispose();
			}
		}

		if ( result )
		{
			convertWidecharToUtf8String( m_platformData.aCurrentFileBuffer, TIKI_COUNT( m_platformData.aCurrentFileBuffer ), m_platformData.searchData.cFileName );
		}

		return result;
	}

	const char* DirectoryIterator::getCurrentFileName() const
	{
		return m_platformData.aCurrentFileBuffer;
	}
}
