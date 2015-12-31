#include "tiki/uieditor/uieditor.hpp"

#include "tiki/ui/uielement.hpp"
#include "tiki/ui/uisystem.hpp"

namespace tiki
{
	UiEditor::UiEditor()
	{
	}

	UiEditor::~UiEditor()
	{
	}

	void UiEditor::fillToolParameters( ToolApplicationParamters& parameters )
	{

	}

	bool UiEditor::initializeTool()
	{
		UiSystem& uiSystem = getUiSystem();

		UiElement* pElement = uiSystem.addElement();
		pElement->setWidth( UiSize( 100.0f ) );
		pElement->setHeight( UiSize( 100.0f ) );

		return true;
	}

	void UiEditor::shutdownTool()
	{
	}

	void UiEditor::updateTool( bool wantToShutdown )
	{

	}

	void UiEditor::renderTool( GraphicsContext& graphicsContext ) const
	{

	}

	bool UiEditor::processToolInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

	void UiEditor::processToolWindowEvent( const WindowEvent& windowEvent )
	{

	}

	ToolApplication& framework::getTool()
	{
		static UiEditor s_application;
		return s_application;
	}
}