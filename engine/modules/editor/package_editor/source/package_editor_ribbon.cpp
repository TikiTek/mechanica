#include "tiki/package_editor/package_editor_ribbon.hpp"

#include "tiki/tool_application/tool_ui.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

#include "res_package_editor.hpp"

#include <imgui.h>

namespace tiki
{
	static const char* s_pPopupNewPackage = "New Package";
	static const char* s_pPopupOpenPackage = "Open Package";

	PackageEditorRibbon::PackageEditorRibbon( EditorInterface* pInterface, PackageEditor& packageEditor )
		: EditorRibbon( "Package" )
		, m_pInterface( pInterface )
		, m_packageEditor( packageEditor )
		, m_newPackageIcon( getPackageEditorResource( PackageEditorResources_RibbonPackageNew ) )
		, m_openPackageIcon( getPackageEditorResource( PackageEditorResources_RibbonPackageOpen ) )
		, m_closePackageIcon( getPackageEditorResource( PackageEditorResources_RibbonPackageClose ) )
		, m_editPackageIcon( getPackageEditorResource( PackageEditorResources_RibbonPackageEdit ) )
	{
	}

	PackageEditorRibbon::~PackageEditorRibbon()
	{
	}

	void PackageEditorRibbon::doUi()
	{
		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_newPackageIcon ), "New" ) )
		{
			ImGui::OpenPopup( s_pPopupNewPackage );
		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_openPackageIcon ), "Open" ) )
		{
			ImGui::OpenPopup( s_pPopupOpenPackage );
		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_closePackageIcon ), "Close" ) )
		{
			m_packageEditor.closePackage();
		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_editPackageIcon ), "Edit" ) )
		{
			m_packageEditor.editPackage();
		}
		ImGui::SameLine();

		doUiPopups();
	}

	void PackageEditorRibbon::doUiPopups()
	{
		if( ImGui::BeginPopupModal( s_pPopupNewPackage, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ) )
		{
			char buffer[ 128u ];
			copyString( buffer, sizeof( buffer ), m_packageName.cStr() );
			ImGui::InputText( "Name:", buffer, sizeof( buffer ), ImGuiInputTextFlags_CharsNoBlank );
			m_packageName = buffer;

			if( ImGui::Button( "Ok" ) )
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if( ImGui::Button( "Cancel" ) )
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if( ImGui::BeginPopupModal( s_pPopupOpenPackage, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ) )
		{
			for( const Package& package : m_pInterface->getProject().getPackages() )
			{
				if( ImGui::Button( package.getName().cStr() ) )
				{
					m_packageEditor.openPackage( const_cast< Package& >( package ) );
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}
}
