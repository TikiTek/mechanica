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
		PlatformType_Invalid,

		PlatformType_Windows,
		PlatformType_UWP,
		PlatformType_Linux,
		PlatformType_Android,
		PlatformType_OSX,
		PlatformType_IOS,
		PlatformType_XboxOne,
		PlatformType_Playstation4,
		PlatformType_Switch,

		PlatformType_Count
	};

	enum GraphicsApi
	{
		GraphicsApi_Invalid,

		GraphicsApi_D3D11,
		GraphicsApi_D3D12,
		//GraphicsApi_OpenGL, // die OpenGL, die!
		GraphicsApi_Vulkan,
		GraphicsApi_Metal,
		GraphicsApi_GNM,
		GraphicsApi_NVN,

		GraphicsApi_Count,
	};

	enum SoundApi
	{
		SoundApi_Invalid,

		SoundApi_XAudio,
		SoundApi_SoftMix,

		SoundApi_Count,
	};

	namespace platform
	{
		PlatformType							getHostPlatform();
		GraphicsApi								getHostGraphicsApi();
		SoundApi								getHostSoundApi();

		uint									getProcessorCount();

		void									getUserName( char* pBuffer, uint bufferSize );

#if TIKI_DISABLED( TIKI_BUILD_LIBRARY )
		InstanceHandle							getInstanceHandle();
		const StaticArray< const char* >&		getArguments();
		bool									hasArgument( const char* pName );
		bool									findArgumentValue( const char** ppValue, const char* pName );

		char**									getArgumentPointer();
		int&									getArgumentCount();
#endif
	}

	int											mainEntryPoint();
}

#endif // TIKI_PLATFORM_HPP
