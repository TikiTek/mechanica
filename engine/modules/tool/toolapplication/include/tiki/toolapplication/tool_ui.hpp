#pragma once

#include <imgui.h>

namespace ImGui
{
	ImTextureID	Tex( const tiki::ToolImage& toolImage );
	ImTextureID	Tex( const tiki::TextureData& textureData );

	bool		ImageButtonTextButton( ImTextureID textureId, const char* pText, ImVec2 imageSize = ImVec2() );
}