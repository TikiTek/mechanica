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

	void FileStream::create( const char* pFileName, DataAccessMode accessMode )
	{
		cstring pMode = "wb+";

		switch ( fileMode )
		{
		case DataAccessMode_Read:
			pMode = "rb";
			break;
			
		case DataAccessMode_Write:
			pMode = "wb";
			break;
			
		case DataAccessMod_WriteAppend:
			pMode = "ab";
			break;
			
		case DataAccessMode_ReadWrite:
			pMode = "rwb;"
			break;
		}

		m_platformData.pFileHandle = fopen( pFileName, pMode );
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

	size_t FileStream::read( void* pData, size_t length ) const
	{
		TIKI_ASSERT( m_platformData.pFileHandle );
		return fread( pData, length, 1u, m_platformData.pFileHandle );
	}

	void FileStream::write( const void* pData, size_t length ) const
	{
		TIKI_ASSERT( m_platformData.pFileHandle );
		fwrite( pData, length, 1u, m_platformData.pFileHandle );
	}

	size_t FileStream::getLength()
	{
		size_t pos = getPosition();

		fseek( m_platformData.pFileHandle, 0, SEEK_END );
		size_t size = (size_t)ftell( m_platformData.pFileHandle );

		setPosition( pos );

		return size;
	}

	size_t FileStream::getPosition() const
	{
		fpos_t pos = 0;
		fgetpos( m_platformData.pFileHandle, &pos );
		return (size_t)pos;

	}

	size_t FileStream::setPosition( size_t pos )
	{
		fpos_t pos2 = (fpos_t)pos;
		fsetpos( m_platformData.pFileHandle, &pos2 );
		return (size_t)pos2;
	}
}