#include "tiki/tool_application/tool_ui.hpp"

#include "tiki/graphics/texture_data.hpp"

#include <imgui_internal.h>

ImTextureID ImGui::Tex( const tiki::ToolImage& toolImage )
{
	return (ImTextureID)&toolImage.getData();
}

ImVec2 ImGui::Vec2( const tiki::Vector2& vec2 )
{
	return ImVec2( vec2.x, vec2.y );
}

tiki::Vector2 ImGui::Vec2( const ImVec2& vec2 )
{
	return tiki::vector::create( vec2.x, vec2.y );
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

void ImGui::BeginHorizontalSplitter( ImHorizontalSplitter& splitter, float left, float top, float right, float bottom )
{
	splitter.Top				= top;
	splitter.Bottom				= bottom;
	splitter.Right				= right;
	splitter.ElementCount		= 0u;
	splitter.LastElementRight	= left;
	splitter.LastElementLeft	= left;
	splitter.pLastElementWidth	= nullptr;
}

bool ImGui::BeginHorizontalSplitterElement( ImHorizontalSplitter& splitter, const char* pName, float* pElementWidth )
{
	const ImGuiIO& io = ImGui::GetIO();

	float left = splitter.LastElementRight;
	if( splitter.ElementCount > 0u )
	{
		const ImVec2 windowSize = ImVec2( 5.0f, io.DisplaySize.y - splitter.Bottom - splitter.Top );
		ImGui::SetNextWindowSize( windowSize, ImGuiCond_Always );
		ImGui::SetNextWindowSizeConstraints( windowSize, windowSize );
		if( ImGui::Begin( "Splitter", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing /*| ImGuiWindowFlags_NoNav*/ ) )
		{
			if( ImGui::IsWindowAppearing() )
			{
				ImGui::SetWindowPos( ImVec2( left, splitter.Top ) );
			}

			if( splitter.pLastElementWidth != nullptr )
			{
				const float windowLeft = ImGui::GetWindowPos().x;
				*splitter.pLastElementWidth = windowLeft - splitter.LastElementLeft;
				ImGui::SetWindowPos( ImVec2( windowLeft, splitter.Top ) );
			}

			ImGui::End();
		}

		left += 5.0f;
	}

	float width = 0.0f;
	if( pElementWidth != nullptr )
	{
		width = *pElementWidth;
	}
	else
	{
		width = io.DisplaySize.x - left - 5.0f;
	}

	ImGui::SetNextWindowPos( ImVec2( left, splitter.Top ), ImGuiCond_Always, ImVec2() );
	ImGui::SetNextWindowSize( ImVec2( width, io.DisplaySize.y - splitter.Bottom - splitter.Top ), ImGuiCond_Always );
	if( !ImGui::Begin( pName, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
	{
		return false;
	}

	splitter.ElementCount++;
	splitter.LastElementRight = left + width;
	splitter.LastElementLeft = left;
	splitter.pLastElementWidth = pElementWidth;

	return true;
}

void ImGui::EndHorizontalSplitterElement( ImHorizontalSplitter& splitter )
{
	ImGui::End();
}

void ImGui::EndHorizontalSplitter( ImHorizontalSplitter& splitter )
{
}
