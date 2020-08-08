#include "tiki/ui/uisystem.hpp"

#include "tiki/input/inputevent.hpp"

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
			!m_elementPool.create( parameters.maxElementCount ) ||
			!m_eventHandlerPool.create( parameters.maxEventHandlerCount ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		m_pRootElement = &m_elementPool.push();
		m_pRootElement->create( this, nullptr, TIKI_INVALID_CRC32 );
		m_pRootElement->setToColorRectangle( TIKI_COLOR_TRANSPARENT );

		setScreenSize( (float)parameters.width, (float)parameters.height );

		return true;
	}

	void UiSystem::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		removeElement( m_pRootElement );
		m_pRootElement = nullptr;

		m_eventHandlerPool.dispose();
		m_elementPool.dispose();
		
		m_renderer.dispose( graphicsSystem, resourceManager );

		m_scriptUiElementClass.dispose();
		m_scriptUiSystemClass.dispose();
		m_scriptContext.dispose();
	}

	UiElement* UiSystem::addElement( UiElement* pParent /* = nullptr */, crc32 elementTypeCrc /* = TIKI_INVALID_CRC32 */ )
	{
		UiElement& element = m_elementPool.push();
		element.create( this, !pParent ? m_pRootElement : pParent, elementTypeCrc );

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

		m_prevInputState = m_currentInputState;
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
		switch( inputEvent.eventType )
		{
		case InputEventType_Mouse_Moved:
			{
				m_currentInputState.mousePosition =  vector::create( float( inputEvent.data.mouseMoved.xState ), float( inputEvent.data.mouseMoved.yState ) );

				bool result = false;
				for( UiElement& child : m_pRootElement->m_children )
				{
					result |= child.checkMouseMoveEvent( m_currentInputState );
				}

				return result;
			}
			
		case InputEventType_Controller_ButtonDown:
		case InputEventType_Controller_ButtonUp:
			{
				m_currentInputState.mouseButtonState.state[ inputEvent.data.mouseButton.button ] = (inputEvent.eventType == InputEventType_Controller_ButtonDown);

				bool result = false;
				for( UiElement& child : m_pRootElement->m_children )
				{
					result |= child.checkMouseClickEvent( m_prevInputState, m_currentInputState );
				}

				return result;
			}

		default:
			break;
		}

		return false;
	}

	UiEventHandler* UiSystem::allocateEventHandler()
	{
		if( m_eventHandlerPool.isFull() )
		{
			return nullptr;
		}

		return &m_eventHandlerPool.push();
	}

	void UiSystem::freeEventHandler( UiEventHandler& eventHandler )
	{
		m_eventHandlerPool.removeUnsortedByValue( eventHandler );
	}
}