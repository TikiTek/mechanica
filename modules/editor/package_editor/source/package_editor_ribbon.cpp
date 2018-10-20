#include "tiki/package_editor/package_editor_ribbon.hpp"

#include "tiki/toolapplication/tool_ui.hpp"

#include "res_package_editor.hpp"

#include <imgui.h>

namespace tiki
{
	PackageEditorRibbon::PackageEditorRibbon()
		: EditorRibbon( "Package" )
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

		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_openPackageIcon ), "Open" ) )
		{

		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_closePackageIcon ), "Close" ) )
		{

		}
		ImGui::SameLine();

		if( ImGui::ImageButtonTextButton( ImGui::Tex( m_editPackageIcon ), "Edit" ) )
		{

		}
		ImGui::SameLine();
	}
}
