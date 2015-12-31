#pragma once
#ifndef TIKI_UITYPES_HPP_INCLUDED
#define TIKI_UITYPES_HPP_INCLUDED

#include "tiki/container/fixedarray.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	enum UiElementFlags : uint8
	{
		UiElementFlags_None			= 0u,

		UiElementFlags_DockLeft		= 1u << 0u,
		UiElementFlags_DockTop		= 1u << 1u,
		UiElementFlags_DockBottom	= 1u << 2u,
		UiElementFlags_DockEight	= 1u << 3u
	};

	enum UiElementPoints : uint8
	{
		UiElementPoints_TopLeft,
		UiElementPoints_TopRight,
		UiElementPoints_BottomRight,
		UiElementPoints_BottomLeft,

		UiElementPoints_Count
	};

	enum UiSizeType : uint8
	{
		UiSizeType_Auto,
		UiSizeType_Expand,
		UiSizeType_Pixel,
		UiSizeType_Meters,
		UiSizeType_Percent
	};

	struct UiSize
	{
		UiSize()
		{
			type	= UiSizeType_Auto;
			value	= 0.0f;
		}

		UiSize( float value, UiSizeType type = UiSizeType_Pixel )
		{
			type	= type;
			value	= value;
		}

		UiSizeType	type;
		float		value;
	};
}

#endif // TIKI_UITYPES_HPP_INCLUDED
