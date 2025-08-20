/******************************************************************/
/*!
\file      imgui_editor_window.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th November, 2024
\brief     This file implements the editor window interface for the
           application, using ImGui for the graphical user interface.

           The editor window provides an interactive interface for the
           user to manipulate and view objects in the application's
           scene, such as entities, cameras, and textures. It includes
           functionality for rendering the scene in a window, zooming,
           dragging, and resetting the camera view. Additionally, it
           supports drag-and-drop of assets (such as .png, .ttf, and
           .prefab files) into the editor window, automatically creating
           entities and setting their properties based on the file type.

           Key features of this file:
           - Draws the editor window and render screen.
           - Handles camera zoom, drag, and reset controls.
           - Manages drag-and-drop functionality for asset files.
           - Updates camera matrices and view transformations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/



#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "imgui_internal.h"


#include "ECS/ECS.h"
#include "Application/Application.h"

#include "Graphics/GraphicsPipe.h"
#include "Graphics/GraphicsCamera.h"
#include "Editor/EditorCamera.h"

#include "ECS/Hierachy.h"
#include "Editor/TilemapCalculations.h"



void gui::ImGuiHandler::m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight)
{
    graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
    ecs::ECS* ecs = ecs::ECS::m_GetInstance();

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;
    bool open = true;

    ImGui::Begin("Editor Window", &open, window_flags);


    m_DrawPlayPauseBar();
    

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 renderWindowSize = ImGui::GetContentRegionAvail();

   

    float textureAspectRatio = (float)windowWidth / (float)windowHeight;
    float renderWindowAspectRatio = renderWindowSize.x / renderWindowSize.y;

    ImVec2 imageSize;
    imageSize.x = windowWidth / 2.f;
    imageSize.y = windowHeight / 2.f;

    //Dynamic Window Resizing
   if (renderWindowAspectRatio > textureAspectRatio) 
    {
        imageSize.y = renderWindowSize.y;
        imageSize.x = imageSize.y * textureAspectRatio;
    }
    else 
    {
        imageSize.x = renderWindowSize.x;
        imageSize.y = imageSize.x / textureAspectRatio;
    }

    if (imageSize.x <= renderWindowSize.x)
    {
        pos.x += (renderWindowSize.x - imageSize.x) / 2;
    }

    if (imageSize.y <= renderWindowSize.y)
    {
        pos.y += (renderWindowSize.y - imageSize.y) / 2;
    }


    pipe->m_renderFinalPassWithDebug();
    ImGui::GetWindowDrawList()->AddImage(
        (void*)(long long unsigned int)pipe->m_gamePreviewTexture, pos,
        ImVec2(pos.x + imageSize.x, pos.y + imageSize.y),
        ImVec2(0, 1), ImVec2(1, 0));

    EditorCamera::m_editorWindowPosition.m_x = pos.x;
    EditorCamera::m_editorWindowPosition.m_y = pos.y;
    EditorCamera::m_editorWindowDimensions.m_x = imageSize.x;
    EditorCamera::m_editorWindowDimensions.m_y = imageSize.y;

    auto calculateworld = [pos, imageSize]()-> vector3::Vec3 {
        float screencordX = ImGui::GetMousePos().x - pos.x;
        float screencordY = ImGui::GetMousePos().y - pos.y;

        //TODO calculate mouse pos correctly
        float cordX = (screencordX - imageSize.x / 2.f) / (imageSize.x / 2.f);
        float cordY = (std::abs(screencordY) - imageSize.y / 2.f) / (imageSize.y / 2.f);

        vector3::Vec3 translate = { cordX , -cordY, 0.f };
        translate.m_x *= EditorCamera::m_editorCameraMatrix.m_e00;
        translate.m_y *= EditorCamera::m_editorCameraMatrix.m_e11;
        translate.m_x *= 1.f / graphicpipe::GraphicsCamera::m_aspectRatio;
        translate.m_x += EditorCamera::m_editorCameraMatrix.m_e20;
        translate.m_y += EditorCamera::m_editorCameraMatrix.m_e21;

        return translate;
        };

   





    EditorCamera::calculateLevelEditorCamera();
    EditorCamera::calculateLevelEditorView();
    EditorCamera::calculateLevelEditorOrtho();

    graphicpipe::GraphicsCamera::m_currCameraMatrix = { EditorCamera::m_editorCameraMatrix.m_e00, EditorCamera::m_editorCameraMatrix.m_e01 ,EditorCamera::m_editorCameraMatrix.m_e02 ,
                                                           EditorCamera::m_editorCameraMatrix.m_e10 ,EditorCamera::m_editorCameraMatrix.m_e11 ,EditorCamera::m_editorCameraMatrix.m_e12 ,
                                                           EditorCamera::m_editorCameraMatrix.m_e20 ,EditorCamera::m_editorCameraMatrix.m_e21 ,EditorCamera::m_editorCameraMatrix.m_e22 };
    graphicpipe::GraphicsCamera::m_currViewMatrix = { EditorCamera::m_editorViewMatrix.m_e00, EditorCamera::m_editorViewMatrix.m_e01, EditorCamera::m_editorViewMatrix.m_e02,
                                                      EditorCamera::m_editorViewMatrix.m_e10 ,EditorCamera::m_editorViewMatrix.m_e11 ,EditorCamera::m_editorViewMatrix.m_e12 ,
                                                      EditorCamera::m_editorViewMatrix.m_e20 ,EditorCamera::m_editorViewMatrix.m_e21 ,EditorCamera::m_editorViewMatrix.m_e22 };

    //If no Camera, Set Editor Camera as Game Camera
    if (graphicpipe::GraphicsCamera::m_cameras.size() == 0)
    {
        graphicpipe::GraphicsCamera::m_currCameraScaleX = EditorCamera::m_editorCamera.m_zoom.m_x;
        graphicpipe::GraphicsCamera::m_currCameraScaleY = EditorCamera::m_editorCamera.m_zoom.m_y;
        graphicpipe::GraphicsCamera::m_currCameraTranslateX = EditorCamera::m_editorCamera.m_coordinates.m_x;
        graphicpipe::GraphicsCamera::m_currCameraTranslateY = EditorCamera::m_editorCamera.m_coordinates.m_y;
        graphicpipe::GraphicsCamera::m_currCameraRotate = 0.f;
    }
    

    //Draw gizmo
    m_DrawGizmo(pos.x, pos.y, imageSize.x, imageSize.y);



    float scrollInput = ImGui::GetIO().MouseWheel; // Positive for zoom in, negative for zoom out

    //Zoom In/Out Camera
    if (ImGui::IsWindowHovered())
    {
        EditorCamera::m_editorCamera.m_zoom.m_x -= scrollInput * EditorCamera::m_editorCameraZoomSensitivity * EditorCamera::m_editorCamera.m_zoom.m_x;
        EditorCamera::m_editorCamera.m_zoom.m_y -= scrollInput * EditorCamera::m_editorCameraZoomSensitivity * EditorCamera::m_editorCamera.m_zoom.m_y;

        EditorCamera::m_editorCamera.m_zoom.m_x = std::clamp(EditorCamera::m_editorCamera.m_zoom.m_x, 0.1f, 100.f);
        EditorCamera::m_editorCamera.m_zoom.m_y = std::clamp(EditorCamera::m_editorCamera.m_zoom.m_y, 0.1f, 100.f);


    }

    //Move Camera Around
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
    {
        ImVec2 mouseDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

        vector2::Vec2 delta = vector2::Vec2(mouseDelta.x, mouseDelta.y) * EditorCamera::m_editorCameraDragSensitivity * EditorCamera::m_editorCamera.m_zoom.m_x;

        // Update the camera position
        EditorCamera::m_editorCamera.m_coordinates.m_x -= delta.m_x;
        EditorCamera::m_editorCamera.m_coordinates.m_y += delta.m_y;

        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
    }

    //Reset Camera To Center
    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_R))
    {
        EditorCamera::m_editorCamera.m_coordinates.m_x = 0.f;
        EditorCamera::m_editorCamera.m_coordinates.m_y = 0.f;
        EditorCamera::m_editorCamera.m_zoom.m_x = 1.f;
        EditorCamera::m_editorCamera.m_zoom.m_y = 1.f;
    }

    static unsigned int lastEntity{};

    // Clean up behaviours when switching entities
    if (static_cast<int>(lastEntity) != m_clickedEntityId)
    {
        lastEntity = m_clickedEntityId;
        m_tilePickerMode = false;
        m_collisionSetterMode = false;
    }

    //set tile map 
    if (m_tilePickerMode && ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing() && ImGui::IsWindowHovered() && ecs->m_ECS_EntityMap[m_clickedEntityId].test(ecs::TYPETILEMAPCOMPONENT))
    {
        auto* tmc = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(m_clickedEntityId));
        auto* transform = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_clickedEntityId));
        if (!tmc->m_tilemapFile.empty()) {
            Tilemap::setIndividualTile(transform->m_position, EditorCamera::calculateWorldCoordinatesFromMouse(static_cast<int>(ImGui::GetMousePos().x), static_cast<int>(ImGui::GetMousePos().y)), tmc);
        }
        
    }

    if (m_collisionSetterMode && ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing() && ImGui::IsWindowHovered() && ecs->m_ECS_EntityMap[m_clickedEntityId].test(ecs::TYPEGRIDCOMPONENT))
    {
        auto* grid = static_cast<ecs::GridComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(m_clickedEntityId));
        auto* transform = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(m_clickedEntityId));
      
        Tilemap::setCollidableTile(transform->m_position, EditorCamera::calculateWorldCoordinatesFromMouse(static_cast<int>(ImGui::GetMousePos().x), static_cast<int>(ImGui::GetMousePos().y)), grid);
        /*Tilemap::debugGridChecks(grid);*/
        

    }

    if (ImGui::IsWindowHovered() && !ImGuizmo::IsUsing() && ImGui::IsMouseClicked(0) && !m_tilePickerMode) {
        //If cursor selects object, object is selected
        auto transform = calculateworld();
        ImVec2 WorldMouse = ImVec2{ transform.m_x, transform.m_y };
        //calculate AABB of each object (active scenes)
        for (auto& sceneentity : ecs->m_ECS_SceneMap) {

            if (!sceneentity.second.m_isActive) continue;

            for (auto& entity : sceneentity.second.m_sceneIDs) {
                //calculate AABB
                auto* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));
                const mat3x3::Mat3x3& transformation = tc->m_transformation;


                vector2::Vec2 min, max;

                vector2::Vec2 translation, scale;
                float rotation;

                mat3x3::Mat3Decompose(transformation, translation, scale, rotation);


                max = vector2::Vec2{ float(translation.m_x + scale.m_x * 0.5), float(translation.m_y + scale.m_y * 0.5) };
                min = vector2::Vec2{ float(translation.m_x - scale.m_x * 0.5), float(translation.m_y - scale.m_y * 0.5) };

                if ((min.m_x <= WorldMouse.x && WorldMouse.x <= max.m_x) &&
                    (min.m_y <= WorldMouse.y && WorldMouse.y <= max.m_y)) {

                    m_clickedEntityId = entity;
                    break;
                }

            }



        }


        //if pos is within any of the object, set that object as active.

    }


    //delete entity
    if (ImGui::IsKeyPressed(ImGuiKey_Delete))
    {
        if (m_clickedEntityId >= 0) {

            events::RemoveEntity removeEvent(m_clickedEntityId);
            //DISPATCH_ACTION_EVENT(removeEvent);
            
            ecs::ECS::m_GetInstance()->m_DeleteEntity(m_clickedEntityId);
            m_clickedEntityId = -1;

        }
    }



    //For Dragging Assets Into Editor Window
    ImGui::Dummy(renderWindowSize);
    if (ImGui::BeginDragDropTarget())
    {

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
        {
            ecs::ECS* fileecs = ecs::ECS::m_GetInstance();

            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);

            vector3::Vec3 translate = calculateworld();


            if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {
                assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                ecs::EntityID id = fileecs->m_CreateEntity(m_activeScene); //assign to active scene
                ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                transCom->m_position = { translate.m_x, translate.m_y };
                // Insert matrix
                ecs::NameComponent* nameCom = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
                nameCom->m_entityName = filename->filename().stem().string();

                //If Sprite Is Flagged As Tile Map
                if ((assets->m_imageManager.m_imageMap.find(filename->filename().string()) != assets->m_imageManager.m_imageMap.end()) && assets->m_imageManager.m_imageMap[filename->filename().string()].m_isTilable)
                {
                    ecs::TilemapComponent* tileCom = static_cast<ecs::TilemapComponent*>(ecs->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, id));
                    tileCom->m_tilemapFile = filename->filename().string();
                    tileCom->m_pictureColumnLength = 1;
                    tileCom->m_pictureRowLength = 1;
                    tileCom->m_rowLength = 1;
                    tileCom->m_columnLength = 1;
                    tileCom->m_tilePictureIndex = { {0} };
                }
                else
                {
                    ecs::SpriteComponent* spriteCom = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id));

                    //If Entity Does Not Have Tilemap Component
                    if (!ecs->m_ECS_EntityMap[id].test(ecs::TYPETILEMAPCOMPONENT) && (assets->m_imageManager.m_imageMap.find(filename->filename().string()) != assets->m_imageManager.m_imageMap.end()))
                    {
                        ecs::ColliderComponent* colCom = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, id));

                        spriteCom->m_imageFile = filename->filename().string();
                        colCom->m_Size.m_x = static_cast<float>(static_cast<float>(assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_width) / static_cast<float>(pipe->m_unitWidth) / assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_stripCount);
                        colCom->m_Size.m_y = static_cast<float>(assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_height) / static_cast<float>(pipe->m_unitHeight);
                        
                        //If Sprite Was Flagged as Animation Sprite
                        if (!ecs->m_ECS_EntityMap[id].test(ecs::TYPEANIMATIONCOMPONENT) && assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_stripCount != 1)
                        {
                            ecs::AnimationComponent* aniCom = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, id));
                            aniCom->m_stripCount = assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_stripCount;
                        }
                        else if (ecs->m_ECS_EntityMap[id].test(ecs::TYPEANIMATIONCOMPONENT))
                        {
                            auto* ac = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(id));
                            ac->m_stripCount = assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_stripCount;
                        }

                    }
                    else
                    {
                        LOGGING_WARN("Restriction: Tilemap Component Not Allowed With Sprite Component or Please Reload Content Browser");
                    }

                }

                if (m_prefabSceneMode) {
                    ecs::Hierachy::m_SetParent(ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID, id);

                }

                m_clickedEntityId = id;
            }
            if (filename->filename().extension().string() == ".ttf") {
                
                ecs::EntityID id = fileecs->m_CreateEntity(m_activeScene); //assign to top most scene
                ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                transCom->m_position = { translate.m_x, translate.m_y };
                // Insert matrix
                ecs::NameComponent* nameCom = static_cast<ecs::NameComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
                nameCom->m_entityName = filename->filename().stem().string();
                ecs::TextComponent* textCom = static_cast<ecs::TextComponent*>(fileecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, id));
                textCom->m_fileName = filename->filename().string();

                if (m_prefabSceneMode) {
                    ecs::Hierachy::m_SetParent(ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID, id);
                }

                m_clickedEntityId = id;
            }

            if (filename->filename().extension().string() == ".mpg" || filename->filename().extension().string() == ".mpeg") {

                ecs::EntityID id = fileecs->m_CreateEntity(m_activeScene); //assign to top most scene
                ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                transCom->m_position = { translate.m_x, translate.m_y };
                // Insert matrix
                ecs::NameComponent* nameCom = static_cast<ecs::NameComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
                nameCom->m_entityName = filename->filename().stem().string();
                ecs::VideoComponent* vc = static_cast<ecs::VideoComponent*>(fileecs->m_AddComponent(ecs::TYPEVIDEOCOMPONENT, id));
                vc->filename = filename->filename().string();
                vc->play = true;
                if (m_prefabSceneMode) {
                    ecs::Hierachy::m_SetParent(ecs->m_ECS_SceneMap.find(m_activeScene)->second.m_prefabID, id);
                }

                m_clickedEntityId = id;
            }

            if (!m_prefabSceneMode && filename->filename().extension().string() == ".prefab") {//dont allow adding of prefab in prefab 

                try {
                    //check to see if prefab is even loaded
                    if (ecs->m_ECS_SceneMap.find(filename->filename().string()) != ecs->m_ECS_SceneMap.end()) {
                        ecs::EntityID id = prefab::Prefab::m_CreatePrefab(filename->filename().string(), m_activeScene);
                        ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(fileecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                        transCom->m_position = { translate.m_x, translate.m_y };
                    }
                    else {
                        LOGGING_ERROR("Prefab not loaded");
                    }



                }
                catch (...) {
                    LOGGING_ERROR("Prefab fail to load");
                }

            }


        }
        ImGui::EndDragDropTarget();
    }


    ImGui::End();


}



