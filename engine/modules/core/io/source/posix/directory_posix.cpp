#include "tiki/io/directory.hpp"

#include "tiki/base/functions.hpp"

#include <sys/stat.h>
#include <unistd.h>

namespace tiki
{
	bool directory::create( const char* pPath )
	{
		return mkdir( pPath, 0700 ) == 0;
	}

	bool directory::remove( const char* pPath )
	{
		return rmdir( pPath ) == 0;
	}

	bool directory::exists( const char* pPath )
	{
		struct stat stats = { 0 };
		if ( stat( pPath, &stats ) == -1 )
		{
			return false;
		}
		
		return isBitSet( stats.st_mode, S_IFDIR );
	}
}