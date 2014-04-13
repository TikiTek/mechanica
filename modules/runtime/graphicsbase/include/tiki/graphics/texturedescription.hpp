#pragma once
#ifndef TIKI_TEXTUREDESCRIPTION_HPP__INCLUDED
#define TIKI_TEXTUREDESCRIPTION_HPP__INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/graphics/pixelformat.hpp"

namespace tiki
{
	enum TextureType
	{
		TextureType_Invalid = -1,

		TextureType_1d,
		TextureType_2d,
		TextureType_3d,
		TextureType_Cube,

		TextureType_Count
	};

	enum TextureFlags
	{
		TextureFlags_None			= 0u,

		TextureFlags_RenderTarget	= 1u << 0u,
		TextureFlags_ShaderInput	= 1u << 1u,
		TextureFlags_DepthStencil	= 1u << 2u,

		TextureFlags_Count
	};

	struct TextureDescription
	{
		TextureDescription()
		{
			width		= 0u;
			height		= 0u;
			depth		= 0u;
			arrayCount	= 0u;
			mipCount	= 0u;

			flags		= TextureFlags_None;
			format		= PixelFormat_Invalid;
			type		= TextureType_Invalid;
		}

		uint16	width;
		uint16	height;
		uint16	depth;
		uint16	arrayCount;
		uint16	mipCount;

		uint16	flags;		// see TextureFlag
		uint16	format;		// see PixelFormat
		uint16	type;		// see TextureType
	};
}

#endif // TIKI_TEXTUREDESCRIPTION_HPP__INCLUDED
