/******************************************************************/
/*!
\file      imgui_game_scene_window.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th November, 2024
\brief     This file contains the implementation of the imgui game
           window, which integrates ImGui functionality for rendering
           in game elements.

           This file focuses on rendering the game scene preview
           within a dedicated ImGui window ("Game Window") while
           maintaining the correct aspect ratio for the render.
           The method m_DrawGameSceneWindow():
           - Initializes and updates the GraphicsPipe for rendering.
           - Computes the appropriate aspect ratio for the rendered
             scene to fit within the ImGui window.
           - Centers the rendered scene within the window.
           - Adjusts camera matrices to support the editor camera view.

           This file enables seamless integration of in-game scenes
           within an editor window, making it possible for users to
           preview gameplay directly from the editor.

           Key features:
           - Dynamic aspect ratio handling for scene previews.
           - Integration with GraphicsPipe and EditorCamera for
             scene rendering.
           - Use of ImGui's AddImage for displaying textures.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "ECS/ECS.h"
#include "Application/Application.h"

#include "Graphics/GraphicsPipe.h"
#include "Graphics/GraphicsCamera.h"
#include "Editor/EditorCamera.h"

#include "Graphics/GraphicsCamera.h"
#include <imgui_internal.h>

namespace gui
{
	void ImGuiHandler::m_DrawGameSceneWindow(std::string windowName)
	{
        graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();

        //pipe->m_gameMode = true;
        //pipe->m_funcUpdate();
        ////pipe->m_funcDrawGamePreviewWindow();
        //pipe->m_gameMode = false;
        ImGui::Begin(windowName.c_str());


        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 renderWindowSize = ImGui::GetContentRegionAvail();

        float textureAspectRatio = (float)graphicpipe::GraphicsCamera::m_windowWidth / (float)graphicpipe::GraphicsCamera::m_windowHeight;
        float renderWindowAspectRatio = renderWindowSize.x / renderWindowSize.y;

        ImVec2 imageSize;
        imageSize.x = graphicpipe::GraphicsCamera::m_windowWidth / 2.f;
        imageSize.y = graphicpipe::GraphicsCamera::m_windowHeight / 2.f;

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


        EditorCamera::m_gamePreviewWindowPosition.m_x = pos.x;
        EditorCamera::m_gamePreviewWindowPosition.m_y = pos.y;
        EditorCamera::m_gamePreviewWindowDimensions.m_x = imageSize.x;
        EditorCamera::m_gamePreviewWindowDimensions.m_y = imageSize.y;

        pipe->m_gameMode = true; //Set to game camera
        pipe->m_funcUpdate();
        pipe->m_gameMode = false;
        pipe->m_renderFinalPass();
        ImGui::GetWindowDrawList()->AddImage(
            (void*)(long long unsigned int)pipe->m_finalPassTexture, pos,
            ImVec2(pos.x + imageSize.x, pos.y + imageSize.y),
            ImVec2(0, 1), ImVec2(1, 0));

        EditorCamera::calculateLevelEditorCamera();
        EditorCamera::calculateLevelEditorView();
        graphicpipe::GraphicsCamera::m_currCameraMatrix = { EditorCamera::m_editorCameraMatrix.m_e00, EditorCamera::m_editorCameraMatrix.m_e01 ,EditorCamera::m_editorCameraMatrix.m_e02 ,
                                                            EditorCamera::m_editorCameraMatrix.m_e10 ,EditorCamera::m_editorCameraMatrix.m_e11 ,EditorCamera::m_editorCameraMatrix.m_e12 ,
                                                            EditorCamera::m_editorCameraMatrix.m_e20 ,EditorCamera::m_editorCameraMatrix.m_e21 ,EditorCamera::m_editorCameraMatrix.m_e22 };
        graphicpipe::GraphicsCamera::m_currViewMatrix = { EditorCamera::m_editorViewMatrix.m_e00, EditorCamera::m_editorViewMatrix.m_e01, EditorCamera::m_editorViewMatrix.m_e02,
                                                          EditorCamera::m_editorViewMatrix.m_e10 ,EditorCamera::m_editorViewMatrix.m_e11 ,EditorCamera::m_editorViewMatrix.m_e12 ,
                                                          EditorCamera::m_editorViewMatrix.m_e20 ,EditorCamera::m_editorViewMatrix.m_e21 ,EditorCamera::m_editorViewMatrix.m_e22 };



        //IF GAME IS RUNNING set editor camera as game camera

        if ((ecs::ECS::m_GetInstance()->m_getState() == ecs::RUNNING) && graphicpipe::GraphicsCamera::m_cameras.size() > 0)
        {
            ImGuiWindow* imguiWindow = ImGui::FindWindowByName(gamePreviewName.c_str());

            if (imguiWindow) {
                //temporary, unless someone whos good at math knows what to do
                EditorCamera::m_editorCamera.m_zoom = { 5.f, 5.f };

                EditorCamera::m_editorCamera.m_zoom.m_x = graphicpipe::GraphicsCamera::m_currCameraScaleX;
                EditorCamera::m_editorCamera.m_zoom.m_y = graphicpipe::GraphicsCamera::m_currCameraScaleY;
                EditorCamera::m_editorCamera.m_coordinates.m_x = graphicpipe::GraphicsCamera::m_currCameraTranslateX;
                EditorCamera::m_editorCamera.m_coordinates.m_y = graphicpipe::GraphicsCamera::m_currCameraTranslateY;
            }


        }

        ImGui::End();


	}

}