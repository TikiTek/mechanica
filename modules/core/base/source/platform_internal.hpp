#pragma once

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
namespace tiki
{
	namespace platform
	{
		int startApplication();
	}
}
#endif
