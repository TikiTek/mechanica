
#include "tiki/base/assert.hpp"

#include "tiki/toolbase/toolfilestream.hpp"

#pragma warning(disable: 4996)
#include <stdio.h>

namespace tiki
{
	ToolFileStream::ToolFileStream()
	{
		m_pHandle = nullptr;
	}

	ToolFileStream::~ToolFileStream()
	{
		TIKI_ASSERT( m_pHandle == nullptr );
	}

	void ToolFileStream::create( const string& fileName, FileOpenMode fileMode )
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

	void ToolFileStream::dispose()
	{
		if (m_pHandle)
		{
			fclose( m_pHandle );
			m_pHandle	= nullptr;
		}
	}

	bool ToolFileStream::isOpen() const
	{
		return m_pHandle != nullptr;
	}

	size_t ToolFileStream::read( void* pData, size_t length ) const
	{
		TIKI_ASSERT( m_pHandle );
		return fread( pData, length, 1u, m_pHandle );
	}

	void ToolFileStream::write( const void* pData, size_t length ) const
	{
		TIKI_ASSERT( m_pHandle );
		fwrite( pData, length, 1u, m_pHandle );
	}

	size_t ToolFileStream::getLength()
	{
		size_t pos = getPosition();

		fseek( m_pHandle, 0, SEEK_END );
		size_t size = (size_t)ftell( m_pHandle );

		setPosition( pos );

		return size;
	}

	size_t ToolFileStream::getPosition() const
	{
		fpos_t pos = 0;
		fgetpos( m_pHandle, &pos );
		return (size_t)pos;

	}

	size_t ToolFileStream::setPosition( size_t pos )
	{
		fpos_t pos2 = (fpos_t)pos;
		fsetpos( m_pHandle, &pos2 );
		return (size_t)pos2;
	}

}