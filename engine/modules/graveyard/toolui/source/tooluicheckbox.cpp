#include "tiki/toolui/tooluicheckbox.hpp"

#include "tiki/ui/uielement.hpp"
#include "tiki/ui/uisystem.hpp"

#include "tooluivalues.hpp"

namespace tiki
{
	/*static*/ UiElement* ToolUiCheckBox::createCheckBox( UiSystem& uiSystem )
	{
		UiElement* pBackground = uiSystem.addElement();
		pBackground->registerEventHandler( UiEventType_MouseIn, &ToolUiCheckBox::onMouseIn );
		pBackground->registerEventHandler( UiEventType_MouseOut, &ToolUiCheckBox::onMouseOut );
		pBackground->registerEventHandler( UiEventType_MouseButtonDown, &ToolUiCheckBox::onMouseDown );
		pBackground->registerEventHandler( UiEventType_MouseButtonUp, &ToolUiCheckBox::onMouseUp );

		UiElement* pCheckBorder = uiSystem.addElement( pBackground );
		pCheckBorder->setWidth( UiSize( 12.0f ) );
		pCheckBorder->setHeight( UiSize( 12.0f ) );
		pCheckBorder->setToColorRectangleOne( s_border );

		UiElement* pCheckBackground = uiSystem.addElement( pCheckBorder );
		pCheckBackground->setPosition( UiPosition( UiPositionElement( 1.0f ), UiPositionElement( 1.0f ), UiPositionElement( 1.0f ), UiPositionElement( 1.0f ) ) );
		pCheckBackground->setToColorRectangleOne( s_backgroundDefault );

		UiElement* pCheck = uiSystem.addElement( pCheckBackground );
		pCheck->setPosition( UiPosition( UiPositionElement( 3.0f ), UiPositionElement( 3.0f ), UiPositionElement( 3.0f ), UiPositionElement( 3.0f ) ) );
		pCheck->setToColorRectangleOne( s_backgroundLight );

		UiElement* pText = uiSystem.addElement( pBackground );
		pCheckBackground->setPosition( UiPosition( UiPositionElement( 15.0f ) ) );
		pText->setWidth( UiSize( 10.0f ) );
		pText->setHeight( UiSize( 10.0f ) );
		pText->setToColorRectangleOne( s_backgroundLight );

		return pBackground;

	}

	/*static*/ void ToolUiCheckBox::disposeCheckBox( UiSystem& uiSystem, UiElement* pElement )
	{

	}

	/*static*/ bool ToolUiCheckBox::isChecked( UiElement* pElement )
	{
		return pElement->getFirstChild()->getFirstChild()->getFirstChild() != nullptr;
	}

	/*static*/ void ToolUiCheckBox::setChecked( UiElement* pElement, bool checked )
	{

	}

	/*static*/ void ToolUiCheckBox::setText( UiElement* pElement, const char* pText )
	{

	}

	/*static*/ void ToolUiCheckBox::onMouseIn( UiElement* pElement, const UiEvent& eventData )
	{

	}

	/*static*/ void ToolUiCheckBox::onMouseOut( UiElement* pElement, const UiEvent& eventData )
	{

	}

	/*static*/ void ToolUiCheckBox::onMouseDown( UiElement* pElement, const UiEvent& eventData )
	{

	}

	/*static*/ void ToolUiCheckBox::onMouseUp( UiElement* pElement, const UiEvent& eventData )
	{

	}
}
