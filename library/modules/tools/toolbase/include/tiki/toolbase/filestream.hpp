#pragma once
#ifndef TIKI_FILESTREAM_HPP
#define TIKI_FILESTREAM_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/string.hpp"

struct _iobuf;

namespace tiki
{
	enum FileOpenMode
	{
		FOM_Read,
		FOM_ReadWrite,
		FOM_Write,
		FOM_WriteAppend
	};

	class FileStream
	{
	public:

		FileStream();
		~FileStream();

		void		create( const string& fileName, FileOpenMode fileMode = FOM_ReadWrite );
		void		dispose();

		bool		isOpen() const;

		size_t		read( void* pData, size_t length ) const;
		void		write( const void* pData, size_t length ) const;

		size_t		getLength();

		size_t		getPosition() const;
		size_t		setPosition( size_t pos );

	private:

		_iobuf*		m_pHandle;

	};
}

#endif // TIKI_FILESTREAM_HPP
