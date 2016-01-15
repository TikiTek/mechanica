#include "tiki/ui/uisystem.hpp"

#include "uitypes_private.hpp"

namespace tiki
{
	UiSystem::UiSystem()
	{
		m_pRootElement = nullptr;
	}

	UiSystem::~UiSystem()
	{
		TIKI_ASSERT( m_pRootElement == nullptr );
	}

	bool UiSystem::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiSystemParameters& parameters )
	{
		UiRendererParameters rendererParameters;
		rendererParameters.maxRenderElements = parameters.maxElementCount;

		if( !m_renderer.create( graphicsSystem, resourceManager, rendererParameters ) ||
			!m_elementPool.create( parameters.maxElementCount ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		m_pRootElement = &m_elementPool.push();
		m_pRootElement->create( nullptr );
		m_pRootElement->setToColorRectangle( TIKI_COLOR_TRANSPARENT );
		m_pRootElement->setWidth( UiSize( (float)parameters.width ) );
		m_pRootElement->setHeight( UiSize( (float)parameters.height ) );

		return true;
	}

	void UiSystem::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		removeElement( m_pRootElement );
		m_pRootElement = nullptr;

		m_elementPool.dispose();
		
		m_renderer.dispose( graphicsSystem, resourceManager );
	}

	UiElement* UiSystem::addElement( UiElement* pParent /*= nullptr */ )
	{
		UiElement& element = m_elementPool.push();
		element.create( !pParent ? m_pRootElement : pParent );

		return &element;
	}

	void UiSystem::removeElement( UiElement* pElement )
	{
		if( pElement == nullptr )
		{
			return;
		}

		for( UiElement& child : pElement->m_children )
		{
			removeElement( &child );
		}
		pElement->dispose();

		m_elementPool.removeUnsortedByValue( *pElement );
	}

	void UiSystem::update()
	{
		updateElementLayout( *m_pRootElement );

		UiRenderData renderData( m_pRootElement->m_children );
		m_renderer.update( renderData );
	}

	void UiSystem::render( GraphicsContext& context, const RenderTarget& renderTarget ) const
	{
		m_renderer.render( context, renderTarget );
	}

	bool UiSystem::processInputEvent( InputEvent& inputEvent )
	{
		return false;
	}

	void UiSystem::updateElementLayout( UiElement& element )
	{
		if( !element.m_layoutChanged )
		{
			return;
		}

		UiRectangle parentBounds;
		if( element.m_pParent )
		{
			parentBounds = element.m_pParent->m_boundingRectangle;
		}
		else
		{
			parentBounds = element.m_boundingRectangle;
		}
		element.m_boundingRectangle = parentBounds;
		
		UiRectangle childBounds;
		for( UiElement& child : element.m_children )
		{
			updateElementLayout( child );
			childBounds.extend( child.m_boundingRectangle );
		}

		element.m_layoutSize.x	= getElementLayoutSize( element, element.m_width, parentBounds.getWidth(), childBounds.getWidth() );
		element.m_layoutSize.y	= getElementLayoutSize( element, element.m_height, parentBounds.getHeight(), childBounds.getHeight() );

		element.m_boundingRectangle.right	= element.m_boundingRectangle.left + element.m_layoutSize.x;
		element.m_boundingRectangle.bottom	= element.m_boundingRectangle.top + element.m_layoutSize.y;

		element.m_layoutChanged = false;
	}

	float UiSystem::getElementLayoutSize( UiElement& element, const UiSize& elementSize, float parentSize, float childSize )
	{
		switch( elementSize.type )
		{
		case UiSizeType_Auto:
			return childSize;

		case UiSizeType_Expand:
			return parentSize;

		case UiSizeType_Meters:
			TIKI_ASSERT( false );
			// TODO: get DPI
			return elementSize.value * 0.22f;

		case UiSizeType_Percent:
			return parentSize * (elementSize.value / 100.0f);

		case UiSizeType_Pixel:
			return elementSize.value;

		default:
			TIKI_BREAK( "Size type not supported" );
			break;
		}

		return 0.0f;
	}
}