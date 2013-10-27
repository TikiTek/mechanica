
#include "tiki/base/assert.hpp"

#include "tiki/toolbase/filestream.hpp"

#include <stdio.h>

namespace tiki
{
	FileStream::FileStream()
	{
		m_pHandle = nullptr;
	}

	FileStream::~FileStream()
	{
		TIKI_ASSERT( m_pHandle == nullptr );
	}

	void FileStream::create( const string& fileName, FileOpenMode fileMode )
	{
		cstring pMode = "wb+";

		switch ( fileMode )
		{
		case FOM_Read:
			pMode = "rb";
			break;
		case FOM_Write:
			pMode = "wb";
			break;
		case FOM_WriteAppend:
			pMode = "ab";
			break;
		}

		m_pHandle = fopen( fileName.cStr(), pMode );
	}

	void FileStream::dispose()
	{
		if (m_pHandle)
		{
			fclose( m_pHandle );
			m_pHandle	= nullptr;
		}
	}

	bool FileStream::isOpen() const
	{
		return m_pHandle != nullptr;
	}

	size_t FileStream::read( void* pData, size_t length ) const
	{
		TIKI_ASSERT( m_pHandle );
		return fread( pData, length, 1u, m_pHandle );
	}

	void FileStream::write( const void* pData, size_t length ) const
	{
		TIKI_ASSERT( m_pHandle );
		fwrite( pData, length, 1u, m_pHandle );
	}

	size_t FileStream::getLength()
	{
		size_t pos = getPosition();

		fseek( m_pHandle, 0, SEEK_END );
		size_t size = (size_t)ftell( m_pHandle );

		setPosition( pos );

		return size;
	}

	size_t FileStream::getPosition() const
	{
		fpos_t pos = 0;
		fgetpos( m_pHandle, &pos );
		return (size_t)pos;

	}

	size_t FileStream::setPosition( size_t pos )
	{
		fpos_t pos2 = (fpos_t)pos;
		fsetpos( m_pHandle, &pos2 );
		return (size_t)pos2;
	}

}