#include "tiki/ui/uirenderer.hpp"

#include "tiki/base/fixedarray.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	class Font;

	enum UiRenderElementType : uint8
	{
		UiRenderElementType_ColorRectangle,
		UiRenderElementType_TextureRectangle,
		UiRenderElementType_Text
	};

	enum UiRenderVertexIndex
	{
		UiRenderVertexIndex_TopLeft,
		UiRenderVertexIndex_TopRight,
		UiRenderVertexIndex_BottomRight,
		UiRenderVertexIndex_BottomLeft,

		UiRenderVertexIndex_Count
	};

	struct UiRenderElement
	{
		UiRenderElementType	type;

		Vector2				position;
	};

	struct UiRenderElementColorRectangle : public UiRenderElement
	{
		typedef FixedArray< HdrColor, UiRenderVertexIndex_Count >	ColorArray;

		Vector2				size;
		float				rotation;

		ColorArray			colors;
	};

	struct UiRenderElementDataTextureRectangle : public UiRenderElementColorRectangle
	{
		typedef FixedArray< Vector2, UiRenderVertexIndex_Count >	TexCoordArray;

		TexCoordArray		texCoords;
		const TextureData*	pTextureData;
	};

	struct UiRenderElementDataText : public UiRenderElement
	{
		const Font*		pFont;
		const char*		pText;
	};

	UiRenderer::UiRenderer()
	{
	}

	UiRenderer::~UiRenderer()
	{
	}

	bool UiRenderer::create( const UiRendererParameters& parameters )
	{
		return false;
	}

	void UiRenderer::dispose()
	{
	}

	void UiRenderer::update( const UiRenderData& renderData )
	{
	}

	void UiRenderer::render( GraphicsContext& context ) const
	{
	}

}
