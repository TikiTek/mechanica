#include "tiki/ui/uisystem.hpp"

#include "uitypes_private.hpp"

namespace tiki
{
	UiSystem::UiSystem()
	{
	}

	UiSystem::~UiSystem()
	{
	}

	bool UiSystem::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiSystemParameters& parameters )
	{
		if( !m_renderer.create( graphicsSystem, resourceManager, parameters.rendererParameters ) ||
			!m_elementPool.create( parameters.maxElementCount ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void UiSystem::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		for( UiElement& element : m_elements )
		{
			removeElement( &element );
		}

		m_elementPool.dispose();
		
		m_renderer.dispose( graphicsSystem, resourceManager );
	}

	UiElement* UiSystem::addElement( UiElement* pParent /*= nullptr */ )
	{
		UiElement& element = m_elementPool.push();
		element.create( pParent );

		if( pParent == nullptr )
		{
			m_elements.push( element );
		}

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

		if( pElement->m_pParent == nullptr )
		{
			m_elements.removeSortedByValue( *pElement );
		}

		m_elementPool.removeUnsortedByValue( *pElement );
	}

	void UiSystem::update()
	{
		updateLayout();

		UiRenderData renderData;

		m_renderer.update( renderData );
	}

	void UiSystem::render( GraphicsContext& context ) const
	{
		m_renderer.render( context );
	}

	bool UiSystem::processInputEvent( InputEvent& inputEvent )
	{
		return false;
	}

	void UiSystem::updateLayout()
	{
		for( UiElement& element : m_elements )
		{
			if( !element.m_layoutChanged )
			{
				continue;
			}

		}
	}

}