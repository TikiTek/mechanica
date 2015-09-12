#pragma once
#ifndef __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__
#define __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__

namespace tiki
{
	struct FileStreamPlatformData
	{
		HANDLE	fileHandle;
	};

	struct FileWatcherPlatformData
	{
		string		basePath;

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
}

#endif // __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__
