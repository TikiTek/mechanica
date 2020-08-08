#include "tiki/tool_application/tool_message_box.hpp"

#include "tiki/tool_application/tool_ui.hpp"

#include <imgui.h>

#include "res_tool_application.hpp"

namespace tiki
{
	static const char* s_apMessageBoxButtonTexts[] =
	{
		"OK",
		"Yes",
		"No",
		"Abort",
		"Retry",
		"Ignore",
		"Cancel"
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_apMessageBoxButtonTexts ) == (uintreg)ToolMessageBoxButton::Count );

	ToolMessageBox::ToolMessageBox()
		: m_informationIcon( getToolApplicationResource( ToolApplicationResources_DialogInformation ) )
		, m_questionIcon( getToolApplicationResource( ToolApplicationResources_DialogQuestion ) )
		, m_warningIcon( getToolApplicationResource( ToolApplicationResources_DialogWarning ) )
		, m_errorIcon( getToolApplicationResource( ToolApplicationResources_DialogError ) )
	{
	}

	void ToolMessageBox::open( const DynamicString& title, const DynamicString& text, ToolMessageBoxButtonFlagMask buttons /* = ToolMessageBoxButtons_Ok */, ToolMessageBoxIcon icon /* = ToolMessageBoxIcon::None */, ToolMessageBoxCallbackDelegate callback /* = ToolMessageBoxCallbackDelegate() */, UserData userData /* = UserData() */ )
	{
		TIKI_ASSERT( buttons.isAnyFlagSet() );

		Message& message = m_messages.pushBack();
		message.title		= title;
		message.text		= text;
		message.icon		= icon;
		message.buttons		= buttons;
		message.callback	= callback;
		message.userData	= userData;

		ImGui::OpenPopup( title.cStr() );
	}

	void ToolMessageBox::doUi()
	{
		if( m_messages.isEmpty() )
		{
			return;
		}

		const Message& message = m_messages.getFront();
		if( !ImGui::BeginPopupModal( message.title.cStr(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ) )
		{
			ImGui::OpenPopup( message.title.cStr() );
			return;
		}

		ToolImage* pImage = nullptr;
		switch( message.icon )
		{
		case ToolMessageBoxIcon::None:
			break;

		case ToolMessageBoxIcon::Information:
			pImage = &m_informationIcon;
			break;

		case ToolMessageBoxIcon::Question:
			pImage = &m_questionIcon;
			break;

		case ToolMessageBoxIcon::Warning:
			pImage = &m_warningIcon;
			break;

		case ToolMessageBoxIcon::Error:
			pImage = &m_errorIcon;
			break;
		}

		if( pImage != nullptr )
		{
			ImGui::Image( ImGui::Tex( *pImage ), ImGui::Vec2( pImage->getSize() ) );
			ImGui::SameLine();
		}

		ImGui::Text( message.text.cStr() );

		for( uint i = 0u; i < (uintreg)ToolMessageBoxButton::Count; ++i )
		{
			const ToolMessageBoxButton button = (ToolMessageBoxButton)i;
			if( !message.buttons.isFlagSet( button ) )
			{
				continue;
			}

			if( ImGui::Button( s_apMessageBoxButtonTexts[ i ] ) )
			{
				message.callback.invoke( button, message.userData );

				m_messages.popFront();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
		}

		ImGui::EndPopup();
	}
}