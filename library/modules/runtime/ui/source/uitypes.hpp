#pragma once
#ifndef TIKI_UITYPES_HPP_INCLUDED
#define TIKI_UITYPES_HPP_INCLUDED

#include "tiki/container/fixedarray.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/graphics/color.hpp"
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

	enum UiElementPoints
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
		UiSizeType	type;
		float		value;
	};

	struct UiElement : public LinkedItem< UiElement >
	{
		typedef FixedArray< HdrColor, UiElementPoints_Count >	ColorArray;
		typedef FixedArray< Vector2, UiElementPoints_Count >	TexCoordArray;

		UiElementFlags			flags;

		Vector2					position;
		UiSize					width;
		UiSize					height;

		Vector2					layoutSize;
		Vector4					margin;
		Vector4					padding;

		const Font*				pFont;
		const char*				pText;

		const TextureData*		pTexture;
		ColorArray				colors;
		TexCoordArray			texCoords;

		LinkedList< UiElement >	children;
	};

	struct UiRenderData
	{


		LinkedList< UiElement >	elements;
	};
}

#endif // TIKI_UITYPES_HPP_INCLUDED
