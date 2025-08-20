/******************************************************************/
/*!
\file      Input.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      16 Sept, 2024
\brief     defines the class functions that contains all the callback functions that will be given to GLFW along with variables to hold the return values

The header provides declarations for the Performance class functions that
handle performance tracking and output for various engine systems such as
movement, rendering, and collision.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "Input.h"


namespace Input {
	/*--------------------------------------------------------------
	  GLOBAL VARAIBLE
	--------------------------------------------------------------*/

	vector2::Vec2 InputSystem::MousePosition = { 0,0 };
	std::string InputSystem::m_mouseString;
	std::string InputSystem::m_keyString;
	GLFWwindow* Input::InputSystem::m_windowInput;
	std::vector<std::string> InputSystem::m_droppedFiles;
	bool InputSystem::m_controllerConnected = false;
	int InputSystem::m_controllerID = NULL;
	std::vector<float> InputSystem::m_controllerAxes(6, 0);
	float InputSystem::ControllerSensitivity = 20.0f;
	float InputSystem::deadzone = 0.4f;
	float InputSystem::controllerRightJoyStickRotation = -1.f;
	bool InputSystem::overwriteMousePosWithController = false;
	std::string InputSystem::m_controllerName = "";
	const std::string InputSystem::ps5Mapping = "030000004c050000e60c000000000000, PS5 Controller, a:b1, b : b2, back : b8, dpdown : h0.4, dpleft : h0.8, dpright : h0.2, dpup : h0.1, guide : b12, leftshoulder : b4, leftstick : b10, lefttrigger : a3, leftx : a0, lefty : a1, misc1 : b13, rightshoulder : b5, rightstick : b11, righttrigger : a4, rightx : a2, righty : a5, start : b9, x : b0, y : b3, platform : Windows,";
	const std::string InputSystem::xboxMapping = "78696e70757401000000000000000000,XInput Gamepad (GLFW),platform:Windows,a:b0,b:b1,x:b2,y:b3,leftshoulder:b4,rightshoulder:b5,back:b6,start:b7,leftstick:b8,rightstick:b9,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:a4,righttrigger:a5,dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,";

	void InputSystem::KeyCallBack([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
		if (action == GLFW_PRESS) {
			InputSystem::m_keyString = "key pressed!";
		}
		else if (action == GLFW_RELEASE) {
			InputSystem::m_keyString = "-";
		}
		else if (action == GLFW_REPEAT) {
			InputSystem::m_keyString = "Key Repeated";
		}
	}


	void InputSystem::MouseButtonCallBack([[maybe_unused]] GLFWwindow* pwin, [[maybe_unused]] int button, [[maybe_unused]] int action, [[maybe_unused]] int mod) {
	
		if (action == GLFW_PRESS) {
			// key is press
			InputSystem::m_mouseString = "Mouse pressed!";
		}
		else if (action == GLFW_RELEASE) {
			InputSystem::m_mouseString = "-";
		}
		else if (action == GLFW_REPEAT) {
			InputSystem::m_mouseString = "Mouse Repeated";
		}
		
	}


	void InputSystem::mousepos_cb(GLFWwindow* pwin, double xpos, double ypos) {
		int width{}, height{};
		
		glfwGetWindowSize(pwin, &width, &height);
		//change origin from top left to bottom left
		ypos = static_cast<double>(height - ypos);

		MousePosition.m_x = static_cast<float>(xpos);
		MousePosition.m_y = static_cast<float>(ypos);
		//std::cout << xpos << " : " << ypos << std::endl;

	}

	void InputSystem::joystick_callback(int jid, int event)
	{
		if (event == GLFW_CONNECTED)
		{
			//std::cout << "Joystick " << jid << " connected" << std::endl;
			m_controllerConnected = true;
			m_controllerID = jid;
			m_controllerName = glfwGetGamepadName(jid);
			std::cout << m_controllerName << std::endl;
			if (m_controllerName == "XInput Gamepad (GLFW)") {
				glfwUpdateGamepadMappings(xboxMapping.c_str());
			}
			else if (m_controllerName == "PS5 Controller") {
				std::cout << "updated mappings" << std::endl;
				glfwUpdateGamepadMappings(ps5Mapping.c_str());
			}

		}
		else if (event == GLFW_DISCONNECTED)
		{
			//std::cout << "Joystick " << jid << " disconnected" << std::endl;
			m_controllerConnected = false;
			m_controllerID = NULL;
		}

	}
	

	void InputSystem::dropCallback([[maybe_unused]] GLFWwindow* window, int count, const char** paths) {
		m_droppedFiles.clear();
		for (int i = 0; i < count; ++i) {
			m_droppedFiles.emplace_back(paths[i]);
		}
	}

	void InputSystem::SetCallBack(GLFWwindow* Window) {
		glfwSetKeyCallback(Window, KeyCallBack);
		glfwSetDropCallback(Window, dropCallback);
		glfwSetMouseButtonCallback(Window, MouseButtonCallBack);
		glfwSetCursorPosCallback(Window, mousepos_cb);
		glfwSetJoystickCallback(joystick_callback);
		
	}

	void InputSystem::m_HideCursor(bool _boolean)
	{
		if (_boolean) {
			glfwSetInputMode(m_windowInput, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(m_windowInput, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

	}

	void InputSystem::m_inputUpdate() {
		giveControllerMousePos();
		for (int controlID = GLFW_JOYSTICK_1; controlID <= GLFW_JOYSTICK_LAST; controlID++) {
			if (glfwJoystickPresent(controlID)) {
				m_controllerConnected = true;
				m_controllerID = controlID;
				m_controllerName = glfwGetGamepadName(controlID);
				//if (glfwJoystickIsGamepad(controlID)) {
				//	std::cout << "yepcock" << std::endl;
				//}
				//if (m_controllerName == "XInput Gamepad (GLFW)") {
				//	glfwUpdateGamepadMappings(xboxMapping.c_str());
				//}
				//else if (m_controllerName == "PS5 Controller") {
				//	glfwUpdateGamepadMappings(ps5Mapping.c_str());
				//}
			}
			
		}
		for (auto& currKey : m_wasTriggered) {
			int state;
			if (currKey.first == keys::LMB || currKey.first == keys::RMB || currKey.first == keys::MMB) {
				state = glfwGetMouseButton(m_windowInput, currKey.first);
			}
			else {
				state = glfwGetKey(m_windowInput, currKey.first);
			}

			if (!m_wasPressed[currKey.first] && state == GLFW_PRESS) {
				m_wasPressed[currKey.first] = true;
				m_wasTriggered[currKey.first] = true;
			}
			else if (m_wasPressed[currKey.first]) {
				m_wasTriggered[currKey.first] = false;
			}
			if (state == GLFW_RELEASE) {
				if (m_wasPressed[currKey.first] || m_wasTriggered[currKey.first]) {
					m_wasReleased[currKey.first] = true;
				}
				else {
					m_wasReleased[currKey.first] = false;
				}
				m_wasPressed[currKey.first] = false;
				m_wasTriggered[currKey.first] = false;
			}
		}
		
		if(m_controllerConnected){
			GLFWgamepadstate state;
			if (glfwGetGamepadState(m_controllerID, &state))
			{
				int store = 0;
				const float* axes = glfwGetJoystickAxes(m_controllerID, &store);
				if (m_controllerName == "XInput Gamepad (GLFW)") {
					for (int i = 0; i < store; ++i)  // GLFW_GAMEPAD_AXIS_LAST is 5
					{

						if (i == 4) {
							if (axes[i] > 0.f) {
								if (!m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER]) {
									m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER] = true;
									m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER] = true;
								}
								else {
									m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER] = false;
								}
							}
							else {
								if (m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER] || m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER]) {
									m_wasControllerReleased[keys::CONTROLLER_LEFT_TRIGGER] = true;
								}
								else {
									m_wasControllerReleased[keys::CONTROLLER_LEFT_TRIGGER] = false;
								}
								m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER] = false;
								m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER] = false;
							}

						}
						else if (i == 5) {
							if (axes[i] > 0.f) {
								if (!m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER]) {
									m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER] = true;
									m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER] = true;
								}
								else {
									m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER] = false;
								}
							}
							else {
								if (m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER] || m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER]) {
									m_wasControllerReleased[keys::CONTROLLER_RIGHT_TRIGGER] = true;
								}
								else {
									m_wasControllerReleased[keys::CONTROLLER_RIGHT_TRIGGER] = false;
								}
								m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER] = false;
								m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER] = false;
							}

						}
						else if (i == 1 || i == 3) {
							m_controllerAxes[i] = -axes[i];
						}
						else {
							m_controllerAxes[i] = axes[i];
						}
						if (m_controllerAxes[i] < 0.001f && m_controllerAxes[i] > -0.001f) {
							m_controllerAxes[i] = 0.f;
						}
					}
				}
				else {
					//i = 4 is right trigger
					//i = 5 is right y axis
					//i = 2 is right x axis
					//i = 1 is left y axis
					//i = 0 is left x axis
				//	int store = 0;
					//const float* axes = glfwGetJoystickAxes(m_controllerID, &store);
					for (int i = 0; i < store; ++i)  // GLFW_GAMEPAD_AXIS_LAST is 5
					{
						if (i == 3) {
							if (axes[i] > 0.f) {
								if (!m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER]) {
									m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER] = true;
									m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER] = true;
								}
								else {
									m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER] = false;
								}
							}
							else {
								if (m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER] || m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER]) {
									m_wasControllerReleased[keys::CONTROLLER_LEFT_TRIGGER] = true;
								}
								else {
									m_wasControllerReleased[keys::CONTROLLER_LEFT_TRIGGER] = false;
								}
								m_controllerButtonsPress[keys::CONTROLLER_LEFT_TRIGGER] = false;
								m_controllerButtonsTriggered[keys::CONTROLLER_LEFT_TRIGGER] = false;
							}
						}
						else if (i == 4) {
							if (axes[i] > 0.f) {
								if (!m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER]) {
									m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER] = true;
									m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER] = true;
								}
								else {
									m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER] = false;
								}
							}
							else {
								if (m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER] || m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER]) {
									m_wasControllerReleased[keys::CONTROLLER_RIGHT_TRIGGER] = true;
								}
								else {
									m_wasControllerReleased[keys::CONTROLLER_RIGHT_TRIGGER] = false;
								}
								m_controllerButtonsPress[keys::CONTROLLER_RIGHT_TRIGGER] = false;
								m_controllerButtonsTriggered[keys::CONTROLLER_RIGHT_TRIGGER] = false;
							}

						}
						else if (i == 5) {
							m_controllerAxes[3] = -axes[i];
						}
						else if (i == 1) {
							m_controllerAxes[i] = -axes[i];
						}
						else {
							m_controllerAxes[i] = axes[i];
						}
						
						
						if (m_controllerAxes[i] < 0.001f && m_controllerAxes[i] > -0.001f) {
							m_controllerAxes[i] = 0.f;
						}
					}
				}
				// Update button states
				for (int i = 0; i < 15; ++i)  // GLFW_GAMEPAD_BUTTON_LAST is 14
				{
					if (state.buttons[i] == GLFW_PRESS)
					{
						if (!m_controllerButtonsPress[i]) {
							m_controllerButtonsTriggered[i] = true;
							m_controllerButtonsPress[i] = true;
						}
						else if (m_controllerButtonsPress[i]) {
							m_controllerButtonsTriggered[i] = false;
						}
						//m_controllerButtonsPress[i] = true;
					}
					else
					{
						if (m_controllerButtonsPress[i] || m_controllerButtonsTriggered[i]) {
							m_wasControllerReleased[i] = true;
						}
						else {
							m_wasControllerReleased[i] = false;
						}
						m_controllerButtonsPress[i] = false;
						m_controllerButtonsTriggered[i] = false;
					}
				}


			}
		}
	}

	void InputSystem::giveControllerMousePos()
	{



		//if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {

		//}


		if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
			if (m_controllerName == "XInput Gamepad (GLFW)") {
				if (overwriteMousePosWithController) {
					int axesCount;
					const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

					if (axesCount >= 2) {
						float stickX = axes[2];
						float stickY = axes[3];
						GLFWwindow* window = glfwGetCurrentContext();
						double mouseX, mouseY;
						glfwGetCursorPos(window, &mouseX, &mouseY);


						if (fabs(stickX) > deadzone || fabs(stickY) > deadzone) {
							mouseX += stickX * ControllerSensitivity;
							mouseY += stickY * ControllerSensitivity; // Invert Y if needed
							glfwSetCursorPos(window, mouseX, mouseY);
						}
					}


				}

				int count;
				const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

				if (count >= 2) { // Ensure the joystick has at least two axes
					float x = axes[2]; // X-axis
					float y = axes[3]; // Y-axis (negate to correct inversion)

					if (x == 0.0f && y == 0.0f) {
						controllerRightJoyStickRotation = -1;
						return;
					}
					if (fabs(x) < deadzone && fabs(y) < deadzone) {
						controllerRightJoyStickRotation = -1;
						return;
					}


					float angle = atan2(x, -y) * 57.2958f; // Swap x and y
					//std::cout << angle << std::endl;
					if (angle < 0) angle += 360.0f; // Normalize to [0, 360)

					controllerRightJoyStickRotation = angle;

				}
			}
			else{
				if (overwriteMousePosWithController) {
					int axesCount{};
					glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

					if (axesCount >= 2) {
						float stickX = m_controllerAxes[2];
						float stickY = -m_controllerAxes[3];
						GLFWwindow* window = glfwGetCurrentContext();
						double mouseX, mouseY;
						glfwGetCursorPos(window, &mouseX, &mouseY);
						if (fabs(stickX) > deadzone || fabs(stickY) > deadzone) {
							mouseX += stickX * ControllerSensitivity;
							mouseY += stickY * ControllerSensitivity; // Invert Y if needed
							glfwSetCursorPos(window, mouseX, mouseY);
						}
					}
				}

				int count{};
				glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

				if (count >= 2) { // Ensure the joystick has at least two axes
					float x = m_controllerAxes[2]; // X-axis
					float y = -m_controllerAxes[3]; // Y-axis (negate to correct inversion)

					if (x == 0.0f && y == 0.0f) {
						controllerRightJoyStickRotation = -1;
						return;
					}
					if (fabs(x) < deadzone && fabs(y) < deadzone) {
						controllerRightJoyStickRotation = -1;
						return;
					}


					float angle = atan2(x, -y) * 57.2958f; // Swap x and y
					//std::cout << angle << std::endl;
					if (angle < 0) angle += 360.0f; // Normalize to [0, 360)

					controllerRightJoyStickRotation = angle;

				}
			}
		}
		else {
			controllerRightJoyStickRotation = -1;
		}
		
		//std::cout << controllerRightJoyStickRotation << std::endl;
		return; // Joystick not present or invalid axes
	}


	bool InputSystem::m_isKeyTriggered(const keyCode givenKey) {
		return m_wasTriggered[givenKey];
	}
	bool InputSystem::m_isKeyPressed(const keyCode givenKey) {
		int state;
		if (givenKey == keys::LMB || givenKey == keys::RMB || givenKey == keys::MMB) {
			state = glfwGetMouseButton(m_windowInput, givenKey);
		}
		else {
			state = glfwGetKey(m_windowInput, givenKey);
		}

		return state == GLFW_PRESS ? true : false;
	}
	bool InputSystem::m_isKeyReleased(const keyCode givenKey) {
		return m_wasReleased[givenKey];
	}

	vector2::Vec2 InputSystem::m_getMousePosition() {
		return InputSystem::MousePosition;
	}

	bool InputSystem::m_isControllerButtonTriggered(const keyCode givenKey) {
		return m_controllerButtonsTriggered[givenKey];
	}

	bool InputSystem::m_isControllerButtonPressed(const keyCode givenKey)
	{
		return m_controllerButtonsPress[givenKey];
	}

	bool InputSystem::m_isControllerButtonReleased(const keyCode givenKey)
	{
		return m_wasControllerReleased[givenKey];

	}

	float InputSystem::m_getControllerAxis(const keyCode givenAxis)
	{
		return m_controllerAxes[givenAxis];
	}

	std::vector<float> InputSystem::m_getControllerAxiss(){
		return m_controllerAxes;
	}

}