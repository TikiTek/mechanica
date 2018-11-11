#pragma once

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/platform_data_win.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include "../../../source/posix/platform_data_posix.hpp"
#else
#	error not supported
#endif

namespace tiki
{
	class DirectoryIterator
	{
		TIKI_NONCOPYABLE_CLASS( DirectoryIterator );

	public:

						DirectoryIterator();
						~DirectoryIterator();

		bool			create( const Path& path );
		bool			create( const char* pPath );
		void			dispose();

		bool			findNextFile();

		bool			isCurrentFile() const;
		bool			isCurrentDirectory() const;
		const char*		getCurrentFileName() const;

	private:

		DirectoryIteratorPlatformData	m_platformData;
	};
}
