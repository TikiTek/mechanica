
#include "tiki/base/platform.hpp"

#include "tiki/base/array.hpp"
#include "tiki/base/string.hpp"

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
	
	int returnValue = tiki::mainEntryPoint();
	
	s_arguments.dispose();
	
	return returnValue;
}

//int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
//{
//	s_instanceHandle	= (tiki::InstanceHandle)hInstance;
//

//}

namespace tiki
{
	PlatformType getHostPlatform()
	{
		return PlatformType_Win;
	}

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
