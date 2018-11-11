#pragma once

#include <imgui.h>

namespace ImGui
{
	ImTextureID	Tex( const tiki::ToolImage& toolImage );
	ImTextureID	Tex( const tiki::TextureData& textureData );

	ImVec2		Vec2( const tiki::Vector2& vec2 );

	bool		ImageButtonTextButton( ImTextureID textureId, const char* pText, ImVec2 imageSize = ImVec2() );
}