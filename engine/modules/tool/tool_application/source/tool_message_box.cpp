#include "tiki/toolapplication/tool_message_box.hpp"

#include "tiki/toolapplication/tool_ui.hpp"

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
		m_open		= false;
		m_icon		= ToolMessageBoxIcon_None;
		m_buttons	= ToolMessageBoxButtonFlagMask( ToolMessageBoxButton_Ok );
	}

	void ToolMessageBox::open( const DynamicString& title, const DynamicString& message, ToolMessageBoxButtonFlagMask buttons /* = ToolMessageBoxButtons_Ok */, ToolMessageBoxIcon icon /* = ToolMessageBoxIcon_None */, ToolMessageBoxCallbackDelegate callback /* = ToolMessageBoxCallbackDelegate() */, UserData userData /* = UserData() */ )
	{
		TIKI_ASSERT( buttons.isAnyFlagSet() );

		if( m_open )
		{
			TIKI_TRACE_ERROR( "[tool] Can not open another message box while the old one is still open.\n" );
			return;
		}

		m_title		= title;
		m_message	= message;
		m_icon		= icon;
		m_buttons	= buttons;
		m_open		= true;
		m_callback	= callback;
		m_userData	= userData;

		ImGui::OpenPopup( m_title.cStr() );
	}

	void ToolMessageBox::doUi()
	{
		if( !m_open )
		{
			return;
		}

		if( ImGui::BeginPopupModal( m_title.cStr(), &m_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ) )
		{
			ToolImage* pImage = nullptr;
			switch( m_icon )
			{
			case ToolMessageBoxIcon_Information:
				pImage = &m_informationIcon;
				break;

			case ToolMessageBoxIcon_Question:
				pImage = &m_questionIcon;
				break;

			case ToolMessageBoxIcon_Warning:
				pImage = &m_warningIcon;
				break;

			case ToolMessageBoxIcon_Error:
				pImage = &m_errorIcon;
				break;

			default:
				break;
			}

			if( pImage != nullptr )
			{
				//ImGui::SetCursorPosX(  );
				ImGui::Image( ImGui::Tex( *pImage ), ImGui::Vec2( pImage->getSize() ) );
				ImGui::SameLine();
			}

			ImGui::Text( m_message.cStr() );
			ImGui::GetItemRectSize();

			for( uint i = 0u; i < ToolMessageBoxButton_Count; ++i )
			{
				const ToolMessageBoxButton button = (ToolMessageBoxButton)i;
				if( !m_buttons.isFlagSet( button ) )
				{
					continue;
				}

				if( ImGui::Button( s_apMessageBoxButtonTexts[ i ] ) )
				{
					m_open = false;
					m_callback.invoke( button, m_userData );
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
			}

			ImGui::EndPopup();
		}
	}
}