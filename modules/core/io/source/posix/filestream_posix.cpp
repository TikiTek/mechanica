#include "tiki/io/filestream.hpp"

#include "tiki/base/assert.hpp"

#include <stdio.h>

namespace tiki
{
	FileStream::FileStream()
	{
	}

	FileStream::~FileStream()
	{
	}

	bool FileStream::create( const char* pFileName, DataAccessMode accessMode )
	{
		cstring pMode = "wb+";

		switch ( accessMode )
		{
		case DataAccessMode_Read:
			pMode = "rb";
			break;
			
		case DataAccessMode_Write:
			pMode = "wb";
			break;
			
		case DataAccessMode_WriteAppend:
			pMode = "ab";
			break;
			
		case DataAccessMode_ReadWrite:
			pMode = "rwb";
			break;
		}

		m_platformData.pFileHandle = fopen( pFileName, pMode );
		return m_platformData.pFileHandle != nullptr;
	}

	void FileStream::dispose()
	{
		if( m_platformData.pFileHandle )
		{
			fclose( m_platformData.pFileHandle );
			m_platformData.pFileHandle	= nullptr;
		}
	}

	bool FileStream::isOpen() const
	{
		return m_platformData.pFileHandle != nullptr;
	}

	FileSize FileStream::read( void* pData, FileSize length ) const
	{
		TIKI_ASSERT( m_platformData.pFileHandle );
		return fread( pData, length, 1u, m_platformData.pFileHandle );
	}

	FileSize FileStream::write( const void* pData, FileSize length )
	{
		TIKI_ASSERT( m_platformData.pFileHandle );
		return fwrite( pData, length, 1u, m_platformData.pFileHandle );
	}

	FileSize FileStream::getLength() const
	{
		fpos_t pos;
		fgetpos( m_platformData.pFileHandle, &pos );
		
		fseek( m_platformData.pFileHandle, 0, SEEK_END );
		FileSize size = (FileSize)ftell( m_platformData.pFileHandle );
		
		fsetpos( m_platformData.pFileHandle, &pos );

		return size;
	}

	FileSize FileStream::getPosition() const
	{
		fpos_t pos;
		fgetpos( m_platformData.pFileHandle, &pos );
		return (FileSize)pos.__pos;

	}

	void FileStream::setPosition( FileSize pos )
	{
		fpos_t pos2;
		pos2.__pos = pos;
		fsetpos( m_platformData.pFileHandle, &pos2 );
	}
}