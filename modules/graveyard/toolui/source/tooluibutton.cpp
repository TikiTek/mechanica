#include "tiki/toolui/tooluibutton.hpp"

#include "tiki/ui/uielement.hpp"
#include "tiki/ui/uisystem.hpp"

#include "tooluivalues.hpp"

namespace tiki
{
	/*static*/ UiElement* ToolUiButton::createButton( UiSystem& uiSystem )
	{
		UiElement* pBorder = uiSystem.addElement();
		pBorder->setToColorRectangleOne( s_border );
		pBorder->registerEventHandler( UiEventType_MouseIn, &ToolUiButton::onMouseIn );
		pBorder->registerEventHandler( UiEventType_MouseOut, &ToolUiButton::onMouseOut );
		pBorder->registerEventHandler( UiEventType_MouseButtonDown, &ToolUiButton::onMouseDown );
		pBorder->registerEventHandler( UiEventType_MouseButtonUp, &ToolUiButton::onMouseUp );

		UiElement* pBackground = uiSystem.addElement( pBorder );
		pBackground->setPosition( UiPosition( UiPositionElement( 1.0f ), UiPositionElement( 1.0f ), UiPositionElement( 1.0f ), UiPositionElement( 1.0f ) ) );
		pBackground->setToColorRectangleOne( s_backgroundDefault );
	
		UiElement* pText = uiSystem.addElement( pBackground );		
		pText->setWidth( UiSize( 10.0f ) );
		pText->setHeight( UiSize( 10.0f ) );
		pText->setToColorRectangleOne( s_backgroundLight );
		
		return pBorder;
	}

	/*static*/ void ToolUiButton::disposeButton( UiSystem& uiSystem, UiElement* pElement )
	{
		uiSystem.removeElement( pElement );
	}

	/*static*/ void ToolUiButton::setText( UiElement* pElement, const char* pText )
	{
		pElement->getFirstChild()->getFirstChild()->setToText( pText, nullptr );
	}

	/*static*/ void ToolUiButton::onMouseIn( UiElement* pElement, const UiEvent& eventData )
	{
		pElement->getFirstChild()->setToColorRectangleOne( s_backgroundLight );
	}

	/*static*/ void ToolUiButton::onMouseOut( UiElement* pElement, const UiEvent& eventData )
	{
		pElement->getFirstChild()->setToColorRectangleOne( s_backgroundDefault );
	}

	/*static*/ void ToolUiButton::onMouseDown( UiElement* pElement, const UiEvent& eventData )
	{
		pElement->getFirstChild()->setToColorRectangleOne( s_backgroundDark );
	}

	/*static*/ void ToolUiButton::onMouseUp( UiElement* pElement, const UiEvent& eventData )
	{
		pElement->getFirstChild()->setToColorRectangleOne( s_backgroundLight );
	}
}