#pragma once
#ifndef TIKI_UITYPES_HPP_INCLUDED
#define TIKI_UITYPES_HPP_INCLUDED

#include "tiki/base/linkedlist.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class Font;
	class TextureData;

	enum UiElementFlags
	{
		UiElementFlags_None			= 0u,

		UiElementFlags_DockLeft		= 1u << 0u,
		UiElementFlags_DockTop		= 1u << 1u,
		UiElementFlags_DockBottom	= 1u << 2u,
		UiElementFlags_DockEight	= 1u << 3u
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
		UiSizeType	type;
		float		value;
	};

	struct UiElement
	{
		Vector2			position;
		UiSize			width;
		UiSize			height;
		Vector4			margin;
		Vector4			padding;
		UiElementFlags	flags;

		LinkedList< UiElement >	children;
	};

	struct UiRenderData
	{


		LinkedList< UiElement >	elements;
	};
}

#endif // TIKI_UITYPES_HPP_INCLUDED
