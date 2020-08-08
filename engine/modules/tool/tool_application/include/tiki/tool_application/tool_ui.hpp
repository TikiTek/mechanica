#pragma once

#include <imgui.h>

struct ImHorizontalSplitter
{
	float		Top;
	float		Bottom;
	float		Right;
	ImU32		ElementCount;
	float		LastElementRight;
	float		LastElementLeft;
	float*		pLastElementWidth;
};

namespace ImGui
{
	ImTextureID		Tex( const tiki::ToolImage& toolImage );
	ImTextureID		Tex( const tiki::TextureData& textureData );

	ImVec2			Vec2( const tiki::Vector2& vec2 );

	tiki::Vector2	Vec2( const ImVec2& vec2 );

	bool			ImageButtonTextButton( ImTextureID textureId, const char* pText, ImVec2 imageSize = ImVec2() );

	void			BeginHorizontalSplitter( ImHorizontalSplitter& splitter, float left, float top, float right, float bottom );
	bool			BeginHorizontalSplitterElement( ImHorizontalSplitter& splitter, const char* pName, float* pElementWidth );
	void			EndHorizontalSplitterElement( ImHorizontalSplitter& splitter );
	void			EndHorizontalSplitter( ImHorizontalSplitter& splitter );
}