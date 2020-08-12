
#include "tiki/base/platform.hpp"

namespace tiki
{
	PlatformType platform::getHostPlatform()
	{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		return PlatformType_Win;
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
        return PlatformType_Linux;
#else
#	error not supported
#endif
	}

	GraphicsApi platform::getHostGraphicsApi()
	{
#if TIKI_ENABLED( TIKI_GRAPHICS_D3D11 )
		return GraphicsApi_D3D11;
#elif TIKI_ENABLED( TIKI_GRAPHICS_D3D12 )
		return GraphicsApi_D3D12;
#elif TIKI_ENABLED( TIKI_GRAPHICS_VULKAN )
		return GraphicsApi_Vulkan;
#else
#	error not supported
#endif
	}
}

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )

#include "tiki/base/debug.hpp"
#include "tiki/base/string.hpp"
#include "tiki/io/path.hpp"

#include "platform_internal.hpp"

namespace tiki
{
	int platform::startApplication()
	{
		{
			const string currentPath = path::getCurrentDir();
			const string exePath = path::getExecutablePath();

			TIKI_TRACE_DEBUG( "Executable Path: %s\n", exePath.cStr() );
			TIKI_TRACE_DEBUG( "Current Path: %s\n", currentPath.cStr() );
		}

		int returnValue = 0;
		{
			returnValue = mainEntryPoint();
		}

		debug::dumpMemoryStats();

		return returnValue;
	}
}

#endif