#include "tiki/uieditor/application.hpp"

#include "tiki/ui/uielement.hpp"
#include "tiki/ui/uisystem.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/toolui/tooluibutton.hpp"

namespace tiki
{
	Application::Application()
	{
		m_pTestElement = nullptr;
	}

	Application::~Application()
	{
	}

	void Application::fillToolParameters( ToolApplicationParamters& parameters )
	{
		parameters.pGamebuildPath = "../../../../../../gamebuild";
	}

	bool Application::initializeTool()
	{
		UiSystem& ui = getUiSystem();

		m_pTestElement = ToolUiButton::createButton( ui );
		m_pTestElement->setWidth( UiSize( 100.0f ) );
		m_pTestElement->setHeight( UiSize( 100.0f ) );

		//m_pTestElement = ui.addElement();
		//m_pTestElement->setToColorRectangleOne( TIKI_COLOR_TRANSPARENT );

		//UiElement* pTopLeft = ui.addElement( m_pTestElement );
		//pTopLeft->setPosition( UiPosition( UiPositionElement( 0.0f ), UiPositionElement( 0.0f ), UiPositionElement(), UiPositionElement() ) );
		//pTopLeft->setWidth( UiSize( 5.0f ) );
		//pTopLeft->setHeight( UiSize( 5.0f ) );
		//pTopLeft->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN );

		//UiElement* pTop = ui.addElement( m_pTestElement );
		//pTop->setPosition( UiPosition( UiPositionElement( 5.0f ), UiPositionElement( 0.0f ), UiPositionElement(), UiPositionElement( 5.0f ) ) );
		//pTop->setHeight( UiSize( 5.0f ) );
		//pTop->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN, TIKI_COLOR_GREEN );

		//UiElement* pTopRight = ui.addElement( m_pTestElement );
		//pTopRight->setPosition( UiPosition( UiPositionElement(), UiPositionElement( 0.0f ), UiPositionElement(), UiPositionElement( 0.0f ) ) );
		//pTopRight->setWidth( UiSize( 5.0f ) );
		//pTopRight->setHeight( UiSize( 5.0f ) );
		//pTopRight->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT );

		//UiElement* pLeft = ui.addElement( m_pTestElement );
		//pLeft->setPosition( UiPosition( UiPositionElement( 0.0f ), UiPositionElement( 5.0f ), UiPositionElement( 5.0f ), UiPositionElement() ) );
		//pLeft->setWidth( UiSize( 5.0f ) );
		//pLeft->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN );

		//UiElement* pRight = ui.addElement( m_pTestElement );
		//pRight->setPosition( UiPosition( UiPositionElement(), UiPositionElement( 5.0f ), UiPositionElement( 5.0f ), UiPositionElement( 0.0f ) ) );
		//pRight->setWidth( UiSize( 5.0f ) );
		//pRight->setToColorRectangle( TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT );

		//UiElement* pBottomLeft = ui.addElement( m_pTestElement );
		//pBottomLeft->setPosition( UiPosition( UiPositionElement( 0.0f ), UiPositionElement(), UiPositionElement( 0.0f ), UiPositionElement() ) );
		//pBottomLeft->setWidth( UiSize( 5.0f ) );
		//pBottomLeft->setHeight( UiSize( 5.0f ) );
		//pBottomLeft->setToColorRectangle( TIKI_COLOR_TRANSPARENT, TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT );

		//UiElement* pBottom = ui.addElement( m_pTestElement );
		//pBottom->setPosition( UiPosition( UiPositionElement( 5.0f ), UiPositionElement(), UiPositionElement( 0.0f ), UiPositionElement( 5.0f ) ) );
		//pBottom->setHeight( UiSize( 5.0f ) );
		//pBottom->setToColorRectangle( TIKI_COLOR_GREEN, TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT );

		//UiElement* pBottomRight = ui.addElement( m_pTestElement );
		//pBottomRight->setPosition( UiPosition( UiPositionElement(), UiPositionElement(), UiPositionElement( 0.0f ), UiPositionElement( 0.0f ) ) );
		//pBottomRight->setWidth( UiSize( 5.0f ) );
		//pBottomRight->setHeight( UiSize( 5.0f ) );
		//pBottomRight->setToColorRectangle( TIKI_COLOR_GREEN, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT, TIKI_COLOR_TRANSPARENT );

		//UiElement* pFill = ui.addElement( m_pTestElement );
		//pFill->setPosition( UiPosition( UiPositionElement( 5.0f ), UiPositionElement( 5.0f ), UiPositionElement( 5.0f ), UiPositionElement( 5.0f ) ) );
		//pFill->setToColorRectangleOne( TIKI_COLOR_GREEN );
		//pFill->registerEventHandler( UiEventType_MouseOver, handleMouseOverEvent );

		//const char* pCode = ""
		//	"local element = uiSystem:addElement();"
		//	"element:setWidth( 10 )"
		//	"element:setHeight( 10 )";

		//getUiSystem().getScriptContext().executeScript( pCode );

		return true;
	}

	void Application::shutdownTool()
	{
		getUiSystem().removeElement( m_pTestElement );
	}

	void Application::updateTool( bool wantToShutdown )
	{

	}

	void Application::renderTool( GraphicsContext& graphicsContext ) const
	{
		graphicsContext.clear( graphicsContext.getBackBuffer() );
	}

	bool Application::processToolInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

	void Application::processToolWindowEvent( const WindowEvent& windowEvent )
	{

	}

	void Application::handleMouseOverEvent( UiElement* pSender, const UiEvent& eventData )
	{
		pSender->setToColorRectangleOne( TIKI_COLOR_BLUE );
	}

	ToolApplication& framework::getTool()
	{
		static Application s_application;
		return s_application;
	}
}