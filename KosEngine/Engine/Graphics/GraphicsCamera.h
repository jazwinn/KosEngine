/******************************************************************/
/*!
\file      GraphicsCamera.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file defines the GraphicsCamera class, which manages camera transformations
           and viewport settings for rendering in a graphics pipeline.

\details   The GraphicsCamera class is a singleton that provides static methods and members
           for managing the current view, orthographic projection, and aspect ratio.
           It includes functionality to adjust the camera's rotation, scale, and translation,
           and supports switching between multiple camera matrices.

           Dependencies:
           - GLM for matrix transformations and mathematical operations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


#ifndef GRAPHCAM_H
#define GRAPHCAM_H

#include <glm.hpp>
#include <vector>
#include <memory>

namespace graphicpipe

{
	class GraphicsCamera
	{
    public:
        static int m_windowWidth;                        ///< The current window width.
        static int m_windowHeight;                       ///< The current window height.
        static float m_aspectRatio;                      ///< Aspect ratio of the window.
   

        static void m_CalculateAspectRatio();
        static void m_CalculateCurrView();
        static void m_SetCurrCamera(unsigned int index);
        static void m_MultiplyOrthoMatrix();
        static void m_MultiplyViewMatrix();

        static GraphicsCamera* m_FuncGetInstance();

        static glm::mat3 m_currCameraMatrix;                ///< Current camera matrix
        static glm::mat3 m_currViewMatrix;
        static glm::mat3 m_currOrthoMatrix;

   

        static float m_currCameraRotate;
        static float m_currCameraScaleX;
        static float m_currCameraScaleY;
        static float m_currCameraTranslateX;
        static float m_currCameraTranslateY;

        static std::vector<glm::mat3> m_cameras;

    private:
        static std::unique_ptr<GraphicsCamera> m_instancePtr;
	};
}

#endif GRAPHCAM_H