
#include "tiki/base/platform.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )

#include <windows.h>

tiki::InstanceHandle	s_instanceHandle;

int main( int argc, char** argv )
{
	s_instanceHandle	= (tiki::InstanceHandle)GetModuleHandle( nullptr );

	return tiki::mainEntryPoint();
}

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
	s_instanceHandle	= (tiki::InstanceHandle)hInstance;

	return tiki::mainEntryPoint();
}

namespace tiki
{
	PlatformType getHostPlatform()
	{
		return Platform_Win;
	}

	InstanceHandle getInstanceHandle()
	{
		return s_instanceHandle;
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
