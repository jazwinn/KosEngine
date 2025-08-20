/******************************************************************/
/*!
\file      EditorCamera.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th November, 2024
\brief     This file contains the implementation of the EditorCamera
           class, which provides camera control for the editor view
           in a graphical application.

           The EditorCamera is responsible for setting up and managing
           the orthographic view, including the camera's zoom, drag
           sensitivity, and aspect ratio. It provides methods to:
           - Calculate the camera transformation matrix.
           - Calculate the view matrix based on the camera position.
           - Set up an orthographic projection matrix.

           These calculations enable smooth zooming and dragging within
           the editor, ensuring the camera view can be adjusted
           interactively by the user.

           Key features of this file:
           - Configurable camera zoom and drag sensitivity.
           - Camera matrix, view matrix, and orthographic projection.
           - Static members to control aspect ratio and window size.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/



#include "../Editor/EditorCamera.h"
#include "Graphics/GraphicsCamera.h"
#include "Math/Mat3x3.h"
#include "Math/vector3.h"
#include <iostream>

float EditorCamera::m_aspectRatio{};
int EditorCamera::m_windowWidth{};
int EditorCamera::m_windowHeight{};

vector2::Vec2 EditorCamera::m_editorWindowPosition{};
vector2::Vec2 EditorCamera::m_editorWindowDimensions{};
vector2::Vec2 EditorCamera::m_gamePreviewWindowPosition{};
vector2::Vec2 EditorCamera::m_gamePreviewWindowDimensions{};
bool EditorCamera::m_editorMode = true;
EditorCamera::Camera EditorCamera::m_editorCamera{ 1.f, 1.f ,vector2::Vec2(0.f,0.f), vector2::Vec2(5.f,5.f), 0 }; // Zoom Set To 5.f on Init
mat3x3::Mat3x3 EditorCamera::m_editorCameraMatrix{};
mat3x3::Mat3x3 EditorCamera::m_editorViewMatrix{};
mat3x3::Mat3x3 EditorCamera::m_editorOrthoMatrix{};

float EditorCamera::m_editorCameraDragSensitivity{ 0.01f };
float EditorCamera::m_editorCameraZoomSensitivity{ 0.10f };

void EditorCamera::calculateLevelEditorCamera()
{
	m_editorCameraMatrix.m_e00 = m_editorCamera.m_zoom.m_x;
	m_editorCameraMatrix.m_e11 = m_editorCamera.m_zoom.m_y;
	m_editorCameraMatrix.m_e20 = m_editorCamera.m_coordinates.m_x;
	m_editorCameraMatrix.m_e21 = m_editorCamera.m_coordinates.m_y;
	m_editorCameraMatrix.m_e22 = 1;
}

void EditorCamera::calculateLevelEditorView()
{
    mat3x3::Mat3Inverse(m_editorCameraMatrix, m_editorViewMatrix);
}

void EditorCamera::calculateLevelEditorOrtho()
{
    if (graphicpipe::GraphicsCamera::m_aspectRatio > 0)
    {
        float left = -1.f * (1.f / graphicpipe::GraphicsCamera::m_aspectRatio);
        float right = 1.f * (1.f / graphicpipe::GraphicsCamera::m_aspectRatio);
        float bottom = -1.f;
        float top = 1.f;
        m_editorOrthoMatrix.m_e00 = (2.0f / (right - left));
        m_editorOrthoMatrix.m_e11 = 2.0f / (top - bottom);
        m_editorOrthoMatrix.m_e20 = -(right + left) / (right - left);
        m_editorOrthoMatrix.m_e21 = -(top + bottom) / (top - bottom);
        m_editorOrthoMatrix.m_e22 = 1;
    }
    
}

vector2::Vec2 EditorCamera::calculateWorldCoordinatesFromMouse(int mouseX, int mouseY)
{
    float screencoordX = mouseX - m_editorWindowPosition.m_x;
    float screencoordY = mouseY - m_editorWindowPosition.m_y;

    float cordX = (screencoordX - m_editorWindowDimensions.m_x / 2.f) / (m_editorWindowDimensions.m_x / 2.f);
    float cordY = (std::abs(screencoordY) - m_editorWindowDimensions.m_y / 2.f) / (m_editorWindowDimensions.m_y / 2.f);

    vector3::Vec3 translate = { cordX , -cordY, 0.f };
    translate.m_x *= EditorCamera::m_editorCameraMatrix.m_e00;
    translate.m_y *= EditorCamera::m_editorCameraMatrix.m_e11;
    if (graphicpipe::GraphicsCamera::m_aspectRatio != 0)
    {
        translate.m_x *= 1.f / graphicpipe::GraphicsCamera::m_aspectRatio;
    }
    translate.m_x += EditorCamera::m_editorCameraMatrix.m_e20;
    translate.m_y += EditorCamera::m_editorCameraMatrix.m_e21;

    return vector2::Vec2{ translate.m_x, translate.m_y };
}
