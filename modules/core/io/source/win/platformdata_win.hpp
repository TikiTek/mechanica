#pragma once
#ifndef __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__
#define __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__

#include "tiki/base/inline.hpp"
#include "tiki/base/string.hpp"

#include <windows.h>

namespace tiki
{
	struct FileStreamPlatformData
	{
		HANDLE	fileHandle;
	};

	struct FileWatcherPlatformData
	{
		char		aPathBuffer[ TIKI_MAX_PATH ];

		bool		running;
		HANDLE		dirHandle;
		HANDLE		portHandle;
		OVERLAPPED	overlapped;

		DWORD		requiredSize;
		uint8		dataBuffer[ 2048u ];
	};

	struct DirectoryIteratorPlatformData
	{
		DirectoryIteratorPlatformData()
		{
			aPathBuffer[ 0u	]		= '\0';
			aCurrentFileBuffer[0u ]	= '\0';
			searchHandle			= INVALID_HANDLE_VALUE;
		}

		wchar_t				aPathBuffer[ TIKI_MAX_PATH ];
		char				aCurrentFileBuffer[ TIKI_MAX_PATH ];

		HANDLE				searchHandle;
		WIN32_FIND_DATAW	searchData;
	};

	TIKI_INLINE bool convertToPlatformPath( wchar_t* pTargetBuffer, uint targetLength, const char* pSourceBuffer )
	{
		if ( !convertUtf8ToWidecharString( pTargetBuffer, targetLength, pSourceBuffer ) )
		{
			return false;
		}
		
		uint i = 0u;
		while ( pTargetBuffer[ i ] != L'\0' )
		{
			if ( pTargetBuffer[ i ] == L'/' )
			{
				pTargetBuffer[ i ] = L'\\';
			}

			i++;
		}

		return true;
	}
}

#endif // __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__
