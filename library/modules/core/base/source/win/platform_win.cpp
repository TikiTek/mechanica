
#include "tiki/base/platform.hpp"

namespace tiki
{
	uint getProcessorCount()
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );

		return sysinfo.dwNumberOfProcessors;
	}
}