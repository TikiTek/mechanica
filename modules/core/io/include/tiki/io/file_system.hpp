#pragma once
#ifndef __TIKI_FILESYSTEM_HPP_INCLUDED__
#define __TIKI_FILESYSTEM_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	class DataStream;

	enum DataAccessMode
	{
		DataAccessMode_Read,
		DataAccessMode_Write,
		DataAccessMode_WriteAppend,
		DataAccessMode_ReadWrite,

		DataAccessMode_Count
	};

	class FileSystem
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( FileSystem );

	public:

		virtual const char*		getFilenameByCrc( crc32 filenameCrc ) const TIKI_PURE;

		virtual bool			exists( const char* pFileName ) const TIKI_PURE;
		virtual DataStream*		open( const char* pFileName, DataAccessMode accessMode ) TIKI_PURE;

	};
}

#endif // __TIKI_FILESYSTEM_HPP_INCLUDED__
