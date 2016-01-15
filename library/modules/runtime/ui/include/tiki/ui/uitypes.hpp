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

		UiSize( float _value, UiSizeType _type = UiSizeType_Pixel )
		{
			type	= _type;
			value	= _value;
		}

		UiSizeType	type;
		float		value;
	};

	struct UiRectangle
	{
		float left;
		float top;
		float bottom;
		float right;

		TIKI_FORCE_INLINE UiRectangle()
			: left( 0.0f ), top( 0.0f ), bottom( 0.0f ), right( 0.0f )
		{
		}

		TIKI_FORCE_INLINE void clear()
		{
			left	= 0.0f;
			top		= 0.0f;
			bottom	= 0.0f;
			right	= 0.0f;
		}

		TIKI_FORCE_INLINE void extend( UiRectangle other )
		{
			left	= TIKI_MIN( left, other.left );
			top		= TIKI_MIN( top, other.top );
			bottom	= TIKI_MAX( bottom, other.bottom );
			right	= TIKI_MAX( right, other.right );
		}

		TIKI_FORCE_INLINE float getWidth() const
		{
			return right - left;
		}

		TIKI_FORCE_INLINE float getHeight() const
		{
			return bottom - top;
		}
	};
}

#endif // TIKI_UITYPES_HPP_INCLUDED
