#pragma once
#ifndef TIKI_PLATFORM_HPP
#define TIKI_PLATFORM_HPP

#include "tiki/base/types.hpp"
#include "tiki/container/staticarray.hpp"

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

		GraphicsApi_Count,

		GraphicsApi_OpenGL4,
	};

	namespace platform
	{
		PlatformType							getHostPlatform();
		GraphicsApi							getHostGraphicsApi();

		uint									getProcessorCount();

		void									getUserName( char* pBuffer, uint bufferSize );

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
		InstanceHandle							getInstanceHandle();
		const StaticArray< const char* >&	getArguments();
		bool									hasArgument( const char* name );
#endif
	}

	int											mainEntryPoint();
}

#endif // TIKI_PLATFORM_HPP
