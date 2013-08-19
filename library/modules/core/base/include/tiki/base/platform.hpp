#pragma once
#ifndef TIKI_PLATFORM_HPP
#define TIKI_PLATFORM_HPP

#include "tiki/base/types.hpp"

namespace tiki
{
	struct InstanceHandleType;
	typedef InstanceHandleType* InstanceHandle;

	enum PlatformType
	{
		Platform_Win,
		Platform_Linux,
		Platform_Apple
	};

	PlatformType	getHostPlatform();
	InstanceHandle	getInstanceHandle();

	int				mainEntryPoint();
}

#endif // TIKI_PLATFORM_HPP
