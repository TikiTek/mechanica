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
	class ScriptValue;
	class TextureData;
	class UiSystem;
	struct UiEventData;
	struct UiEventHandler;
	struct UiLayoutContext;

	enum UiElementEventType
	{
		UiElementEventType_MouseIn,
		UiElementEventType_MouseOut,
		UiElementEventType_MouseOver,
		UiElementEventType_MouseButtonDown,
		UiElementEventType_MouseButtonUp,
		UiElementEventType_MouseButtonClick,

		UiElementEventType_Count
	};

	typedef void( *UiEventFunc )( const UiEventData& eventData );

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

		const UiPosition&		getPosition() const;
		void					setPosition( const UiPosition& position );

		UiSize					getWidth() const;
		void					setWidth( UiSize width );

		UiSize					getHeight() const;
		void					setHeight( UiSize height );

		Thickness				getMargin() const;
		void					setMargin( Thickness margin );

		Thickness				getPadding() const;
		void					setPadding( Thickness padding );

		void					registerEventHandler( UiElementEventType type, UiEventFunc* pHandlerFunc );
		void					unregisterEventHandler( UiElementEventType type, UiEventFunc pHandlerFunc );

		void					setToColorRectangle( Color topLeft = TIKI_COLOR_WHITE, Color topRight = TIKI_COLOR_WHITE, Color bottomLeft = TIKI_COLOR_WHITE, Color bottomRight = TIKI_COLOR_WHITE );
		void					setToColorRectangleOne( Color color = TIKI_COLOR_WHITE );
		void					setToTextureRectangle( const TextureData* pTexture, const TexCoordArray* pTexCoords = nullptr, const ColorArray* pVertexColors = nullptr );
		void					setToText( const char* pText, const Font* pFont );

	private: // friend

		void					create( UiSystem* pUiSystem, UiElement* pParent );
		void					dispose();

		void					registerScriptEventHandler( UiElementEventType type, const ScriptValue& handlerFunc );
		void					unregisterScriptEventHandler( UiElementEventType type, const ScriptValue& handlerFunc );

	private:

		typedef FixedArray< LinkedList< UiEventHandler >, UiElementEventType_Count > UiEventHandlerArray;

		UiSystem*				m_pUiSystem;

		bool					m_layoutChanged;
		UiRectangle				m_layoutRectangle;

		UiPosition				m_position;
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

		UiEventHandlerArray		m_eventHandlers;

		void					setLayoutChanged( bool applyToChildren = true );

		void					updateLayout( const UiLayoutContext& context );

		static void				getElementLayoutSizeAndPosition( float& targetMin, float& targetMax, UiPositionElement elementPositionMin, UiPositionElement elementPositionMax, UiSize elementExtension, float parentMin, float parentMax, float childExtension, float marginMin, float marginMax, float paddingExtension, const UiLayoutContext& context );

	};
}

#endif // TIKI_UIELEMENT_HPP_INCLUDED
