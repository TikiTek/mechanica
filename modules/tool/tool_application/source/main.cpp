#include "tiki/base/platform.hpp"
#include "tiki/tool_application/tool_application.hpp"

#include "tiki/base/debug.hpp"

namespace tiki
{
	int tiki::mainEntryPoint()
	{
		return tool::getTool().run();
	}
}
