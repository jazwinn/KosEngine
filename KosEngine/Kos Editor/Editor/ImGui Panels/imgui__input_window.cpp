/******************************************************************/
/*!
\file      Imgui_input_window.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the draw input window
		   function. It draws the input window to show data.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"

#include"Inputs/Input.h"

void gui::ImGuiHandler::m_DrawInputWindow() {

	bool open = true;
	ImGui::Begin("Inputs", &open);

	std::string mousePos = "Mouse Position: (" + std::to_string(static_cast<int>(Input::InputSystem::MousePosition.m_x)) + " , " + std::to_string(static_cast<int>(Input::InputSystem::MousePosition.m_y)) + ")";
	ImGui::Text(mousePos.c_str());
	
	std::string keyStatus = "Key Status: " + Input::InputSystem::m_keyString;
	ImGui::Text(keyStatus.c_str());

	std::string mouseStatus = "MouseButton Status: " + Input::InputSystem::m_mouseString;
	ImGui::Text(mouseStatus.c_str());

	ImGui::End();
}
