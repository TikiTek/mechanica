#pragma once
#ifndef TIKI_UITYPES_HPP_INCLUDED
#define TIKI_UITYPES_HPP_INCLUDED

#include "tiki/container/fixedarray.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/input/mousebutton.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	enum UiElementPoints : uint8
	{
		UiElementPoints_BottomLeft,
		UiElementPoints_TopLeft,
		UiElementPoints_BottomRight,
		UiElementPoints_TopRight,

		UiElementPoints_Count
	};

	enum UiElementEdges : uint8
	{
		UiElementEdges_Left,
		UiElementEdges_Top,
		UiElementEdges_Bottom,
		UiElementEdges_Right,

		UiElementEdges_Count
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

	enum UiPositionType : uint8
	{
		UiPositionType_Auto,
		UiPositionType_Pixel,
		UiPositionType_Meters,
		UiPositionType_Percent
	};

	struct UiPositionElement
	{
		UiPositionElement()
		{
			type	= UiPositionType_Auto;
			value	= 0.0f;
		}

		UiPositionElement( float _value, UiPositionType _type = UiPositionType_Pixel )
		{
			type	= _type;
			value	= _value;
		}

		UiPositionType	type;
		float			value;
	};

	struct UiPosition
	{
		UiPosition( UiPositionElement left = UiPositionElement(), UiPositionElement top = UiPositionElement(), UiPositionElement bottom = UiPositionElement(), UiPositionElement right = UiPositionElement() )
		{
			edgeMode[ UiElementEdges_Left ]		= left.type;
			edgeValue[ UiElementEdges_Left ]	= left.value;
			edgeMode[ UiElementEdges_Top ]		= top.type;
			edgeValue[ UiElementEdges_Top ]		= top.value;
			edgeMode[ UiElementEdges_Bottom ]	= bottom.type;
			edgeValue[ UiElementEdges_Bottom ]	= bottom.value;
			edgeMode[ UiElementEdges_Right ]	= right.type;
			edgeValue[ UiElementEdges_Right ]	= right.value;
		}

		CopyableFixedArray< UiPositionType, UiElementEdges_Count>	edgeMode;
		CopyableFixedArray< float, UiElementEdges_Count >			edgeValue;

		UiPositionElement	getLeft() const		{ return UiPositionElement( edgeValue[ UiElementEdges_Left ], edgeMode[ UiElementEdges_Left ] ); }
		UiPositionElement	getTop() const		{ return UiPositionElement( edgeValue[ UiElementEdges_Top ], edgeMode[ UiElementEdges_Top ] ); }
		UiPositionElement	getBottom() const	{ return UiPositionElement( edgeValue[ UiElementEdges_Bottom ], edgeMode[ UiElementEdges_Bottom ] ); }
		UiPositionElement	getRight() const	{ return UiPositionElement( edgeValue[ UiElementEdges_Right ], edgeMode[ UiElementEdges_Right ] ); }
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

		TIKI_FORCE_INLINE UiRectangle( float _left, float _top, float _bottom, float _right )
			: left( _left ), top( _top ), bottom( _bottom ), right( _right )
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

		TIKI_FORCE_INLINE bool contains( const Vector2& point ) const
		{
			return (point.x >= left) && (point.x <= right) && (point.y >= top) && (point.y <= bottom);
		}
	};

	enum UiEventType
	{
		UiEventType_MouseIn,
		UiEventType_MouseOut,
		UiEventType_MouseOver,
		UiEventType_MouseButtonDown,
		UiEventType_MouseButtonUp,
		UiEventType_MouseButtonClick,
		UiEventType_PositionChanged,
		UiEventType_SizeChanged,

		UiEventType_Count
	};

	union UiEventData
	{
		UiEventData()
		{
		}

		struct UiMouseEventData
		{
			Vector2		position;
			bool		buttons[ MouseButton_Count ];
		} mouse;

		struct UiPositionEventData
		{
			UiPosition	position;
		} position;

		struct UiSizeEventData
		{
			UiSize		width;
			UiSize		height;
		} size;
	};

	struct UiEvent
	{
		UiEvent()
		{
			type = UiEventType_Count;
		}

		UiEventType	type;
		UiEventData	data;
	};
}

#endif // TIKI_UITYPES_HPP_INCLUDED
