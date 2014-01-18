#pragma once
#ifndef __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__
#define __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__

namespace tiki
{
	struct FileStreamPlatformData
	{
		HANDLE	fileHandle;
	};

	struct FileSystemWatcherPlatformData
	{
		HANDLE	watchHandle;
	};
}

#endif // __TIKI_PLATFORMDATA_WIN_HPP_INCLUDED__
