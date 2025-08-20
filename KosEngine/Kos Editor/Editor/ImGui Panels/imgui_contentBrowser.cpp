/******************************************************************/
/*!
\file      imgui_contentBrowser.cpp
\author    Ng Jaz Winn, jazwinn.ng, 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 30, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


#include "Editor.h"

#include <filesystem>
#include <string>
#include "Asset Manager/AssetManager.h"
#include "Asset Manager/SceneManager.h"
#include "Inputs/Input.h"
#include <imgui_internal.h>

namespace gui {
	std::vector<std::string> filePath = Serialization::Serialize::m_LoadFilePath("../configs");
	static std::filesystem::path assetDirectory = filePath[1];
	static std::filesystem::path currentDirectory  = assetDirectory;
	static const char* fileIcon = "img_folderIcon.png";
	static std::string searchString;
	static float padding = 20.f;
	static float thumbnail = 100.f;

	void MoveFolder(const std::filesystem::path& newDirectory) {
		if (ImGui::BeginDragDropTarget())
		{
			//assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
			{

				IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
				std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);


				if (newDirectory == *filename) return;
				std::filesystem::path destinationFile = newDirectory / filename->filename();
				
				std::filesystem::rename(*filename, destinationFile);

			}
			ImGui::EndDragDropTarget();
		}
	}

	void textorimage(std::string directoryString, std::string fileName) {
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		if (assetmanager->m_imageManager.m_imageMap.find(fileName) != assetmanager->m_imageManager.m_imageMap.end()) {
			float imageRatio = static_cast<float>(assetmanager->m_imageManager.m_imageMap.find(fileName)->second.m_height) / static_cast<float>(assetmanager->m_imageManager.m_imageMap.find(fileName)->second.m_width);
			if (imageRatio > 1)
			{
				ImGui::ImageButton(directoryString.c_str(), (ImTextureID)(uintptr_t)assetmanager->m_imageManager.m_imageMap.find(fileName)->second.textureID, { thumbnail / imageRatio ,thumbnail }, { 0 ,1 }, { 1 ,0 }, { 0,0,0,0 });
			}
			else
			{
				ImGui::ImageButton(directoryString.c_str(), (ImTextureID)(uintptr_t)assetmanager->m_imageManager.m_imageMap.find(fileName)->second.textureID, { thumbnail ,thumbnail * imageRatio }, { 0 ,1 }, { 1 ,0 }, { 0,0,0,0 });
			}

		}
		else {
			ImGui::Button(directoryString.c_str(), { thumbnail ,thumbnail });
		}
		};

	void ImGuiHandler::m_DrawContentBrowser() {

		ImGui::Begin("Content Browser");
		
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		//if (currentDirectory != assetDirectory && ImGui::Button("Back")) {

		//	currentDirectory = currentDirectory.parent_path();

		//}
		//else {
		//	ImGui::NewLine();
		//}
		{
			ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border);
			static bool isSelected{ false };
			for (auto& directoryPath : std::filesystem::directory_iterator(assetDirectory)) {
				std::string directoryString = directoryPath.path().filename().string();
				isSelected = ImGui::Selectable(directoryString.c_str());
				MoveFolder(assetDirectory/directoryPath.path().filename());

				if (isSelected) {

					if (std::filesystem::is_directory(directoryPath)) {
						currentDirectory = assetDirectory / directoryPath.path().filename();
						//reset search string
						searchString.clear();
					}



				}

			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		if (m_tilePickerMode && ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_HasComponent(m_clickedEntityId))
		{
			m_DrawTilePicker();
		}
		else if (m_collisionSetterMode && ecs->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_HasComponent(m_clickedEntityId))
		{
			if (ImGui::Button("Back"))
			{
				m_tilePickerMode = false;
				m_collisionSetterMode = false;
			}
		}
		else
		{
			ImGui::BeginChild("ChildLa", ImVec2(0, ImGui::GetContentRegionAvail().y) , 0 , ImGuiWindowFlags_MenuBar);

			if (ImGui::BeginPopupContextWindow()) {
				if (ImGui::MenuItem("Add Folder")) {

					std::string path = "/New Folder";

					int count{1};
					while (std::filesystem::exists(currentDirectory.string() + path)) {
						path = "/New Folder_" + std::to_string(count++);
					}

					if (std::filesystem::create_directories(currentDirectory.string() + path)) {

						LOGGING_INFO("Directory created successfully!");
					}
					else {
						LOGGING_ERROR("Directory already exists or failed to create!");
					}

				}
				if (ImGui::MenuItem("Reload Browser")) {
					assetmanager::AssetManager* AstManager = assetmanager::AssetManager::m_funcGetInstance();
					AstManager->m_funcLoadAssets(assetDirectory.string());
				}
				ImGui::EndPopup();
			}

			//search bar
			if (m_prefabSceneMode)searchString.clear();

			//menu bar for search

			if (ImGui::BeginMenuBar()) {
				ImGui::Text("Search:");
				ImGui::SameLine(); // Keep the next widget on the same line
				ImGui::SetNextItemWidth(300);
				if (ImGui::InputText("##4312Search", &searchString)) {

				}
				ImGui::EndMenuBar(); // End menu bar
			}

			//back button
			
			if (currentDirectory != assetDirectory) {
				bool open = ImGui::Button("Back");
				MoveFolder(currentDirectory.parent_path());
				if (open) {
					currentDirectory = currentDirectory.parent_path();
					searchString.clear();
				}
				
			}
			else {
				ImGui::NewLine();
			}


			float cellsize = padding + thumbnail;

			float panelwidth = ImGui::GetContentRegionAvail().x;
			int columns = (int)(panelwidth / cellsize);
			if (columns <= 0) {
				columns = 1;
			}
			ImGui::Columns(columns, 0, false);




			for (auto& directoryPath : std::filesystem::directory_iterator(currentDirectory)) {
				std::string directoryString = directoryPath.path().filename().string();

				if (!searchString.empty() && !containsSubstring(directoryString, searchString)) {


					continue;
				}
				

				if (directoryPath.is_directory()) {
					// if a folder
					textorimage(directoryString, fileIcon);
					
					MoveFolder(currentDirectory / directoryPath.path().filename());


					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
						currentDirectory /= directoryPath.path().filename();

						//reset search
						searchString.clear();
					}
				}
				else {
					if (directoryPath.path().filename().extension().string() == ".png" || directoryPath.path().filename().extension().string() == ".jpg") {
						std::string fileName = directoryPath.path().filename().string();
						textorimage(directoryString, fileName);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {


						}
					}
					else if (directoryPath.path().filename().extension().string() == ".wav") {
						std::string wavicon = "img_audioWav.png";
						textorimage(directoryString, wavicon);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							//TODO play sound
							events::AudioFromImgui audioDblClick(directoryPath.path().filename().string());
							DISPATCH_BUTTON_EVENT(audioDblClick);
						}
					}
					else if (directoryPath.path().filename().extension().string() == ".cs") {
						std::string script = "img_scriptIcon.png";
						textorimage(directoryString, script);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							std::string command = "code \"" + directoryPath.path().string() + "\"";

							// Execute the command to open the file
							int result = std::system(command.c_str());

							if (result != 0) {
								LOGGING_ERROR("Fail to open .cs script");
							}

						}
					}
					else if (directoryPath.path().filename().extension().string() == ".prefab") {
						std::string prefab = "";
						textorimage(std::string(directoryPath.path().filename().extension().string() + "##" + directoryPath.path().filename().string()).c_str(), prefab);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
							//skip if active scene is filename
							if (m_activeScene == directoryPath.path().filename())continue;

							const auto& prefabscene = ecs->m_ECS_SceneMap.find(directoryPath.path().filename().string());
							if (prefabscene == ecs->m_ECS_SceneMap.end()) {
								LOGGING_ERROR("Prefab not loaded");
								continue;
							}

							//skip if prefab mode alraedy true
							if (!m_prefabSceneMode) {
								m_savedSceneState.clear();
								for (auto& scene : ecs->m_ECS_SceneMap) {
									if (scene.second.m_isPrefab == false) {
										//save all scenes active state
										m_savedSceneState[scene.first] = scene.second.m_isActive;
									}
								}

							}

							

							// clear save scene state

							// unload all regular scenes
							for (auto& scene : ecs->m_ECS_SceneMap) {
								scene.second.m_isActive = false;
							}

							// set prefab to active
							prefabscene->second.m_isActive = true;

							//set prefab as active scene
							m_activeScene = directoryPath.path().filename().string();

							m_prefabSceneMode = true;

							m_clickedEntityId = -1;
						}
					}
					else if (directoryPath.path().filename().extension().string() == ".json") {
						std::string script;
						textorimage(directoryString, script);

						if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {

							scenemanager->m_ClearAllScene();
							scenemanager->m_LoadScene(directoryPath.path());
							if (!m_prefabSceneMode) {
								m_activeScene = directoryPath.path().filename().string();
							}
							else {
								ecs::ECS::m_GetInstance()->m_ECS_SceneMap.find(directoryPath.path().filename().string())->second.m_isActive = false;
								m_savedSceneState[directoryPath.path().filename().string()] = true;
							}

							m_clickedEntityId = -1;

						}
					}
					else if (ImGui::Button(std::string(directoryPath.path().filename().extension().string() + "##" + directoryPath.path().filename().string()).c_str(), {thumbnail ,thumbnail})) {

						
					}


				}
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
					static std::filesystem::path filepath;
					filepath = directoryPath.path();
					ImGui::SetDragDropPayload("file", &filepath, sizeof(std::filesystem::path));
					ImGui::Text(filepath.string().c_str());
					ImGui::EndDragDropSource();
				}

				//create context window
				static bool rename = false;
				static bool _delete = false;
				static std::filesystem::path selectedfile{};
				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem("Rename")) {
						rename = true;
						selectedfile = directoryString;
					}
					if (ImGui::MenuItem("Delete")) {
						std::filesystem::remove_all(directoryPath.path());
						_delete = true;
						selectedfile = directoryString;
					}
					if (directoryPath.path().filename().extension().string() == ".json" && ImGui::MenuItem("Set as Startup Scene")) {

						if (Serialization::Serialize::m_UpdateConfigStartScene(directoryPath.path().string())) {
							
							LOGGING_INFO("Scene Updated to Config");
						}
						else {
							LOGGING_ERROR("Scene fail to update Config");
						}
						
					}


					ImGui::EndPopup();
				}


				if (rename && (selectedfile.string() == directoryString)) {
					if (ImGui::InputText("##rename", m_charBuffer, IM_ARRAYSIZE(m_charBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
						//TODO check if file has extension, keep the extension
						std::string extension{};
						if (!directoryPath.is_directory()) {
							extension = directoryPath.path().filename().extension().string();
							ImGui::SameLine();
							ImGui::Text(extension.c_str());
						}

						std::filesystem::path path = std::filesystem::current_path();
						std::string newpath = path.string() + "\\" + currentDirectory.string() + "\\" + m_charBuffer + extension;
						std::string oldpath = path.string() + "\\" + currentDirectory.string() + "\\" + directoryString;


						assetmanager->m_RenameAsset(oldpath, newpath);

						rename = false;
						selectedfile.clear();

						//TODO edge cases,
						//Update assets if any of them are renamed
					}
				}
				else {
					ImGui::SetWindowFontScale(0.8f);
					ImGui::Text(directoryPath.path().filename().stem().string().c_str());
					ImGui::SetWindowFontScale(1.f);
				}

				if (_delete){
					
					if (selectedfile.filename().extension().string() == ".cs") {

						//erease script from scriptmanager
					
						//assetmanager->m_scriptManager.m_CSScripts.erase(std::find(assetmanager->m_scriptManager.m_CSScripts.begin(), assetmanager->m_scriptManager.m_CSScripts.end(), selectedfile.filename().stem().string()));

					}
					

					_delete = false;
				}

				

				ImGui::NextColumn();
			}

			//allow drag and drop on child directory
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
				if (!Input::InputSystem::m_droppedFiles.empty()) {
					for (const auto& file : Input::InputSystem::m_droppedFiles) {
						std::filesystem::path source = file;
						std::filesystem::path destination = currentDirectory;

						if (!std::filesystem::exists(source)) {
							LOGGING_WARN("Source directory does not exist ");
						}
						else {
							if (std::filesystem::is_directory(source)) {
								destination /= source.filename();
								if (std::filesystem::exists(destination)) {
									LOGGING_WARN("Directory already exists");
									continue;
								}
							}

							if (std::filesystem::exists(destination/source.filename())) {
								LOGGING_WARN("Directory already in folder");
								continue;
							}

							// Copy directory and all contents recursively
							std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive);

							//load new asset
							assetmanager->m_LoadAsset(destination/source.filename());
						}

					}
					// Clear the vector if you want to reset after displaying
					Input::InputSystem::m_droppedFiles.clear();
				}
			}

			//if (ImGui::BeginDrapDropTargetWindow("Entity"))
			//{


			//	ImGui::EndDragDropTarget();
			//}


			ImGui::EndChild();
		}


		ImGui::End();
	}

}