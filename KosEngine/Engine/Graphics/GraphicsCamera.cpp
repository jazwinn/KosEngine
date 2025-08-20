/******************************************************************/
/*!
\file      GraphicsCamera.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file implements the GraphicsCamera class, which manages camera transformations
		   and viewport settings for rendering in a graphics pipeline.

\details   The GraphicsCamera class is a singleton that provides static methods and members
		   for managing the current view, orthographic projection, and aspect ratio.
		   It includes functionality to adjust the camera's rotation, scale, and translation,
		   and supports switching between multiple camera matrices. This file contains the
		   implementation of camera transformation calculations, matrix multiplications,
		   and the management of the current camera state.

		   Key functions:
		   - `m_funcGetInstance()`: Returns the single instance of the `GraphicsCamera` class.
		   - `setCurrCamera()`: Sets the current camera matrix.
		   - `calculateAspectRatio()`: Calculates the aspect ratio based on the window size.
		   - `calculateCurrView()`: Calculates the current view matrix by inverting the camera matrix.
		   - `m_MultiplyOrthoMatrix()`: Applies an orthographic projection matrix and updates the pipeline matrices.
		   - `m_MultiplyViewMatrix()`: Applies the current view matrix to the model matrices in the graphics pipeline.

		   Dependencies:
		   - GraphicsPipe.h for pipeline matrix management.
		   - Helper.h for access to window dimensions.
		   - GLM for matrix transformations and mathematical operations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Helper/Helper.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	int GraphicsCamera::m_windowWidth{};
	int GraphicsCamera::m_windowHeight{};
	float GraphicsCamera::m_aspectRatio{1.f};
	std::unique_ptr<GraphicsCamera> GraphicsCamera::m_instancePtr = nullptr;
	glm::mat3 GraphicsCamera::m_currCameraMatrix{1.f};
	glm::mat3 GraphicsCamera::m_currViewMatrix{ 1.f };
	glm::mat3 GraphicsCamera::m_currOrthoMatrix{ 1.f };
	std::vector<glm::mat3> GraphicsCamera::m_cameras{};
	float GraphicsCamera::m_currCameraRotate{};
	float GraphicsCamera::m_currCameraScaleX{ 1.f };
	float GraphicsCamera::m_currCameraScaleY{ 1.f };
	float GraphicsCamera::m_currCameraTranslateX{};
	float GraphicsCamera::m_currCameraTranslateY{};

	GraphicsCamera* GraphicsCamera::m_FuncGetInstance()
	{
		if (!m_instancePtr)
		{
			m_instancePtr.reset(new GraphicsCamera{});
		}
		return m_instancePtr.get();
	}

	void GraphicsCamera::m_SetCurrCamera(unsigned int index)
	{
		m_currCameraMatrix = m_cameras[index];
	}

	void GraphicsCamera::m_CalculateAspectRatio()
	{
		m_windowWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
		m_windowHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);
		if (m_windowWidth != 0)
		{
			m_aspectRatio = static_cast<float>(static_cast<float>(m_windowHeight) / static_cast<float>(m_windowWidth));

		}
	}

	void GraphicsCamera::m_CalculateCurrView()
	{
		m_currViewMatrix = glm::inverse(m_currCameraMatrix);
	}

	void GraphicsCamera::m_MultiplyOrthoMatrix()
	{
		//GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();
		float left = -1.f * (1.f/ m_aspectRatio);
		float right = 1.f * (1.f /m_aspectRatio);
		float bottom = -1.f;
		float top = 1.f;
		m_currOrthoMatrix[0][0] = (2.0f / (right - left));
		m_currOrthoMatrix[1][1] = 2.0f / (top - bottom);
		m_currOrthoMatrix[2][0] = -(right + left) / (right - left);
		m_currOrthoMatrix[2][1] = -(top + bottom) / (top - bottom);
		m_currOrthoMatrix[2][2] = 1;

		/*for (glm::mat3& matrix : pipe->m_modelToNDCMatrix)
		{
			matrix = m_currOrthoMatrix * matrix;
		}*/

		/*for (glm::mat3& debugMatrix : pipe->m_debugBoxToNDCMatrix)
		{
			debugMatrix = m_currOrthoMatrix * debugMatrix;
		}

		for (glm::mat3& debugMatrix : pipe->m_debugCircleToNDCMatrix)
		{
			debugMatrix = m_currOrthoMatrix * debugMatrix;
		}*/
		//m_cameras.clear();
	}

	void GraphicsCamera::m_MultiplyViewMatrix()
	{
		//GraphicsPipe* pipe = GraphicsPipe::m_funcGetInstance();
		/*if (!(pipe->m_modelToNDCMatrix.empty()))
		{
			pipe->m_modelToNDCMatrix.clear();
		}
		for (const glm::mat3& matrix : pipe->m_modelMatrix)
		{
			pipe->m_modelToNDCMatrix.push_back(m_currViewMatrix * matrix);
		}*/
		/*for (glm::mat3& debugMatrix : pipe->m_debugBoxToNDCMatrix)
		{
			debugMatrix = m_currViewMatrix * debugMatrix;
		}
		for (glm::mat3& debugMatrix : pipe->m_debugCircleToNDCMatrix)
		{
			debugMatrix = m_currViewMatrix * debugMatrix;
		}*/

	
	}
	
}