/******************************************************************/
/*!
\file      EditorCamera.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th November, 2024
\brief     This file defines the EditorCamera class, which manages
           the editor-specific camera properties and transformations.
           It includes functions for calculating the camera matrix,
           view matrix, and orthographic projection matrix for
           rendering scenes in an editor environment.

           The EditorCamera is designed specifically for use within
           the level editor, providing:
           - Configurable zoom and drag sensitivities.
           - Static transformations that apply to all instances of
             the editor.
           - Orthographic projections optimized for a consistent
             editor camera view.

           Key features:
           - Camera struct for position and orientation settings.
           - Methods to calculate matrices used for rendering.
           - Adjustable parameters for drag and zoom sensitivity.

           The class is intended for managing the viewport within
           a level editor, allowing for smooth camera controls.

           NOTE: This header file is meant only for editor camera
           configuration, and not for in-game camera use.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef m_editorOrthoMatrixEDITORCAM_H
#define EDITORCAM_H

#include <vector>
#include "Math/vector2.h"
#include "Math/Mat3x3.h"


    /**
     * @class EditorCamera
     * @brief Manages the level editor's camera, providing static camera
     *        transformations and view configuration for an editor scene.
     */
    class EditorCamera
    {
    public:
        static int m_windowWidth;                        ///< The current window width.
        static int m_windowHeight;                       ///< The current window height.
        static float m_aspectRatio;                      ///< Aspect ratio of the window.


        static vector2::Vec2 m_editorWindowPosition;
        static vector2::Vec2 m_editorWindowDimensions;
        static vector2::Vec2 m_gamePreviewWindowPosition;
        static vector2::Vec2 m_gamePreviewWindowDimensions;

        static bool m_editorMode;
        /**
         * @struct Camera
         * @brief Represents the editor camera's configuration, specifically
         *        its position, zoom level, and orientation in the editor.
         */
        struct Camera                  ///< ONLY MEANT FOR LEVEL EDITOR CAMERA
        {
            float upVector;
            float rightVector;
            vector2::Vec2 m_coordinates;   ///< Coordinates of the camera.
            vector2::Vec2 m_zoom{1,1};          ///< Zoom of the a=canera
            float m_angle;             ///< Rotation angle of the camera.
        };

        static Camera m_editorCamera;
        static mat3x3::Mat3x3 m_editorCameraMatrix;
        static mat3x3::Mat3x3 m_editorViewMatrix;
        static mat3x3::Mat3x3 m_editorOrthoMatrix;
        static float m_editorCameraDragSensitivity;  ///< Editor Camera Drag Sensitivity
        static float m_editorCameraZoomSensitivity;  ///< Editor Camera Zoom Sensitivity

        /**
        * @brief Calculates the transformation matrix for the editor camera.
        */
        static void calculateLevelEditorCamera();
        /**
         * @brief Calculates the view matrix for the editor camera.
         */
        static void calculateLevelEditorView();
        /**
         * @brief Calculates the orthographic projection matrix for the editor camera.
         */
        static void calculateLevelEditorOrtho();


        static vector2::Vec2 calculateWorldCoordinatesFromMouse(int mouseX, int mouseY);



    };


#endif GRAPHCAM_H