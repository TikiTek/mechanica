#include "tiki/uieditor/uieditor.hpp"

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