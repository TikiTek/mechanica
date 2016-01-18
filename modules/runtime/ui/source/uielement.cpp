#include "tiki/ui/uielement.hpp"

#include "uitypes_private.hpp"

namespace tiki
{
	UiElement::UiElement()
	{
		m_pFont			= nullptr;
		m_pText			= nullptr;
		m_pTextureData	= nullptr;
		m_pParent		= nullptr;
	}

	UiElement::~UiElement()
	{
	}

	void UiElement::create( UiElement* pParent )
	{
		m_pParent = pParent;
		if( m_pParent )
		{
			m_pParent->m_children.push( this );
		}

		m_layoutPosition	= Vector2::zero;
		m_layoutSize		= Vector2::zero;

		m_pFont				= nullptr;
		m_pText				= nullptr;
		m_pTextureData		= nullptr;

		for( uint i = 0u; i < m_colors.getCount(); ++i )
		{
			m_colors[ i ] = TIKI_COLOR_WHITE;
		}

		setLayoutChanged();
	}

	void UiElement::dispose()
	{
		if( m_pParent )
		{
			m_pParent->m_children.removeSortedByValue( *this );
		}

		m_children.clear();
	}

	const UiPosition& UiElement::getPosition() const
	{
		return m_position;
	}

	void UiElement::setPosition( const UiPosition& position )
	{
		m_position = position;
		setLayoutChanged();
	}

	UiSize UiElement::getWidth() const
	{
		return m_width;
	}

	void UiElement::setWidth( UiSize width )
	{
		m_width = width;
		setLayoutChanged();
	}

	UiSize UiElement::getHeight() const
	{
		return m_height;
	}

	void UiElement::setHeight( UiSize height )
	{
		m_height = height;
		setLayoutChanged();
	}

	Thickness UiElement::getMargin() const
	{
		return m_margin;
	}

	void UiElement::setMargin( Thickness margin )
	{
		m_margin = margin;
		setLayoutChanged();
	}

	Thickness UiElement::getPadding() const
	{
		return m_padding;
	}

	void UiElement::setPadding( Thickness padding )
	{
		m_padding = padding;
		setLayoutChanged();
	}

	void UiElement::setToColorRectangle( Color topLeft /* = TIKI_COLOR_WHITE */, Color topRight /* = TIKI_COLOR_WHITE */, Color bottomLeft /* = TIKI_COLOR_WHITE */, Color bottomRight /* = TIKI_COLOR_WHITE */ )
	{
		m_pFont			= nullptr;
		m_pText			= nullptr;
		m_pTextureData	= nullptr;

		m_colors[ UiElementPoints_TopLeft ]			= topLeft;
		m_colors[ UiElementPoints_TopRight ]		= topRight;
		m_colors[ UiElementPoints_BottomLeft ]		= bottomLeft;
		m_colors[ UiElementPoints_BottomRight ]		= bottomRight;
	}

