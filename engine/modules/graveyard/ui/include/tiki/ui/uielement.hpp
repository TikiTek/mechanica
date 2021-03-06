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
	class UiElement;
	class UiSystem;
	struct UiEvent;
	struct UiEventHandler;
	struct UiInputState;
	struct UiLayoutContext;

	typedef void( *UiEventFunc )( UiElement* pElement, const UiEvent& eventData );

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

		crc32					getTypeCrc() const { return m_typeCrc; }

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

		uint					getChildCount() const;
		UiElement*				getChildByIndex( uint index );
		UiElement*				getFirstChild();

		void					registerEventHandler( UiEventType type, UiEventFunc pHandlerFunc );
		void					unregisterEventHandler( UiEventType type, UiEventFunc pHandlerFunc );

		void					setToColorRectangle( Color topLeft = TIKI_COLOR_WHITE, Color topRight = TIKI_COLOR_WHITE, Color bottomLeft = TIKI_COLOR_WHITE, Color bottomRight = TIKI_COLOR_WHITE );
		void					setToColorRectangleOne( Color color = TIKI_COLOR_WHITE );
		void					setToTextureRectangle( const TextureData* pTexture, const TexCoordArray* pTexCoords = nullptr, const ColorArray* pVertexColors = nullptr );
		void					setToText( const char* pText, const Font* pFont );

	private: // friend

		void					create( UiSystem* pUiSystem, UiElement* pParent, crc32 typeCrc );
		void					dispose();

		void					registerScriptEventHandler( UiEventType type, const ScriptValue& handlerFunc );
		void					unregisterScriptEventHandler( UiEventType type, const ScriptValue& handlerFunc );

		void					raiseEvent( const UiEvent& eventData );

	private:

		typedef FixedArray< LinkedList< UiEventHandler >, UiEventType_Count > UiEventHandlerArray;

		UiSystem*				m_pUiSystem;

		crc32					m_typeCrc;

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

		bool					m_mouseMoveState;
		MouseButtonState		m_mouseClickState;
		UiEventHandlerArray		m_eventHandlers;

		void					setLayoutChanged( bool applyToChildren = true );

		void					updateLayout( const UiLayoutContext& context );

		bool					checkMouseMoveEvent( const UiInputState& state );
		bool					checkMouseClickEvent( const UiInputState& prevState, const UiInputState& currentState );

		static void				getElementLayoutSizeAndPosition( float& targetMin, float& targetMax, UiPositionElement elementPositionMin, UiPositionElement elementPositionMax, UiSize elementExtension, float parentMin, float parentMax, float childExtension, float marginMin, float marginMax, float paddingExtension, const UiLayoutContext& context );

	};
}

#endif // TIKI_UIELEMENT_HPP_INCLUDED
