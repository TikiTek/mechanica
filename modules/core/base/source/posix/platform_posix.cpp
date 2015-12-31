
#include "tiki/base/platform.hpp"

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )

int main( int argc, const char** argv )
{
	return tiki::mainEntryPoint();	
}

#endif

namespace tiki
{
	InstanceHandle getInstanceHandle()
	{
		return nullptr;
	}
}
