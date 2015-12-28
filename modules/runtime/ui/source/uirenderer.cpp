#include "tiki/ui/uirenderer.hpp"

#include "tiki/base/fixedarray.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/vector.hpp"

#include "uitypes.hpp"

namespace tiki
{
	class Font;

	enum UiRenderElementType : uint8
	{
		UiRenderElementType_ColorRectangle,
		UiRenderElementType_TextureRectangle,
		UiRenderElementType_Text
	};
	
	struct UiRenderElement
	{
		UiRenderElementType	type;

		Vector2				position;
	};

	struct UiRenderElementColorRectangle : public UiRenderElement
	{
		typedef FixedArray< HdrColor, UiElementPoints_Count >	ColorArray;

		Vector2				position;
		Vector2				size;
		float				rotation;

		ColorArray			colors;
	};

	struct UiRenderElementDataTextureRectangle : public UiRenderElementColorRectangle
	{
		typedef FixedArray< Vector2, UiElementPoints_Count >	TexCoordArray;

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
		//if ( !m_renderElements.create( parameters.maxRenderElements ) )
		//{
		//	dispose();
		//	return false;
		//}

		return true;
	}

	void UiRenderer::dispose()
	{
	}

	void UiRenderer::update( const UiRenderData& renderData )
	{
		//m_renderElements.clear();
	}

	void UiRenderer::render( GraphicsContext& context ) const
	{
	}

	void UiRenderer::updateRecursiveRenderTree( const UiElement& element )
	{
		UiRenderElement* pRenderElement = nullptr;
		if( element.pFont != nullptr )
		{
			//pRenderElement = m_
		}
		

		for( uint i = 0u; i < element.children.getCount(); ++i )
		{

		}
	}
}
