#pragma once
#ifndef TIKI_TEXTUREDESCRIPTION_HPP__INCLUDED
#define TIKI_TEXTUREDESCRIPTION_HPP__INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/graphics/pixelformat.hpp"

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
		TextureDescription()
		{
			width		= 0u;
			height		= 0u;
			depth		= 1u;
			arrayCount	= 1u;
			mipCount	= 1u;

			flags		= TextureFlags_None;
			format		= PixelFormat_Invalid;
			type		= TextureType_Invalid;
		}

		uint16			width;
		uint16			height;
		uint16			depth;
		uint16			arrayCount;
		uint8			mipCount;

		TextureFlags	flags;
		PixelFormat		format;
		TextureType		type;
	};
}

#endif // TIKI_TEXTUREDESCRIPTION_HPP__INCLUDED
