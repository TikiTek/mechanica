
#include "tiki/base/platform.hpp"

namespace tiki
{
	PlatformType getHostPlatform()
	{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		return PlatformType_Win;
#else
#	error not supported
#endif
	}

	GraphicsApi getHostGraphicsApi()
	{
#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
		return GraphicsApi_D3D11;
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
		return GraphicsApi_D3D12;
#elif TIKI_ENABLED( TIKI_GRAPHICS_OPENGL4 )
		return GraphicsApi_OpenGL4;
#else
#	error not supported
#endif
	}
}

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )

#include "tiki/base/array.hpp"
#include "tiki/base/debug.hpp"
#include "tiki/base/iopath.hpp"
#include "tiki/base/string.hpp"

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
#	include "tiki/base/reflection.hpp"
#endif

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )

#include <windows.h>

tiki::InstanceHandle		s_instanceHandle;
tiki::Array< tiki::string >	s_arguments;

int main( int argc, char** argv )
{
	s_instanceHandle	= (tiki::InstanceHandle)GetModuleHandle( nullptr );

	s_arguments.create( argc );
	for (int i = 0u; i < argc; ++i)
	{
		s_arguments[ i ] = argv[ i ];
	} 
	
#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
	tiki::reflection::initialize();

	{
		const tiki::string currentPath = tiki::path::getCurrentDir();
		const tiki::string exePath = tiki::path::getExecutablePath();

		TIKI_TRACE_DEBUG( "Executable Path: %s\n", exePath.cStr() );
		TIKI_TRACE_DEBUG( "Current Path: %s\n", currentPath.cStr() );
	}
#endif

	int returnValue = 0;
	{
		returnValue = tiki::mainEntryPoint();

		s_arguments.dispose();
	}

#if TIKI_ENABLED( TIKI_BUILD_TOOLS )
	tiki::reflection::shutdown();
#endif

	tiki::debug::dumpMemoryStats();

	return returnValue;
}

namespace tiki
{
	InstanceHandle getInstanceHandle()
	{
		return s_instanceHandle;
	}

	const Array< string >& getArguments()
	{
		return s_arguments;
	}
	
	bool hasArgument( const string& name )
	{
		for (uint i = 0u; i < s_arguments.getCount(); ++i)
		{
			if ( s_arguments[ i ] == name )
			{
				return true;
			}
		}

		return false;
	}
}

#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )

int main( int argc, const char** argv )
{
	return tiki::mainEntryPoint();	
}

namespace tiki
{
	PlatformType getHostPlatform()
	{
		return Platform_Linux;
	}

	InstanceHandle getInstanceHandle()
	{
		return nullptr;
	}
}

#else

#	error Platform not implemented

#endif

#endif