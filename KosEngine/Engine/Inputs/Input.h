/******************************************************************/
/*!
\file      Input.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      16 Sept, 2024
\brief     Declares a class that contains all the callback functions that will be given to GLFW along with variables to hold the return values

The header provides declarations for the Performance class functions that
handle performance tracking and output for various engine systems such as
movement, rendering, and collision.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#ifndef INPUT_H
#define INPUT_H

#include <glfw3.h>
#include "Keycodes.h"
#include "../Helper/Helper.h"

namespace Input {

	enum CONTROLLER_TYPE {
		XBOX,
		PS5
	};

	class InputSystem {

	public:

		/*
		\brief	This function is meant to be given to GLFW as a way for GLFW to register key inputs and change the variables in this class
		*/
		static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void dropCallback(GLFWwindow* window, int count, const char** paths);

		/*
		\brief	This function is meant to be given to GLFW as a way for GLFW to register mouse button inputs and change the variables in this class
		*/
		static void MouseButtonCallBack(GLFWwindow* pwin, int button, int action, int mod);

		/*
		\brief	This function is meant to be given to GLFW as a way for GLFW to register mouse movement		inputs and change the variables in this class
		*/
		static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

		/*
		\brief	This function is meant to be called when GLFW is initializing so as to bind the callback functions with GLFW
		*/
		void SetCallBack(GLFWwindow* window);

		static void m_HideCursor(bool);

		static bool m_isKeyTriggered(const keyCode givenKey);
		static bool m_isKeyPressed(const keyCode givenKey);
		static bool m_isKeyReleased(const keyCode givenKey);
		static vector2::Vec2 m_getMousePosition();

		static void joystick_callback(int jid, int event);

		static bool m_isControllerButtonPressed(const keyCode givenKey);
		static bool m_isControllerButtonTriggered(const keyCode givenKey);
		static bool m_isControllerButtonReleased(const keyCode givenKey);
		static float m_getControllerAxis(const keyCode);
		static std::vector<float> m_getControllerAxiss();

		void m_inputUpdate();

		static void giveControllerMousePos();
		
		static vector2::Vec2 MousePosition;

		static std::string m_mouseString;
		static std::string m_keyString;
		static bool m_controllerConnected;
		static int m_controllerID;

		static std::vector<std::string> m_droppedFiles;
		static std::vector<float> m_controllerAxes;
		static float controllerRightJoyStickRotation;

		static GLFWwindow* m_windowInput;

		static float ControllerSensitivity;
		static float deadzone;

		static bool overwriteMousePosWithController;
		static std::string m_controllerName;
		static const std::string ps5Mapping;
		static const std::string xboxMapping;
	private:
		inline static std::unordered_map<int, bool> m_wasTriggered;
		inline static std::unordered_map<int, bool> m_wasPressed;
		inline static std::unordered_map<int, bool> m_wasReleased;
		inline static std::unordered_map<int, bool> m_controllerButtonsPress;
		inline static std::unordered_map<int, bool> m_controllerButtonsTriggered;
		inline static std::unordered_map<int, bool> m_wasControllerReleased;


	};

}




#endif INPUT_H
