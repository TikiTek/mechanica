#include "tiki/toolapplication/tool_ui.hpp"

#include <imgui_internal.h>

ImTextureID ImGui::Tex( const tiki::ToolImage& toolImage )
{
	return (ImTextureID)&toolImage.getData();
}

ImTextureID ImGui::Tex( const tiki::TextureData& textureData )
{
	return (ImTextureID)&textureData;
}

bool ImGui::ImageButtonTextButton( ImTextureID textureId, const char* pText, ImVec2 imageSize /* = ImVec2() */ )
{
	TIKI_ASSERT( textureId != nullptr );

	ImGuiWindow* window = GetCurrentWindow();
	if( window->SkipItems )
	{
		return false;
	}

	ImVec2 size = imageSize;
	if( size.x <= 0.0f && size.y <= 0.0f )
	{
		size.x = ImGui::GetTextLineHeightWithSpacing();
		size.y = ImGui::GetTextLineHeightWithSpacing();
	}
	else
	{
		if( size.x <= 0 )
		{
			size.x = size.y;
		}
		else if( size.y <= 0 )
		{
			size.y = size.x;
		}

		const float scale = window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
		size.x *= scale;
		size.y *= scale;
	}

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID( pText );
	const ImVec2 textSize = ImGui::CalcTextSize( pText, NULL, true );
	const bool hasText = textSize.x > 0;


	float frame_padding = 7;


	const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
	const ImVec2 padding = (frame_padding >= 0) ? ImVec2( (float)frame_padding, (float)frame_padding ) : style.FramePadding;
	const ImVec2 totalSizeWithoutPadding( size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y );

	ImRect bb( window->DC.CursorPos, window->DC.CursorPos );
	bb.Max.x += totalSizeWithoutPadding.x + padding.x * 2;
	bb.Max.y += totalSizeWithoutPadding.y + padding.y * 2;

	ImVec2 start( 0, 0 );
	start = window->DC.CursorPos;
	start.x += padding.x;
	start.y += padding.y;

	if( size.y < textSize.y ) start.y+=(textSize.y - size.y)*.5f;

	ImRect image_bb( start, start );
	image_bb.Max.x += size.x;
	image_bb.Max.y += size.y;

	start = window->DC.CursorPos;
	start.x += padding.x;
	start.y += padding.y;

	start.x += innerSpacing;

	if( size.y > textSize.y ) start.y+=(size.y - textSize.y)*.5f;
	ItemSize( bb );
	if( !ItemAdd( bb, id ) )
		return false;

	bool hovered=false, held=false;
	bool pressed = ButtonBehavior( bb, id, &hovered, &held );

	// Render
	const ImU32 col = GetColorU32( (hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button );
	RenderFrame( bb.Min, bb.Max, col, true, ImClamp( (float)ImMin( padding.x, padding.y ), 0.0f, style.FrameRounding ) );
	//if( bg_col.w > 0.0f )
	//	window->DrawList->AddRectFilled( image_bb.Min, image_bb.Max, GetColorU32( bg_col ) );

	window->DrawList->AddImage( textureId, image_bb.Min, image_bb.Max );

	if( textSize.x > 0 ) ImGui::RenderText( start, pText );
	return pressed;
}