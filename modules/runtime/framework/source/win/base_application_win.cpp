#include "tiki/framework/base_application.hpp"

namespace tiki
{
	bool BaseApplication::initializePlatform()
	{
		return true;
	}

	void BaseApplication::shutdownPlatform()
	{
		Thread::shutdownSystem();
	}

	void BaseApplication::updatePlatform()
	{
	}
}
