#pragma once
#ifndef TIKI_PLATFORM_HPP
#define TIKI_PLATFORM_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/forwarddeclaration.hpp"

namespace tiki
{
	TIKI_DEFINE_HANLE( InstanceHandle );
	TIKI_DEFINE_HANLE( WindowHandle );

	enum PlatformType
	{
		PlatformType_Invalid = -1,

		PlatformType_Win,
		PlatformType_Linux,
		PlatformType_Apple,

		PlatformType_Count
	};

	enum GraphicsApi
	{
		GraphicsApi_D3D11,
		GraphicsApi_OpenGL4,

		GraphicsApi_Count
	};

	PlatformType			getHostPlatform();

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
	InstanceHandle			getInstanceHandle();
	const Array< string >&	getArguments();
	bool					hasArgument( const string& name );

	int						mainEntryPoint();
#endif
}

#endif // TIKI_PLATFORM_HPP
