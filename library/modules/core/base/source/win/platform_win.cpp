
#include "tiki/base/platform.hpp"

#include <windows.h>

namespace tiki
{
	uint getProcessorCount()
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );

		return sysinfo.dwNumberOfProcessors;
	}
}
