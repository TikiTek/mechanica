#include "tiki/uieditor/uieditor.hpp"

#include "tiki/ui/uielement.hpp"
#include "tiki/ui/uisystem.hpp"
#include "tiki/graphics/graphicscontext.hpp"

namespace tiki
{
	UiEditor::UiEditor()
	{
		m_pTestElement = nullptr;
	}

	UiEditor::~UiEditor()
	{
	}

	void UiEditor::fillToolParameters( ToolApplicationParamters& parameters )
	{
		parameters.pGamebuildPath = "../../../../../../gamebuild";
	}

	bool UiEditor::initializeTool()
	{
		m_pTestElement = getUiSystem().addElement();
		m_pTestElement->setWidth( UiSize( 100.0f ) );
		m_pTestElement->setHeight( UiSize( 100.0f ) );
		m_pTestElement->setToColorRectangle( TIKI_COLOR_GREEN );

		return true;
	}

	void UiEditor::shutdownTool()
	{
		getUiSystem().removeElement( m_pTestElement );
	}

	void UiEditor::updateTool( bool wantToShutdown )
	{

	}

	void UiEditor::renderTool( GraphicsContext& graphicsContext ) const
	{
		graphicsContext.clear( graphicsContext.getBackBuffer() );
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