
#include "tiki/base/platform.hpp"
#include "tiki/gameapplication/gameapplication.hpp"

#include "tiki/base/debug.hpp"

int tiki::mainEntryPoint()
{
	//debug::breakOnAlloc( 239 );
	//_CrtSetDbgFlag( _CRTDBG_CHECK_ALWAYS_DF );
	
	return framework::getGame().run();
}
