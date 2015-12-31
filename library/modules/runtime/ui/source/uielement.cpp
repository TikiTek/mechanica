#include "tiki/ui/uielement.hpp"

namespace tiki
{
	UiElement::UiElement()
	{
		m_pFont		= nullptr;
		m_pText		= nullptr;
		m_pTexture	= nullptr;
		m_pParent	= nullptr;
	}

	UiElement::~UiElement()
	{
	}

	void UiElement::create( UiElement* pParent )
	{
		m_pParent		= pParent;

		m_flags			= UiElementFlags_None;
		m_layoutChanged	= true;

		m_position		= Vector2::zero;
		m_layoutSize	= Vector2::zero;

		m_pFont			= nullptr;
		m_pText			= nullptr;
		m_pTexture		= nullptr;

		for( uint i = 0u; i < m_colors.getCount(); ++i )
		{
			m_colors[ i ]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		m_texCoords[ UiElementPoints_TopLeft ]		= vector::create( 0.0f, 0.0f );
		m_texCoords[ UiElementPoints_TopRight ]		= vector::create( 1.0f, 0.0f );
		m_texCoords[ UiElementPoints_BottomRight ]	= vector::create( 1.0f, 1.0f );
		m_texCoords[ UiElementPoints_BottomLeft ]	= vector::create( 0.0f, 1.0f );
	}

	void UiElement::dispose()
	{
		m_children.clear();
	}

	UiElementFlags UiElement::getFlags() const
	{
		return m_flags;
	}

	void UiElement::setFlags( UiElementFlags flags )
	{
		m_flags = flags;
	}

	Vector2 UiElement::getPosition() const
	{
		return m_position;
	}

	void UiElement::setPosition( const Vector2& position )
	{
		m_position = position;
		m_layoutChanged = true;
	}

	UiSize UiElement::getWidth() const
	{
		return m_width;
	}

	void UiElement::setWidth( UiSize width )
	{
		m_width = width;
		m_layoutChanged = true;
	}

	UiSize UiElement::getHeight() const
	{
		return m_height;
	}

	void UiElement::setHeight( UiSize height )
	{
		m_height = height;
		m_layoutChanged = true;
	}

	Thickness UiElement::getMargin() const
	{
		return m_margin;
	}

	void UiElement::setMargin( Thickness margin )
	{
		m_margin = margin;
		m_layoutChanged = true;
	}

	Thickness UiElement::getPadding() const
	{
		return m_padding;
	}

	void UiElement::setPadding( Thickness padding )
	{
		m_padding = padding;
		m_layoutChanged = true;
	}

	void UiElement::setToColorRectangle( const ColorArray* pVertexColors /*= nullptr */ )
	{
		m_pFont			= nullptr;
		m_pText			= nullptr;
		m_pTexture		= nullptr;

		if( pVertexColors != nullptr )
		{
			m_colors.create( pVertexColors->getBegin(), pVertexColors->getCount() );
		}
	}

	void UiElement::setToTextureRectangle( const TextureData* pTexture, const TexCoordArray* pTexCoords /*= nullptr*/, const ColorArray* pVertexColors /*= nullptr */ )
	{
		TIKI_ASSERT( pTexture != nullptr );

		m_pFont			= nullptr;
		m_pText			= nullptr;
		m_pTexture		= pTexture;

		if( pTexCoords != nullptr )
		{
			m_texCoords.create( pTexCoords->getBegin(), pTexCoords->getCount() );
		}

		if( pVertexColors != nullptr )
		{
			m_colors.create( pVertexColors->getBegin(), pVertexColors->getCount() );
		}
	}

	void UiElement::setToText( const char* pText, const Font* pFont )
	{
		TIKI_ASSERT( pText != nullptr );
		TIKI_ASSERT( pFont != nullptr );

		m_pFont			= pFont;
		m_pText			= pText;
		m_pTexture		= nullptr;

		m_layoutChanged = true;
	}
}