#include "tiki/toolapplication/tool_message_box.hpp"

#include <imgui.h>

#include "res_toolapplication.hpp"

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
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_apMessageBoxButtonTexts ) == ToolMessageBoxButton_Count );

	ToolMessageBox::ToolMessageBox()
		: m_informationIcon( getToolapplicationResource( ToolapplicationResources_DialogInformation ) )
		, m_questionIcon( getToolapplicationResource( ToolapplicationResources_DialogQuestion ) )
		, m_warningIcon( getToolapplicationResource( ToolapplicationResources_DialogWarning ) )
		, m_errorIcon( getToolapplicationResource( ToolapplicationResources_DialogError ) )
	{
		m_icon		= ToolMessageBoxIcon_None;
		m_buttons	= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Ok );
	}

	void ToolMessageBox::open( const DynamicString& title, const DynamicString& message, ToolMessageBoxButtonFlagMask buttons /*= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Ok )*/, ToolMessageBoxIcon icon /*= ToolMessageBoxIcon_None */ )
	{
		TIKI_ASSERT( buttons.isAnyFlagSet() );

		m_title		= title;
		m_message	= message;
		m_icon		= icon;
		m_buttons	= buttons;

		ImGui::OpenPopup( m_title.cStr() );
	}

	void ToolMessageBox::doUi()
	{
		if( ImGui::BeginPopupModal( m_title.cStr(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ) )
		{
			//ImGui::SetCursorPosX( ... );

			ImGui::Text( m_message.cStr() );

			for( uint i = 0u; i < ToolMessageBoxButton_Count; ++i )
			{
				const ToolMessageBoxButton button = (ToolMessageBoxButton)i;
				if( !m_buttons.isFlagSet( button ) )
				{
					continue;
				}

				if( ImGui::Button( s_apMessageBoxButtonTexts[ i ] ) )
				{
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}
}