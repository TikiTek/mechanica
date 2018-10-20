#include "tiki/toolapplication/tool_ui.hpp"

#include "tiki/graphics/texturedata.hpp"

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

	const tiki::TextureData* pTexture = (const tiki::TextureData*)textureId;
	if( imageSize.x <= 0.0f && imageSize.y <= 0.0f && pTexture != nullptr )
	{
		imageSize.x = float( pTexture->getWidth() );
		imageSize.y = float( pTexture->getHeight() );
	}
	else
	{
		const float scale = window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
		imageSize.x *= scale;
		imageSize.y *= scale;
	}

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID( pText );
	const ImVec2 textSize = ImGui::CalcTextSize( pText, NULL, true );
	const ImVec2 padding = style.FramePadding;
	const ImVec2 totalSizeWithoutPadding(
		TIKI_MAX( imageSize.x, textSize.x ),
		imageSize.y + style.ItemInnerSpacing.y + textSize.y
	);

	ImRect buttonBox( window->DC.CursorPos, window->DC.CursorPos );
	buttonBox.Max.x += totalSizeWithoutPadding.x + (padding.x * 2.0f);
	buttonBox.Max.y += totalSizeWithoutPadding.y + (padding.y * 2.0f);

	const float buttonCenterX = buttonBox.Min.x + (buttonBox.GetWidth() / 2.0f);
	ImRect imageBox;
	imageBox.Min.x = buttonCenterX - (imageSize.x / 2.0f);
	imageBox.Max.x = buttonCenterX + (imageSize.x / 2.0f);
	imageBox.Min.y = buttonBox.Min.y + padding.y;
	imageBox.Max.y = imageBox.Min.y + imageSize.y;

	ImVec2 textStart = buttonBox.GetTL();
	textStart.x += style.FramePadding.x;
	textStart.y += style.FramePadding.y + imageSize.y + style.ItemInnerSpacing.y;

	ItemSize( buttonBox );
	if( !ItemAdd( buttonBox, id ) )
	{
		return false;
	}

	bool hovered = false;
	bool held = false;
	const bool pressed = ButtonBehavior( buttonBox, id, &hovered, &held );
	const ImGuiCol colorType = (hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button;
	const ImU32 color = GetColorU32( colorType );

	RenderFrame( buttonBox.Min, buttonBox.Max, color, true, ImClamp( ImMin( style.FramePadding.x, style.FramePadding.y ), 0.0f, style.FrameRounding ) );
	window->DrawList->AddImage( textureId, imageBox.Min, imageBox.Max );
	if( textSize.x > 0 )
	{
		ImGui::RenderText( textStart, pText );
	}

	return pressed;
}