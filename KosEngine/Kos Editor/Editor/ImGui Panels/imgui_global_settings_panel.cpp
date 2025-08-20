/******************************************************************/
/*!
\file      imgui_global_settings_panel.cpp
\author    Sean Tiu, s.tiu , 2303398
\par       s.tiu@digipen.edu
\date      31 January 2025
\brief     This file helps with implementing global settings for the editor

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "implot.h"
#include "Debugging/Logging.h"
#include "Debugging/Performance.h"
#include <ECS/System/SystemType.h>
#include "ECS/Layers.h"
#include "Debugging/Logging.h"
#include "Helper/Helper.h"
#include "Graphics/GraphicsPipe.h"




void gui::ImGuiHandler::m_DrawGlobalSettingsWindow() 
{
    ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_HorizontalScrollbar);

    graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
    Helper::Helpers* helper = Helper::Helpers::GetInstance();
    ImVec4 color = ImVec4(helper->m_colour.m_x, helper->m_colour.m_y, helper->m_colour.m_z, 255.0f / 255.0f);

    ImGui::Text("Global Brightness");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 100.0f);  // Push slider to the right
    ImGui::SetNextItemWidth(100.0f);
    ImGui::DragFloat("##Brightness", &pipe->m_globalLightIntensity, 0.02f, 0.0f, 2.0f, "%.2f");

    ImGui::Text("Background Color");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 200.0f);  // Push color picker to the right
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::ColorEdit3("##MyColor3", (float*)&color, ImGuiColorEditFlags_DisplayRGB))
    {
        helper->m_colour.m_x = color.x;
        helper->m_colour.m_y = color.y;
        helper->m_colour.m_z = color.z;
    }
  

    // Dropdown menu for cursor selection
    static const char* cursorOptions[] = 
    {
        "default",
        "../Assets/Texture/UI/img_startScreenCursor.png",
        "../Assets/Texture/UI/img_inGameCursor.png"
    };


    static int selectedCursorIndex = (helper->m_currMousePicture == "../Assets/Texture/UI/img_startScreenCursor.png") ? 1 :
                                      (helper->m_currMousePicture == "../Assets/Texture/UI/img_inGameCursor.png") ? 2: 0;// Keep track of selected cursor index

    ImGui::Text("Cursor Image:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::Combo("##CursorDropdown", &selectedCursorIndex, cursorOptions, IM_ARRAYSIZE(cursorOptions)))
    {
        // Update the cursor image selection
        helper->m_currMousePicture = cursorOptions[selectedCursorIndex];
    }

    // Checkbox for cursor centering
    ImGui::Text("Center Cursor:");
    ImGui::SameLine();
    ImGui::Checkbox("##CursorCentered", &helper->m_isMouseCentered);

    ImGui::End();
}

    