	void UiElement::setToTextureRectangle( const TextureData* pTexture, const TexCoordArray* pTexCoords /*= nullptr*/, const ColorArray* pVertexColors /*= nullptr */ )
	{
		TIKI_ASSERT( pTexture != nullptr );

		m_pFont			= nullptr;
		m_pText			= nullptr;
		m_pTextureData	= pTexture;

		if( pTexCoords != nullptr )
		{
			m_texCoords.create( pTexCoords->getBegin(), pTexCoords->getCount() );
		}
		else
		{
			vector::set( m_texCoords[ UiElementPoints_TopLeft ],		0.0f, 0.0f );
			vector::set( m_texCoords[ UiElementPoints_TopRight ],		1.0f, 0.0f );
			vector::set( m_texCoords[ UiElementPoints_BottomLeft ],		0.0f, 1.0f );
			vector::set( m_texCoords[ UiElementPoints_BottomRight ],	1.0f, 1.0f );
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
		m_pTextureData	= nullptr;

		setLayoutChanged();
	}

	void UiElement::setLayoutChanged( bool applyToChildren )
	{
		m_layoutChanged = true;

		if( m_pParent )
		{
			m_pParent->setLayoutChanged( false );
		}

		if( applyToChildren )
		{
			for( UiElement& child : m_children )
			{
				child.setLayoutChanged( applyToChildren );
			}
		}
	}

	void UiElement::updateLayout( const UiLayoutContext& context )
	{
		if( !m_layoutChanged )
		{
			return;
		}

		UiRectangle parentBounds;
		if( m_pParent )
		{
			parentBounds = m_pParent->m_layoutRectangle;
		}
		else
		{
			parentBounds.left	= 0.0f;
			parentBounds.top	= 0.0f;
			parentBounds.bottom	= m_height.value;
			parentBounds.right	= m_width.value;
		}
		m_layoutRectangle = parentBounds;

		UiRectangle childBounds;
		for( UiElement& child : m_children )
		{
			child.updateLayout( context );
			childBounds.extend( child.m_layoutRectangle );
		}

		m_layoutSize.x		= getElementLayoutSizeAndPosition( m_layoutRectangle.left, m_layoutRectangle.right, m_position.getLeft(), m_position.getRight(), m_width, parentBounds.left, parentBounds.right, childBounds.getWidth(), m_margin.left, m_margin.right, m_padding.getWidth(), context );
		m_layoutSize.y		= getElementLayoutSizeAndPosition( m_layoutRectangle.top, m_layoutRectangle.bottom, m_position.getTop(), m_position.getBottom(), m_height, parentBounds.top, parentBounds.bottom, childBounds.getHeight(), m_margin.top, m_margin.bottom, m_padding.getHeight(), context );

		m_layoutPosition.x	= m_layoutRectangle.left;
		m_layoutPosition.y	= m_layoutRectangle.top;

		m_layoutChanged = false;
	}

	/*static*/ float UiElement::getElementLayoutSizeAndPosition( float& targetMin, float& targetMax, UiPositionElement elementPositionMin, UiPositionElement elementPositionMax, UiSize elementExtension, float parentMin, float parentMax, float childExtension, float marginMin, float marginMax, float paddingExtension, const UiLayoutContext& context )
	{
		const float minMin	= parentMin + marginMin;
		const float maxMax	= parentMax - marginMax;
		const float maxSize	= maxMax - minMin;

		float wantedSize	= 0.0f;
		switch( elementExtension.type )
		{
		case UiSizeType_Auto:
			break;

		case UiSizeType_Expand:
			wantedSize = maxMax;
			break;

		case UiSizeType_Pixel:
			wantedSize = elementExtension.value;
			break;

		case UiSizeType_Meters:
			wantedSize = elementExtension.value * context.meterToPixel;
			break;

		case UiSizeType_Percent:
			wantedSize = maxSize * (elementExtension.value / 100.0f);
			break;

		default:
			break;
		}
		const float requiredSize = childExtension + paddingExtension;
		wantedSize = TIKI_MAX( requiredSize, wantedSize );

		float minValue = 0.0f;
		switch( elementPositionMin.type )
		{
		case UiPositionType_Auto:
			break;

		case UiPositionType_Pixel:
			minValue = elementPositionMin.value;
			break;

		case UiPositionType_Meters:
			minValue = elementPositionMin.value * context.meterToPixel;
			break;

		case UiPositionType_Percent:
			minValue = maxSize * (elementPositionMin.value / 100.0f);
			break;

		default:
			break;
		}

		float maxValue = 0.0f;
		switch( elementPositionMax.type )
		{
		case UiPositionType_Auto:
			break;

		case UiPositionType_Pixel:
			minValue = elementPositionMax.value;
			break;

		case UiPositionType_Meters:
			minValue = elementPositionMax.value * context.meterToPixel;
			break;

		case UiPositionType_Percent:
			minValue = maxSize * (elementPositionMax.value / 100.0f);
			break;

		default:
			break;
		}

		float size = 0.0f;
		if( elementPositionMin.type == UiPositionType_Auto && elementPositionMax.type == UiPositionType_Auto )
		{
			wantedSize = TIKI_MIN( wantedSize, maxSize );
			const float freeSpace = maxSize - wantedSize;

			targetMin	= minMin + (freeSpace / 2.0f);
			targetMax	= maxMax + (freeSpace / 2.0f);
			size		= wantedSize;
		}
		else if( elementPositionMin.type == UiPositionType_Auto || elementPositionMax.type == UiPositionType_Auto )
		{
			const float requiredPositionSpace = minValue + maxValue;
			wantedSize = TIKI_MIN( wantedSize, maxSize - requiredPositionSpace );
			wantedSize = TIKI_MAX( wantedSize, 0.0f );

			const float freeSpace = maxMax - wantedSize;
			targetMin	= (elementPositionMin.type == UiPositionType_Auto ? minMin + (freeSpace / 2.0f): minMin + minValue);
			targetMax	= (elementPositionMax.type == UiPositionType_Auto ? maxMax + (freeSpace / 2.0f) : maxMax + maxValue);
			size		= (freeSpace / 2.0f);
		}
		else
		{
			targetMin	= minMin + minValue;
			targetMax	= maxValue - maxValue;
			size		= targetMax - targetMin;
		}

		return size;
	}
}