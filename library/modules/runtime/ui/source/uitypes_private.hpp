#pragma once
#ifndef TIKI_UITYPES_PRIVATE_HPP_INCLUDED
#define TIKI_UITYPES_PRIVATE_HPP_INCLUDED

#include "tiki/ui/uielement.hpp"

namespace tiki
{
	class Font;

	struct UiRenderData
	{
		UiRenderData( const LinkedList< UiElement >& _elements )
			: elements( _elements )
		{
		}

		float							screenWidth;
		float							screenHeight;

		const LinkedList< UiElement >&	elements;
	};

	enum UiRenderElementType : uint8
	{
		UiRenderElementType_ColorRectangle,
		UiRenderElementType_TextureRectangle,
		UiRenderElementType_Text
	};

	struct UiRenderElementColorRectangleData
	{
		typedef FixedArray< Color, UiElementPoints_Count > ColorArray;

		ColorArray			colors;
	};

	struct UiRenderElementDataTextureRectangleData : public UiRenderElementColorRectangleData
	{
		typedef FixedArray< Vector2, UiElementPoints_Count >	TexCoordArray;

		TexCoordArray		texCoords;
		const TextureData*	pTextureData;
	};

	struct UiRenderElementDataTextData
	{
		const Font*		pFont;
		const char*		pText;
	};

	union UiRenderElementData
	{
		UiRenderElementData() { }
		~UiRenderElementData() { }

		UiRenderElementColorRectangleData		coloredRectangle;
		UiRenderElementDataTextureRectangleData	textureRectangle;
		UiRenderElementDataTextData				text;
	};

	struct UiRenderElement
	{
		UiRenderElementType	type;

		Vector2				position;
		Vector2				size;
		float				rotation;

		UiRenderElementData	data;
	};

	struct UiVertex
	{
		float2	position;
		float2	texCood;
		Color	color;
	};

	struct UiLayoutContext
	{
		float	meterToPixel;
	};

	struct UiEventHandler
	{
		ScriptValue		scriptFunc;
		UiEventFunc*	pEventFunc;
	};
}

#endif // TIKI_UITYPES_PRIVTAE_HPP_INCLUDED
