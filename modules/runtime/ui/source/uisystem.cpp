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
		TIKI_TRACE_INFO( "[ui] System: %p.\n", this );

		if( !m_scriptContext.create() )
		{
			return false;
		}

		m_scriptContext.addExtension( ScriptExtensions_Base );
		m_scriptContext.addExtension( ScriptExtensions_Math );
		m_scriptContext.addExtension( ScriptExtensions_Objects );
		m_scriptContext.addExtension( ScriptExtensions_String );

		if( !m_scriptUiSystemClass.create( m_scriptContext, *this, m_scriptUiElementClass ) ||
			!m_scriptUiElementClass.create( m_scriptContext, *this ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		UiRendererParameters rendererParameters;
		rendererParameters.maxRenderElements = parameters.maxElementCount;
		
		if( !m_renderer.create( graphicsSystem, resourceManager, rendererParameters ) ||
			!m_elementPool.create( parameters.maxElementCount ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		m_pRootElement = &m_elementPool.push();
		m_pRootElement->create( this, nullptr );
		m_pRootElement->setToColorRectangle( TIKI_COLOR_TRANSPARENT );

		setScreenSize( (float)parameters.width, (float)parameters.height );

		return true;
	}

	void UiSystem::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		removeElement( m_pRootElement );
		m_pRootElement = nullptr;

		m_elementPool.dispose();
		
		m_renderer.dispose( graphicsSystem, resourceManager );

		m_scriptUiElementClass.dispose();
		m_scriptUiSystemClass.dispose();
		m_scriptContext.dispose();
	}

	UiElement* UiSystem::addElement( UiElement* pParent /*= nullptr */ )
	{
		UiElement& element = m_elementPool.push();
		element.create( this, !pParent ? m_pRootElement : pParent );

		return &element;
	}

	void UiSystem::removeElement( UiElement* pElement )
	{
		if( pElement == nullptr )
		{
			return;
		}

		while( !pElement->m_children.isEmpty() )
		{
			removeElement( &*pElement->m_children.getBegin() );
		}
		pElement->dispose();

		m_elementPool.removeUnsortedByValue( *pElement );
	}

	void UiSystem::update()
	{
		UiLayoutContext context;
		context.meterToPixel = 1.0f;

		m_pRootElement->updateLayout( context );

		UiRenderData renderData( m_pRootElement->m_children );
		m_renderer.update( renderData );
	}

	void UiSystem::render( GraphicsContext& context, const RenderTarget& renderTarget ) const
	{
		m_renderer.render( context, renderTarget );
	}

	ScriptContext& UiSystem::getScriptContext()
	{
		return m_scriptContext;
	}

	void UiSystem::setScreenSize( float width, float height )
	{
		m_pRootElement->setWidth( UiSize( width ) );
		m_pRootElement->setHeight( UiSize( height ) );

		m_renderer.setScreenSize( width, height );
	}

	bool UiSystem::processInputEvent( InputEvent& inputEvent )
	{
		return false;
	}

	UiEventHandler* UiSystem::allocateEventHandler()
	{
		if( m_eventHandlers.isFull() )
		{
			return nullptr;
		}

		return &m_eventHandlers.push();
	}

	void UiSystem::freeEventHandler( UiEventHandler& eventHandler )
	{
		m_eventHandlers.removeUnsortedByValue( eventHandler );
	}
}