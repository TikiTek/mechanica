#pragma once
#ifndef TIKI_PLATFORM_HPP
#define TIKI_PLATFORM_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/staticarray.hpp"

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
		GraphicsApi_Invalid = -1,

		GraphicsApi_D3D11,
		GraphicsApi_D3D12,
		GraphicsApi_OpenGL4,

		GraphicsApi_Count
	};

	namespace platform
	{
		PlatformType			        getHostPlatform();
		GraphicsApi			        	getHostGraphicsApi();

		uint					        getProcessorCount();

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
		InstanceHandle					getInstanceHandle();
		const StaticArray< cstring >&	getArguments();
		bool							hasArgument( cstring name );
#endif
	}

	int							mainEntryPoint();
}

#endif // TIKI_PLATFORM_HPP
