/********************************************************************/
/*!
\file      imgui_hierarchy_window.cpp
\author    Ng Jaz Winn, jazwinn.ng, 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 30, 2024
\brief     This file contains the implementation of the ImGui Hierarchy Window for rendering and managing game objects.
           - Displays and updates a list of game objects (entities) in the ECS framework.
           - Provides functionality for adding, deleting, and duplicating game objects.
           - Uses ImGui to create a hierarchy view, with interaction buttons for each game object.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"

#include "ECS/ECS.h"
#include "ECS/Hierachy.h"
#include "De&Serialization/json_handler.h"
#include "Asset Manager/AssetManager.h"
#include "Debugging/Logging.h"
#include "Asset Manager/SceneManager.h"
#include "Asset Manager/Prefab.h"
//Testing purposes! this are the three files needed!
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>



namespace gui {

    


    unsigned int ImGuiHandler::m_DrawHierachyWindow()
    {
        //fetch ecs
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
        //assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
        // Custom window with example widgets
        ImGui::Begin("Hierachy Window", nullptr, ImGuiWindowFlags_MenuBar);

        //if (ImGui::BeginMenuBar())
        //{

        //    ImGui::EndMenuBar();
        //}

      /* std::string ObjectCountStr = "Oject Count: " + std::to_string(ecs->m_ECS_EntityMap.size());
        ImGui::Text(ObjectCountStr.c_str());*/

        static std::string searchString;
        if (m_prefabSceneMode)searchString.clear();

        //menu bar for search

        if (ImGui::BeginMenuBar()) {
            ImGui::Text("Search:");
            ImGui::SameLine(); // Keep the next widget on the same line
            if (ImGui::InputText("###1234Search", &searchString)) {

            }
            ImGui::EndMenuBar(); // End menu bar
        }


        if (ImGui::Button("+ Add GameObject"))
            ImGuiHandler::m_objectNameBox ? ImGuiHandler::m_objectNameBox = false : m_objectNameBox = true;

        //create ID then push into vector
        // assign to the top most scene



        if (m_objectNameBox)
        {
            if (ImGui::InputText("##", m_charBuffer, IM_ARRAYSIZE(m_charBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {

                ecs::EntityID newEntityID = ecs->m_CreateEntity(m_activeScene);

                //if in prefab mode, assign entity to upmost parent
                if (m_prefabSceneMode) {
                    ecs::EntityID id = ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID;
                    ecs::Hierachy::m_SetParent(std::move(id), newEntityID);
                }


                //set new ID to be clicked
                m_clickedEntityId = newEntityID;
                //Add the string into the vector
                // add string to name component
                static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(newEntityID))->m_entityName = std::string(m_charBuffer);
                //events::AddEntity addEvent(newEntityID);
                ////DISPATCH_ACTION_EVENT(addEvent);

                m_charBuffer[0] = '\0';
                m_objectNameBox = false;

                //Serialization::Serialize::SaveComponentsJson("RoombaRampage/Json", Ecs::ECS::GetInstance()->ECS_EntityMap, obj_text_entries, obj_entity_id);
            }
        }


        for (auto& sceneentity : ecs->m_ECS_SceneMap) {

            //when prefab mode is on, skip non prefabs, and non active prefabs
            if (m_prefabSceneMode) {

                if (sceneentity.second.m_isPrefab == false || sceneentity.second.m_isActive == false)continue;

                //create seperator
                ImGui::SeparatorText("Prefab");

                if (ImGui::Button("Back")){
                    //save "prefab"
                    scenemanager->m_SaveScene(m_activeScene);
                    //set current prefab back to inactive
                    ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_isActive = false;
                    
                    //set back scene's active state
                    for (auto& scene : ecs->m_ECS_SceneMap) {
                        if(scene.second.m_isPrefab == false)
                        scene.second.m_isActive = m_savedSceneState.find(scene.first)->second;
                        
                    }

                    //set back active scene
                    for (auto& scene : ecs->m_ECS_SceneMap) {
                        if (!scene.second.m_isPrefab) {
                            m_activeScene = scene.first;
                            break;
                        }
                    }

                    m_prefabSceneMode = false;
                    m_clickedEntityId = -1;
                }



            }
            
            //skip if prefab is not active 
            if (sceneentity.second.m_isPrefab == true && sceneentity.second.m_isActive == false) continue;

            std::string headerstr = sceneentity.first.substr(0, sceneentity.first.find_last_of('.'));
            //collapsing header for scene
            bool opens{};
            if (sceneentity.second.m_isActive == false) {
                headerstr += " (Unloaded)";
                ImGui::CollapsingHeader(headerstr.c_str());
            }
            else {
                if (sceneentity.first == m_activeScene) {
                    headerstr += " (Active)";
                }
                opens = ImGui::CollapsingHeader(headerstr.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
            }





            if (ImGui::BeginPopupContextItem()) {
                if ((sceneentity.first != m_activeScene) && ImGui::MenuItem("Remove Scene")) {
                    scenemanager->m_ClearScene(sceneentity.first);

                    //break loop
                    ImGui::EndPopup();
                    break;
                }

                if ((sceneentity.first != m_activeScene) && (sceneentity.second.m_isActive == true) && ImGui::MenuItem("Unload Scene")) {
                    sceneentity.second.m_isActive = false;
                    m_clickedEntityId = -1;

                    if (!m_prefabSceneMode) {
                        //change scene if current active scene is unloaded
                        if (ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_isActive == false) {
                            //set first loaded scene as active
                            for (auto& scene : ecs->m_ECS_SceneMap) {
                                if (scene.second.m_isActive == true && scene.second.m_isPrefab == false) {
                                    m_activeScene = ecs->m_ECS_SceneMap.begin()->first;
                                }

                            }
                        }
                       

                    }

                    ImGui::EndPopup();
                    break;
                }

                if ((sceneentity.second.m_isActive == false) && ImGui::MenuItem("load Scene")) {
                    sceneentity.second.m_isActive = true;

                    ImGui::EndPopup();
                    break;
                }

                if (ImGui::MenuItem("Save Scene")) {
                    scenemanager->m_SaveScene(sceneentity.first);

                }

                if ((sceneentity.first != m_activeScene) && ImGui::MenuItem("Set Active")) {
                    m_activeScene = sceneentity.first;

                    if (sceneentity.second.m_isActive == false) {
                        sceneentity.second.m_isActive = true;
                    }
                }

                ImGui::EndPopup();
            }
            if (ImGui::BeginDragDropTarget())
            {

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                    ecs::EntityID Id = *static_cast<ecs::EntityID*>(payload->Data);
                    const auto& scene = scenemanager->GetSceneByEntityID(Id);

                    if (scene.has_value()) {
                        scenemanager->m_SwapScenes(scene.value(), sceneentity.first, Id);
                    }

                    //if entity is a child, break from parent
                    const auto& parent = ecs::Hierachy::m_GetParent(Id);
                    if (parent.has_value()) {
                        events::MoveEntityChildToParent moveEvent(Id, parent.value());
                        //DISPATCH_ACTION_EVENT(moveEvent);
                        ecs::Hierachy::m_RemoveParent(Id);
                    }

                    ecs::Hierachy::m_UpdateChildScene(Id);
                    

                }

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                    std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);

                    if (filename->filename().extension().string() == ".prefab") {
                        
                        prefab::Prefab::m_CreatePrefab(filename->filename().string(), sceneentity.first);

                    }

                }
                ImGui::EndDragDropTarget();
            }

            if (opens) {


                for (auto entity : sceneentity.second.m_sceneIDs) {

                    //search bar if if string not empty, must match the entity name
                    ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entity));
                    if (!searchString.empty() && !containsSubstring(nc->m_entityName, searchString))continue;

                    //draw parent entity node
                    //draw entity with no parents hahaha
                    if (!ecs::Hierachy::m_GetParent(entity).has_value()) {
                        if (m_DrawEntityNode(entity) == false) {
                            //delete is called
                            break;
                        }

                    }
                }
            }      
        }


           
        if (ImGui::GetContentRegionAvail().x > 0 && ImGui::GetContentRegionAvail().y > 0) {
            //std::cout << "x: " << ImGui::GetContentRegionAvail().x << std::endl;
            //std::cout << "y: " << ImGui::GetContentRegionAvail().y << std::endl;


            ImGui::InvisibleButton("#invbut", ImVec2{ ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y });
            if (ImGui::BeginDragDropTarget())
            {

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
                {
                    

                    IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                    ecs::EntityID Id = *static_cast<ecs::EntityID*>(payload->Data);

                    // if in prefab mode and parent does not have parent, reject
                    if (m_prefabSceneMode && ecs::Hierachy::m_GetParent(Id).has_value() && (!ecs::Hierachy::m_GetParent(ecs::Hierachy::m_GetParent(Id).value()).has_value())) {
                        
                    }
                    if (m_prefabSceneMode) {
                        ecs::Hierachy::m_SetParent(ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID, Id);
                    }
                    else {
                        ecs::Hierachy::m_RemoveParent(Id);
                    }
                    


                }

                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                    std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);


                    if (!m_prefabSceneMode && filename->filename().extension().string() == ".json") {
                        scenemanager->m_LoadScene(*filename);
                    }

                    if (!m_prefabSceneMode && filename->filename().extension().string() == ".prefab") {

                       prefab::Prefab::m_CreatePrefab(filename->filename().string(), m_activeScene);


                    }




                }
                ImGui::EndDragDropTarget();
            }

        }

        ImGui::End();

        return m_clickedEntityId;
    }


    bool ImGuiHandler::m_DrawEntityNode(ecs::EntityID id) {

        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        

        ecs::TransformComponent* transCom =  static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
        if (transCom == NULL) return false;

        ImGuiTreeNodeFlags flag = ((static_cast<unsigned int>(m_clickedEntityId) == id) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        if (transCom->m_childID.size() <= 0) {
            flag |= ImGuiTreeNodeFlags_Leaf;
        }
        
        ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        if (ImGui::InvisibleButton(std::string{ "##invireorderbutton" + std::to_string(id) }.c_str(), ImVec2{ImGui::GetContentRegionAvail().x , 1.f})) {

        }

        ImGui::PopStyleVar();

        if (ImGui::BeginDragDropTarget())
        {

            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
            {
                IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                ecs::EntityID SwapId = *static_cast<ecs::EntityID*>(payload->Data);

                const std::string swapScene = scenes::SceneManager::GetSceneByEntityID(SwapId).value();
                const std::string idScene = scenes::SceneManager::GetSceneByEntityID(id).value();

                if (SwapId != id && swapScene == idScene) {

                    auto& _scene = ecs->m_ECS_SceneMap.find(swapScene)->second;
                    const auto& eraseit = std::find(_scene.m_sceneIDs.begin(), _scene.m_sceneIDs.end(), SwapId);
                    
                    if (eraseit != _scene.m_sceneIDs.end()) {
                        _scene.m_sceneIDs.erase(eraseit);
                    }

                    const auto& IDit = std::find(_scene.m_sceneIDs.begin(), _scene.m_sceneIDs.end(), id);

                    if (IDit != _scene.m_sceneIDs.end()) {
                        _scene.m_sceneIDs.insert(IDit, SwapId);
                    }

                    


                    //return false;
                }
            }

            ImGui::EndDragDropTarget();
        }

        //create color if prefab
        if (nc->m_isPrefab) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.2f, 0.1f, 1.0f));
        bool open = ImGui::TreeNodeEx(std::to_string(id).c_str(), flag, nc->m_entityName.c_str());
        if (nc->m_isPrefab) ImGui::PopStyleColor();

        if (ImGui::IsItemClicked())
        {
            m_clickedEntityId = id;
            std::cout << "EntityID: " << id << std::endl;
            
        }
       

        //draw context window
        if (ImGui::BeginPopupContextItem()) {

            //disable if the upmost prefab
            if (m_prefabSceneMode && (id == ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID)) {
              

            }
            else {
                if (ImGui::MenuItem("Delete Entity")) {

                    events::RemoveEntity removeEvent(id);
                    //DISPATCH_ACTION_EVENT(removeEvent);
                    
                    ecs->m_DeleteEntity(id);
                    m_clickedEntityId = -1;
                    ImGui::EndPopup();
                    if (open)ImGui::TreePop();
                    return false;
                }
            }
           

            if (ImGui::MenuItem("Duplicate Entity")) {
                ecs::EntityID newid = ecs->m_DuplicateEntity(id);

                if (m_prefabSceneMode && (!ecs::Hierachy::m_GetParent(id).has_value())) {
                   ecs::Hierachy::m_SetParent(id, newid);
                }
                //events::AddEntity addEvent(newid);
                ////DISPATCH_ACTION_EVENT(addEvent);



                ImGui::EndPopup();
                if (open)ImGui::TreePop();
                return false;
                //return false to reset the loop
            }

            if (ImGui::MenuItem("Create Prefab")) {
                if (!m_prefabSceneMode) {
                    prefab::Prefab::m_SaveEntitytoPrefab(id);
                }
                
            }

            ImGui::EndPopup();
        }







        if (ImGui::BeginDragDropTarget())
        {
            
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
            {
                IM_ASSERT(payload->DataSize == sizeof(ecs::EntityID));
                ecs::EntityID childId = *static_cast<ecs::EntityID*>(payload->Data);

                // dont allow prefabs to be dragged inside prefab
                const auto& childnc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(childId));
                const auto& parent = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));


                if (!m_prefabSceneMode && childnc->m_isPrefab && (childnc->m_prefabName == parent->m_prefabName)) {

                    LOGGING_WARN("Unable to drag prefabs of same type into each other, pls go to prefab editor");
                }
                else {
                    const auto& parent2 = ecs::Hierachy::m_GetParent(childId);
                    if (parent2.has_value()) {
                        events::MoveEntityChildToChild moveEvent(childId, parent2.value(), id);
                        //DISPATCH_ACTION_EVENT(moveEvent);
                    }
                    else {
                        events::MoveEntityParentToChild moveEvent(childId, id);
                        //DISPATCH_ACTION_EVENT(moveEvent);
                    }
                    ecs::Hierachy::m_SetParent(id, childId);
                    LOGGING_INFO("Set Parent: %d, Child: %d", id, childId);
                    // update child's scene
                    ecs::Hierachy::m_UpdateChildScene(id);

                    //return
                    ImGui::EndDragDropTarget();
                    if (open)ImGui::TreePop();
                    return false;
                }

            }


            ImGui::EndDragDropTarget();
        }
        
        //no reordering of child prefabs
        if (!transCom->m_haveParent || !static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(transCom->m_parentID))->m_isPrefab ||
            static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(transCom->m_parentID))->m_prefabName != nc->m_prefabName || m_prefabSceneMode) {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                //might undefine behaviour
                ecs::EntityID index = id;
                ImGui::SetDragDropPayload("Entity", &index, sizeof(ecs::EntityID));
                ImGui::Text(nc->m_entityName.c_str());
                ImGui::EndDragDropSource();
            }
        }


        

        if (open) {
            //recursion
            if (transCom->m_childID.size() > 0) {
                for (auto& ids : transCom->m_childID) {
                    if (!m_DrawEntityNode(ids)) {

                        ImGui::TreePop();
                        return false;
                    }
                }
            }
           // m_DrawEntityNode(1);
            ImGui::TreePop();
        }


        return true;
    }


}

