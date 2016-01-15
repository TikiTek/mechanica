#pragma once
#ifndef TIKI_UIELEMENT_HPP_INCLUDED
#define TIKI_UIELEMENT_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/math/thickness.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/ui/uitypes.hpp"

namespace tiki
{
	class Font;
	class TextureData;
	class UiSystem;

	class UiElement : public LinkedItem< UiElement >
	{
		TIKI_NONCOPYABLE_CLASS( UiElement );
		friend class UiSystem;
		friend class UiRenderer;

	public:

		typedef FixedArray< Color, UiElementPoints_Count >		ColorArray;
		typedef FixedArray< Vector2, UiElementPoints_Count >	TexCoordArray;

								UiElement();
								~UiElement();

		UiElementFlags			getFlags() const;
		void					setFlags( UiElementFlags flags );

		Vector2					getPosition() const;
		void					setPosition( const Vector2& position );

		UiSize					getWidth() const;
		void					setWidth( UiSize width );

		UiSize					getHeight() const;
		void					setHeight( UiSize height );

		Thickness				getMargin() const;
		void					setMargin( Thickness margin );

		Thickness				getPadding() const;
		void					setPadding( Thickness padding );

		void					setToColorRectangle( Color color );
		void					setToColorRectangle( const ColorArray* pVertexColors = nullptr );
		void					setToTextureRectangle( const TextureData* pTexture, const TexCoordArray* pTexCoords = nullptr, const ColorArray* pVertexColors = nullptr );
		void					setToText( const char* pText, const Font* pFont );

	private:

		void					create( UiElement* pParent );
		void					dispose();

		UiElementFlags			m_flags;
		bool					m_layoutChanged;
		Vector2					m_layoutSize;
		UiRectangle				m_boundingRectangle;

		Vector2					m_position;
		UiSize					m_width;
		UiSize					m_height;

		Thickness				m_margin;
		Thickness				m_padding;

		const Font*				m_pFont;
		const char*				m_pText;

		const TextureData*		m_pTextureData;
		ColorArray				m_colors;
		TexCoordArray			m_texCoords;

		UiElement*				m_pParent;
		LinkedList< UiElement >	m_children;

	};
}

#endif // TIKI_UIELEMENT_HPP_INCLUDED
