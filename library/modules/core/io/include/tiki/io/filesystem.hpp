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
		DataAccessMode_ReadWrite
	};

	class FileSystem
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( FileSystem );

	public:

		virtual bool			exists( cstring pFileName ) const = 0;
		virtual DataStream*		open( cstring pFileName, DataAccessMode accessMode ) = 0;

	};
}

#endif // __TIKI_FILESYSTEM_HPP_INCLUDED__
