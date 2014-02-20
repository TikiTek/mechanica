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
}

#endif // __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__
