
#include "tiki/base/platform.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/string.hpp"

#include "../platform_internal.hpp"

#include <windows.h>

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
tiki::InstanceHandle				s_instanceHandle;
tiki::StaticArray< const char* >	s_arguments;

#if TIKI_ENABLED( TIKI_SDL )
#	include "SDL_main.h"
#endif

int main( int argc, char** argv )
{
	s_instanceHandle = (tiki::InstanceHandle)GetModuleHandle( nullptr );

	s_arguments.create( (const char**)argv, (tiki::uint)argc );

	const int returnValue = tiki::platform::startApplication();

	s_arguments.dispose();
	return returnValue;
}

#endif

namespace tiki
{
	void platform::getUserName( char* pBuffer, uint bufferSize )
	{
		wchar_t aUserNameWideCharBuffer[ 128u ];
		DWORD length = TIKI_COUNT( aUserNameWideCharBuffer );
		GetUserNameW( aUserNameWideCharBuffer, &length );

		convertWidecharToUtf8String( pBuffer, bufferSize, aUserNameWideCharBuffer );
	}

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
	InstanceHandle platform::getInstanceHandle()
	{
		return s_instanceHandle;
	}

	const StaticArray< const char* >& platform::getArguments()
	{
		return s_arguments;
	}

	bool platform::hasArgument( const char* name )
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
