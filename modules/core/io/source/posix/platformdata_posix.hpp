#pragma once
#ifndef __TIKI_PLATFORMDATA_POSIX_HPP_INCLUDED__
#define __TIKI_PLATFORMDATA_POSIX_HPP_INCLUDED__

struct __dirstream;
struct dirent;
struct _IO_FILE;

namespace tiki
{
	struct FileStreamPlatformData
	{
		FileStreamPlatformData()
		{
			pFileHandle = nullptr;
		}
		
		_IO_FILE*	pFileHandle;
	};

	struct FileWatcherPlatformData
	{		
	};
	
	struct DirectoryIteratorPlatformData
	{
		DirectoryIteratorPlatformData()
		{
			pDirHandle	= nullptr;
			pDirEntry	= nullptr;
		}
		
		__dirstream*	pDirHandle;
		dirent*		pDirEntry;
	};
}

#endif // __TIKI_PLATFORMDATA_POSIX_HPP_INCLUDED__
