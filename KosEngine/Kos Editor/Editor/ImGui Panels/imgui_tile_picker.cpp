/******************************************************************/
/*!
\file      imgui_tile_picker.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      29th Nov, 2024
\brief     Contains functions for handling the tile picker UI in the editor.

\details   This file includes:
		   - `isTileEmpty`: A helper function to determine if a specific
			 tile in the tilemap is empty, based on transparency or custom conditions.
		   - `ImGuiHandler::m_DrawTilePicker`: Renders the tile picker UI for
			 selecting tiles from the tilemap texture.

Functions:
	- bool isTileEmpty(const unsigned char* data, int x, int y, int tileWidth, int tileHeight, int imgWidth, int imgHeight, int channels):
		- Checks whether a specified tile in the tilemap texture is empty, considering transparency or other conditions.
		- Parameters:
			- `data`: Pointer to the image data.
			- `x`, `y`: The top-left corner of the tile in the image.
			- `tileWidth`, `tileHeight`: Dimensions of the tile.
			- `imgWidth`, `imgHeight`: The dimensions of the entire image.
			- `channels`: The number of channels (e.g., RGBA).
		- Returns: `true` if the tile is considered empty, `false` otherwise.

	- void ImGuiHandler::m_DrawTilePicker():
		- Displays the tile picker interface within an ImGui window.
		- It allows the user to select tiles from the tilemap texture and updates the `TilemapComponent` with the selected tile index.
		- Handles the rendering of tiles in a grid and supports empty tile filtering based on transparency.
		- Parameters: None.
		- Returns: None.
*/
/******************************************************************/


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "imgui_stdlib.h"
#include "ECS/ECS.h"
#include "Asset Manager/AssetManager.h"
#include "Graphics/GraphicsPipe.h"
#include "ECS/Layers.h"
#include "Editor/TilemapCalculations.h"
#include <string>


namespace gui

{
	bool isTileEmpty(const unsigned char* data, int x, int y, int tileWidth, int tileHeight, int imgWidth, int imgHeight, int channels) {
		// Ensure the tile fits within the image boundaries
		if (x < 0 || y < 0 || (x + tileWidth) > imgWidth || (y - tileHeight) > imgHeight) {
			return true; // Consider out-of-bounds tiles as empty or invalid
		}

		// Check each pixel in the tile for transparency or a specific condition
		for (int i = 0; i < tileHeight; ++i) {
			for (int j = 0; j < tileWidth; ++j) {
				int index = ((y - i) * imgWidth + (x + j)) * channels;

				// Example condition: check for transparency (alpha channel)
				if (channels == 4 && data[index + 3] != 0) {
					return false; // Non-empty pixel found
				}
			}
		}
		return true; // All pixels in the tile are "empty"
	}


	void ImGuiHandler::m_DrawTilePicker()
	{
		if (ImGui::Button("Back")) 
		{
			m_tilePickerMode = false;
			m_collisionSetterMode = false;
			return;
		}
		
		ImGui::SameLine();
		ImGui::BeginChild("ChildLb", ImVec2(0, ImGui::GetContentRegionAvail().y));

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		//graphicpipe::GraphicsPipe* gp = graphicpipe::GraphicsPipe::m_funcGetInstance();



		auto* tmc = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(m_clickedEntityId));
		//auto* transform = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_clickedEntityId));



		const float padding = 20.f;
		const float thumbnail = 80.f;
		float cellsize = padding + thumbnail;

		float panelwidth = ImGui::GetContentRegionAvail().x;
		int columns = (int)(panelwidth / cellsize);
		if (columns <= 0) {
			columns = 1;
		}
		ImGui::Columns(columns, 0, false);
		float xWidth = 1.0f / tmc->m_pictureRowLength;
		float yWidth = 1.0f / tmc->m_pictureColumnLength;

		const auto& image = assetmanager->m_imageManager.m_imageMap.find(tmc->m_tilemapFile);
		

		if (image != assetmanager->m_imageManager.m_imageMap.end())
		{
			auto* data = assetmanager->m_imageManager.m_imagedataArray[image->second.m_imageID];
			for (int i = 0; i < (tmc->m_pictureRowLength * tmc->m_pictureColumnLength); ++i)
			{

				int xIndex = i % tmc->m_pictureRowLength;

				int yIndex = static_cast<int>(floor(i / tmc->m_pictureRowLength));

				float uvX0 = xIndex * xWidth;               // Left UV coordinate
				float uvX1 = (xIndex + 1) * xWidth;         // Right UV coordinate
				float uvY0 = 1.f - (yIndex + 1) * yWidth;  // Top UV coordinate (flipped vertically)
				float uvY1 = 1.f - yIndex * yWidth;        // Bottom UV coordinate

				if (image->second.m_isTilable && isTileEmpty(data, static_cast<int>(uvX0 * image->second.m_width), static_cast<int>(uvY1 * image->second.m_height), static_cast<int>(xWidth * image->second.m_width), static_cast<int>(yWidth * image->second.m_height), image->second.m_width, image->second.m_height,  image->second.m_channels)) continue;

				ImGui::ImageButton(std::to_string(i).c_str(), (ImTextureID)(uintptr_t)assetmanager->m_imageManager.m_imageMap.find(tmc->m_tilemapFile)->second.textureID,
					{ thumbnail ,thumbnail }, { uvX0, uvY1 },{ uvX1, uvY0 }, { 1,1,1,1 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					tmc->m_tileIndex = i;
				}
				ImGui::Text(std::to_string(i).c_str());
				
				ImGui::NextColumn();
				
			}
		}

		
		
		ImGui::EndChild();
	}
}