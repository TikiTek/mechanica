
#include "tiki/base/platform.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/string.hpp"

#include "../platform_internal.hpp"

#include <windows.h>

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
tiki::InstanceHandle				s_instanceHandle;
tiki::StaticArray< tiki::cstring >	s_arguments;

int main( int argc, char** argv )
{
	s_instanceHandle	= (tiki::InstanceHandle)GetModuleHandle( nullptr );

	s_arguments.create( (tiki::cstring*)argv, (tiki::uint)argc );

	const int returnValue = tiki::platform::startApplication();

	s_arguments.dispose();
	return returnValue;
}
#endif

namespace tiki
{
#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
	InstanceHandle platform::getInstanceHandle()
	{
		return s_instanceHandle;
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
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );

		return sysinfo.dwNumberOfProcessors;
	}
}
