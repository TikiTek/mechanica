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
		UiSystem& ui = getUiSystem();

		m_pTestElement = ui.addElement();
		m_pTestElement->setWidth( UiSize( 100.0f ) );
		m_pTestElement->setHeight( UiSize( 100.0f ) );
		m_pTestElement->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT );

		UiElement* pLeft = ui.addElement( m_pTestElement );
		pLeft->setPosition( UiPosition( UiPositionElement( 0.0f ), UiPositionElement( 0.0f ) ) );
		pLeft->setWidth( UiSize( 10.0f ) );
		pLeft->setHeight( UiSize( 10.0f ) );
		pLeft->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN );

		UiElement* pRight = ui.addElement( m_pTestElement );
		pRight->setPosition( UiPosition( UiPositionElement(), UiPositionElement( 0.0f ), UiPositionElement(), UiPositionElement( 0.0f ) ) );
		pRight->setWidth( UiSize( 10.0f ) );
		pRight->setHeight( UiSize( 10.0f ) );
		pRight->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT );

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