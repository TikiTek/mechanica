
#include "tiki/base/platform.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/string.hpp"

#include "../platform_internal.hpp"

#include <windows.h>

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
tiki::InstanceHandle				s_instanceHandle;
tiki::StaticArray< const char* >	s_arguments;

char**								s_ppArguments;
int									s_argumentCount;

#if TIKI_ENABLED( TIKI_SDL )
#	include "SDL_main.h"
#endif

int main( int argc, char** argv )
{
	s_instanceHandle	= (tiki::InstanceHandle)GetModuleHandle( nullptr );
	s_ppArguments		= argv;
	s_argumentCount		= argc;

	s_arguments.create( (const char**)argv, (tiki::uint)argc );

	const int returnValue = tiki::platform::startApplication();

	s_arguments.dispose();
	return returnValue;
}

#endif

namespace tiki
{
	static Path s_currentDirectory;
	static Path s_executablePath;

	uint platform::getProcessorCount()
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );

		return sysinfo.dwNumberOfProcessors;
	}

	const Path& platform::getCurrentPath()
	{
		wchar_t widecharBuffer[ TIKI_MAX_PATH ];
		GetCurrentDirectoryW( TIKI_COUNT( widecharBuffer ), widecharBuffer );

		char utf8Buffer[ TIKI_MAX_PATH ];
		convertWidecharToUtf8String( utf8Buffer, TIKI_COUNT( utf8Buffer ), widecharBuffer );

		s_currentDirectory.setCompletePath( utf8Buffer );
		return s_currentDirectory;
	}

	const Path& platform::getExecutablePath()
	{
		HMODULE currentModule = GetModuleHandleA( nullptr );

		wchar_t widecharBuffer[ TIKI_MAX_PATH ];
		GetModuleFileNameW( currentModule, widecharBuffer, TIKI_COUNT( widecharBuffer ) );

		char utf8Buffer[ TIKI_MAX_PATH ];
		convertWidecharToUtf8String( utf8Buffer, TIKI_COUNT( utf8Buffer ), widecharBuffer );

		s_executablePath.setCompletePath( utf8Buffer );
		s_executablePath.pop();
		return s_executablePath;
	}

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

	bool platform::hasArgument( const char* pName )
	{
		for (uint i = 0u; i < s_arguments.getCount(); ++i)
		{
			if ( isStringEquals( s_arguments[ i ], pName ) )
			{
				return true;
			}
		}

		return false;
	}

	bool platform::findArgumentValue( const char** ppValue, const char* pName, bool onlyAssignment /* = false */ )
	{
		TIKI_ASSERT( ppValue != nullptr );

		for( uint i = 0u; i < s_arguments.getCount(); ++i )
		{
			const char* pArgument = s_arguments[ i ];

			if( !doesStringStartWith( pArgument, pName ) )
			{
				continue;
			}

			const uint nameLength = getStringLength( pName );
			if( pArgument[ nameLength ] == '=' )
			{
				*ppValue = pArgument + nameLength + 1u;
				return true;
			}
			else if( !onlyAssignment && i < s_arguments.getCount() - 1u )
			{
				*ppValue = s_arguments[ i + 1u ];
				return true;
			}

			break;
		}

		return false;
	}

	char** platform::getArgumentPointer()
	{
		return s_ppArguments;
	}

	int& platform::getArgumentCount()
	{
		return s_argumentCount;
	}
#endif
}
