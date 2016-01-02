#include "tiki/base/platform.hpp"

#include "tiki/base/string.hpp"
#include "tiki/container/staticarray.hpp"

#include "../platform_internal.hpp"

#include <unistd.h>

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )

tiki::StaticArray< const char* >	s_arguments;

int main( int argc, const char** argv )
{
	s_arguments.create( argv, (tiki::uint)argc );
	
	const int result = tiki::platform::startApplication();
	
	s_arguments.dispose();
	
	return result;
}

#endif

namespace tiki
{
	void platform::getUserName( char* pBuffer, uint bufferSize )
	{
		getlogin_r( pBuffer, bufferSize );
	}
	
#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
	InstanceHandle platform::getInstanceHandle()
	{
		return nullptr;
	}
	
	const StaticArray< cstring >& platform::getArguments()
	{
		return s_arguments;
	}
	
	bool platform::hasArgument( cstring name )
	{
		for (uint i = 0u; i < s_arguments.getCount(); ++i)
		{
			if ( isStringEquals( s_arguments[ i ], name ) )
			{
				return true;
			}
		}

		return false;
	}
#endif

	uint platform::getProcessorCount()
	{
		return sysconf( _SC_NPROCESSORS_ONLN );
	}
}
