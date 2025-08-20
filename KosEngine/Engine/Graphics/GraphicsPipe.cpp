/******************************************************************/
/*!
\file      GraphicsPipe.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file implements the `GraphicsPipe` class functions for setting up and managing
		   the rendering pipeline, including initializing shaders, buffers, and rendering models
		   and debug geometries.

\details   The `GraphicsPipe` class provides functionality for initializing rendering settings,
		   setting up VAOs (Vertex Array Objects), and compiling shader programs for various
		   tasks like drawing models, debug shapes, and text. It also handles the transformation
		   of models from world space to normalized device coordinates (NDC) and facilitates the
		   offscreen rendering through framebuffers. Additionally, the class manages the current
		   camera state and updates transformations as per the game or editor mode.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"
#include "../Asset Manager/AssetManager.h"
#include "../Helper/Helper.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "../ECS/ECSList.h"
//#include "../Editor/EditorCamera.h"

namespace graphicpipe {


	std::unique_ptr<GraphicsPipe> GraphicsPipe::m_instancePtr = nullptr;


	void GraphicsPipe::m_funcInit()
	{
		//Enable transparency for images
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(4.f);
		m_squareLinesMesh.m_shapeType = SQUARE_LINES;
		m_testMatrix = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

		// Reserve memory for the maximum number of entities.
		m_modelData.reserve(ecs::MaxEntity);
		m_unlitModelData.reserve(ecs::MaxEntity);
		m_debugBoxData.reserve(ecs::MaxEntity);
		m_textData.reserve(ecs::MaxEntity);
		m_modelToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugBoxToNDCMatrix.reserve(ecs::MaxEntity);
		m_debugBoxCollisionChecks.reserve(ecs::MaxEntity);
		m_frameNumbers.reserve(ecs::MaxEntity);
		m_iVec3Array.reserve(ecs::MaxEntity);
		m_vec3Array.reserve(ecs::MaxEntity);
		m_layers.reserve(ecs::MaxEntity);

		// Set up VAOs for different shapes and text rendering.
		m_funcSetupVao(m_squareMesh);
		m_funcSetupFrameBufferVao();
		m_funcSetupCircleLinesVao();
		m_funcSetupSquareLinesVao();
		m_funcSetupGridVao();
		m_funcSetupTextVao();
		m_funcSetupVideoVAO();
		m_funcSetDrawMode(GL_FILL);

		// Compile and set up shader programs for various rendering tasks.
		m_gridDebugShaderProgram = m_funcSetupShader(gridDebugVertexShader, gridDebugFragmentShader);
		m_genericShaderProgram = m_funcSetupShader(genericVertexShader, genericFragmentShader);
		m_frameBufferShaderProgram = m_funcSetupShader(frameBufferVertexShader, frameBufferFragmentShader);
		m_debugShaderProgram = m_funcSetupShader(debugVertexShader, debugFragmentShader);
		m_textShaderProgram = m_funcSetupShader(textVertexShader, textFragmentShader);
		m_gridShaderProgram = m_funcSetupShader(gridVertexShader, gridFragmentShader);
		m_tilemapShaderProgram = m_funcSetupShader(tilemapVertexShader, tilemapFragmentShader);
		m_lightingShaderProgram = m_funcSetupShader(lightingVertexShader, lightingFragmentShader);
		m_finalPassShaderProgram = m_funcSetupShader(finalPassVertexShader, finalPassFragmentShader);
		m_videoShaderProgram = m_funcSetupShader(videoVertexShader,videoFragmentShader);
		m_particleComputerShaderProgram = m_funcSetupComputerShader(particleComputerShader);
		m_particleShaderProgram = m_funcSetupShader(particleVertexShader, particleFragmentShader);
		m_crtShaderProgram = m_funcSetupShader(crtVertexShader, crtFragmentShader);

		// Initialize model-to-NDC transformation matrix and other drawing data.
		m_modelToNDCMatrix.push_back(m_testMatrix);
		//m_vec3Array.push_back(0);
		m_frameNumbers.push_back(0);
		m_iVec3Array.push_back({ 0,0,0 });
		m_vec3Array.push_back({ 0,0,0 });
		m_layers.push_back(0);
		m_tileIndexes.push_back({0});
		m_colors.push_back({ 0.f, 0.f, 0.f, 0.f });
		m_debugBoxToNDCMatrix.push_back(m_testMatrix);
		m_debugBoxCollisionChecks.push_back(false);
		m_gridColliderChecks.push_back({ 0 });

		// Set up array buffer and framebuffers for offscreen rendering.
		m_funcSetupArrayBuffer();
		m_funcSetupFrameBuffer();
		m_funcSetupGamePreviewFrameBuffer();
		m_funcSetupMultiLightingFrameBuffer();
		m_funcSetupCRTFrameBuffer();
		m_funcSetupAdditiveLightingFrameBuffer();
		m_funcSetupFinalPassBuffer();
		m_funcSetupSSBO();

		// Clear temporary data structures used during setup.
		m_debugBoxToNDCMatrix.clear();
		m_debugBoxCollisionChecks.clear();
		m_modelToNDCMatrix.clear();
		m_vec3Array.clear();
		m_frameNumbers.clear();
		m_iVec3Array.clear();
		m_layers.clear();
		m_colors.clear();
		m_tileIndexes.clear();

		// Enable scissor test for limiting rendering to a specific area.
		//glEnable(GL_SCISSOR_TEST);


	}

	GraphicsPipe::~GraphicsPipe()
	{
		//delete instancePtr;
		if (m_frameBufferObject)
		{
			glDeleteFramebuffers(1, &m_frameBufferObject);
		}
		m_funcDeleteShader();
	}

	GraphicsPipe* GraphicsPipe::m_funcGetInstance()
	{
		if (!m_instancePtr)
		{
			m_instancePtr.reset(new GraphicsPipe{});
		}
		return m_instancePtr.get();
	}

	void GraphicsPipe::m_funcUpdate()
	{
		m_funcCalculateModelToWorldMatrix();
		m_spawnParticles();
		m_updateParticles();
		GraphicsCamera::m_CalculateAspectRatio();
		
		if (GraphicsCamera::m_cameras.size() > 0 && m_gameMode)
		{
			GraphicsCamera::m_SetCurrCamera(0);
			
			GraphicsCamera::m_CalculateCurrView();
		}
		GraphicsCamera::m_MultiplyViewMatrix();
		GraphicsCamera::m_MultiplyOrthoMatrix();

		if (!m_gameMode)
		{
			
			m_funcDrawWindow();
			//m_drawMultiLightingTexture();
		}
		
	}

	void GraphicsPipe::m_funcClearContainers()
	{
		m_modelToNDCMatrix.clear();
		m_vec3Array.clear();
		m_frameNumbers.clear();
		m_iVec3Array.clear();
		m_layers.clear();
		m_modelMatrix.clear();
		m_unlitModelMatrix.clear();
		m_unlitModelParams.clear();
		m_unlitLayers.clear();
		m_unlitColors.clear();
		m_unlitModelData.clear();
		m_modelData.clear();
		m_debugBoxToNDCMatrix.clear();
		m_debugBoxCollisionChecks.clear();
		m_debugCircleToNDCMatrix.clear();
		m_debugCircleCollisionChecks.clear();
		m_debugBoxData.clear();
		m_textData.clear();
		m_colors.clear();
		m_multiLightingData.clear();
		m_additiveLightingData.clear();
		m_multiLightingColors.clear();
		m_multiLightingTransforms.clear();
		m_multiLightingParams.clear();
		m_additiveLightingTransforms.clear();
		m_additiveLightingParams.clear();
		m_additiveLightingColors.clear();
		m_tilemapData.clear();
		m_transformedTilemaps.clear();
		m_tileIndexes.clear();
		m_tilemapIndexArrays.clear();
		m_gridColliderArrays.clear();
		m_gridColliderChecks.clear();
		m_colliderGridData.clear();
		m_videoData.clear();
		GraphicsCamera::m_cameras.clear();
		m_emitterData.clear();
		
	}

	void GraphicsPipe::m_funcRenderGameScene()
	{
		
		if (m_gameMode)
		{
			glClearColor(0,0,0,1.f);
			m_renderFinalPass();
			//m_funcDrawFullScreenQuad(m_finalPassTexture);
		}
		
		m_funcClearContainers();
	}

}
	

