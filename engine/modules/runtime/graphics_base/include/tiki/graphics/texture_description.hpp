#pragma once

#include "tiki/base/types.hpp"
#include "tiki/graphics/pixel_format.hpp"

namespace tiki
{
	enum TextureType : uint8
	{
		TextureType_1d,
		TextureType_2d,
		TextureType_3d,
		TextureType_Cube,

		TextureType_Count,
		TextureType_Invalid = 0xffu,
	};

	enum TextureFlags : uint8
	{
		TextureFlags_None			= 0u,

		TextureFlags_RenderTarget	= 1u << 0u,
		TextureFlags_ShaderInput	= 1u << 1u,
		TextureFlags_DepthStencil	= 1u << 2u
	};
	TIKI_FLAGS_ENUM( TextureFlags );

	struct TextureDescription
	{
		uint16			width		= 0u;
		uint16			height		= 0u;
		uint16			depth		= 1u;
		uint16			arrayCount	= 1u;
		uint8			mipCount	= 1u;

		TextureFlags	flags		= TextureFlags_None;
		PixelFormat		format		= PixelFormat_Invalid;
		TextureType		type		= TextureType_Invalid;
	};
}
