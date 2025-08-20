/******************************************************************/
/*!
\file      Imgui_menubar.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the draw mainmenu bar window
           function.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "imgui_internal.h"

#include "Asset Manager/SceneManager.h"
#include "ECS/ECS.h"

#include "Editor/WindowFile.h"

void gui::ImGuiHandler::m_DrawMainMenuBar() {

    ImGuiIO& io = ImGui::GetIO();  // Get input/output data
    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
    ecs::ECS* ecs = ecs::ECS::m_GetInstance();
    //If CTRL + S press, save
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S)) && (ecs->m_getState() != ecs::RUNNING)) {
        scenemanager->m_SaveAllActiveScenes();
        LOGGING_INFO("Saving data...");
    }



    bool openNewFilepopup = false;;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {
                openNewFilepopup = true;


            }
           
            if (ImGui::MenuItem("Save")) {
                
                scenemanager->m_SaveAllActiveScenes();
                
            }

            // TODOm improve more test, case e.g. in prefab mode, changing scene            
            //if ((scenemanager->m_recentFiles.size() > 0) && ImGui::BeginMenu("Open Recent")) {

            //   for (auto& scene : scenemanager->m_recentFiles) {
            //        if (ImGui::MenuItem(scene.filename().stem().string().c_str())) {
            //            scenemanager->m_LoadScene(scene);
            //            m_activeScene = scene.filename().string();
            //            m_clickedEntityId = -1;
            //        }
            //    }

            //    ImGui::EndMenu();
            //}

            if (ecs->m_ECS_SceneMap.size() < 0 || ImGui::MenuItem("Open")) {
                

                char filePath[MAX_PATH];
                std::filesystem::path path = file::FileWindow::m_OpenfileDialog(filePath);
                if (!path.empty() && (path.filename().extension().string() == ".json")) {
                    //clear all other scenes
                    scenemanager->m_ClearAllScene();
                    scenemanager->m_LoadScene(path);
                    if (!m_prefabSceneMode) {
                        m_activeScene = path.filename().string();
                    }
                    else {
                        ecs::ECS::m_GetInstance()->m_ECS_SceneMap.find(path.filename().string())->second.m_isActive = false;
                        m_savedSceneState[path.filename().string()] = true;
                    }
                    
                    m_clickedEntityId = -1;
                }
                else {
                    LOGGING_ERROR("Wrong File Type, only .json");
                }


            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Save Layout")) {
                SaveLayout();
                
            }

            ImGui::EndMenu();
        }

        if (openNewFilepopup) {
            ImGui::OpenPopup("New Scene");
        }


        if (ImGui::BeginPopupModal("New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Enter Scene Name");
            ImGui::Separator();

            //static int unused_i = 0;
            //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

            static char str1[128] = "";
            ImGui::InputTextWithHint(".json", "Enter scene name here", str1, IM_ARRAYSIZE(str1));




            if (ImGui::Button("Save", ImVec2(120, 0))) { 

                std::string m_jsonFilePath{ "../Assets/Scene/" }; //TODO temp open window in future
                std::string scene = m_jsonFilePath + str1 + ".json";
                m_activeScene = scene;
                if (!scene.empty()) {
                    if (scenemanager->m_CreateNewScene(scene)) {

                        //remove all scenes
                        scenemanager->m_ClearAllScene();

                        scenemanager->m_LoadScene(scene);
                        m_activeScene = std::string(str1) + ".json";
                        m_clickedEntityId = -1;
                    }
                    else {
                        LOGGING_ERROR("Fail to create scene");
                    }
                }
                else {
                    LOGGING_WARN("Please Insert a Scene Name");
                }



                ImGui::CloseCurrentPopup(); 

            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::EndMainMenuBar();
    }



    
}

