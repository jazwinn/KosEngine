/******************************************************************/
/*!
\file      GraphicsBuffers.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file contains the implementation of functions in the GraphicsPipe class
		   for setting up OpenGL buffers and framebuffers, including operations
		   for rendering and managing textures, depth buffers, and framebuffers.

\details   The GraphicsPipe functions provided here include setup operations for instanced
		   drawing with buffer binding for model matrices, layers, texture order, strip counts,
		   and color data. Additionally, this file contains framebuffer setup functions
		   for general rendering as well as a game preview framebuffer.

		   Dependencies:
		   - GLEW for OpenGL extension loading.
		   - GLFW for window and context management.
		   - GLM for matrix operations and transformations.
		   - Helper class for accessing window size information.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Helper/Helper.h"

namespace graphicpipe
{
	void GraphicsPipe::m_funcSetupArrayBuffer()
	{
		///Square Mesh Buffer Setup
		glBindVertexArray(m_squareMesh.m_vaoId);
		glGenBuffers(1, &m_modelMatrixArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_modelToNDCMatrix.size() * sizeof(glm::mat3), &m_modelToNDCMatrix[0], GL_DYNAMIC_DRAW);
		glBindVertexArray(m_squareMesh.m_vaoId);
		unsigned int location = 6;
		for (int i = 0; i < 3; ++i)
		{
			glEnableVertexAttribArray(location + i);
			glVertexAttribPointer(location + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
			glVertexAttribDivisor(location + i, 1);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(m_squareMesh.m_vaoId);

		///Container Buffer Setup
		glGenBuffers(1, &m_tileIndexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_tileIndexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_tileIndexes.size() * sizeof(int), &m_tileIndexes[0][0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(15);
		glVertexAttribIPointer(15, 1, GL_INT, sizeof(int), (void*)0);
		glVertexAttribDivisor(15, 1);

		glGenBuffers(1, &m_layerBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_layerBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_layers.size() * sizeof(int), &m_layers[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(10);
		glVertexAttribIPointer(10, 1, GL_INT, sizeof(int), (void*)0);
		glVertexAttribDivisor(10, 1);


		glGenBuffers(1, &m_vec3Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vec3Buffer);
		glBufferData(GL_ARRAY_BUFFER, m_vec3Array.size() * sizeof(glm::vec3), &m_vec3Array[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribDivisor(5, 1);

		



		glGenBuffers(1, &m_iVec3Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_iVec3Buffer);
		glBufferData(GL_ARRAY_BUFFER, m_iVec3Array.size() * sizeof(glm::ivec3), &m_iVec3Array[0], GL_DYNAMIC_DRAW); // Strip, FrameNumber, Texture Order
		glEnableVertexAttribArray(4);
		glVertexAttribIPointer(4, 3, GL_INT, sizeof(glm::ivec3), (void*)0);
		glVertexAttribDivisor(4, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Check for errors
		

		glGenBuffers(1, &m_colorBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
		
		glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), &m_colors[0], GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(11);
		
		glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

		glVertexAttribDivisor(11, 1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		///Square Lines Mesh Buffer Setup
		glBindVertexArray(m_squareLinesMesh.m_vaoId);
		glGenBuffers(1, &m_debugMatrixArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxToNDCMatrix.size() * sizeof(glm::mat3), &m_debugBoxToNDCMatrix[0], GL_DYNAMIC_DRAW);
		unsigned int otherLocation = 7; // Location 7
		for (int i = 0; i < 3; ++i)
		{
			glEnableVertexAttribArray(otherLocation + i);
			glVertexAttribPointer(otherLocation + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
			glVertexAttribDivisor(otherLocation + i, 1);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Bool Check
		glGenBuffers(1, &m_gridColliderBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_gridColliderBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_gridColliderChecks.size() * sizeof(int), &m_gridColliderChecks[0][0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(14);
		glVertexAttribIPointer(14, 1, GL_INT, sizeof(int), (void*)0);
		glVertexAttribDivisor(14, 1);

		glGenBuffers(1, &m_debugCollisionCheckBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxCollisionChecks.size() * sizeof(int), &m_debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW); //Size Of Int
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		glVertexAttribDivisor(4, 1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		

		///Circle Lines
		glBindVertexArray(m_circleLinesMesh.m_vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxToNDCMatrix.size() * sizeof(glm::mat3), &m_debugBoxToNDCMatrix[0], GL_DYNAMIC_DRAW);
		for (int i = 0; i < 3; ++i)
		{
			glEnableVertexAttribArray(otherLocation + i);
			glVertexAttribPointer(otherLocation + i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*)(sizeof(glm::vec3) * i));
			glVertexAttribDivisor(otherLocation + i, 1);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_debugBoxCollisionChecks.size() * sizeof(int), &m_debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
		glVertexAttribDivisor(4, 1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "ERROR: " << err << std::endl;
		}
	}

	void GraphicsPipe::m_funcSetupSSBO()
	{
		// Generate SSBO for Particles
		glGenBuffers(1, &m_particleSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_particleSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(ParticleData), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_particleSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void GraphicsPipe::m_funcSetupFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		if (help->m_windowWidth <= 0 || help->m_windowHeight <= 0) //Skip if window is minimized
		{
			return;
		}

		if (m_screenTexture)
		{
			glDeleteTextures(1, &m_screenTexture);
		}
		if (m_depthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_depthBufferObject);
		}
		if (m_frameBufferObject)
		{
			glDeleteFramebuffers(1, &m_frameBufferObject);
		}
		glGenFramebuffers(1, &m_frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);

		glGenTextures(1, &m_screenTexture);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);


		glGenRenderbuffers(1, &m_depthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glScissor(0, 0, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));

	}


	void GraphicsPipe::m_funcSetupFinalPassBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		if (help->m_windowWidth <= 0 || help->m_windowHeight <= 0) //Skip if window is minimized
		{
			return;
		}

		if (m_finalPassTexture)
		{
			glDeleteTextures(1, &m_finalPassTexture);
		}
		if (m_finalPassDepthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_finalPassDepthBufferObject);
		}
		if (m_finalPassFrameBufferObject)
		{
			glDeleteFramebuffers(1, &m_finalPassFrameBufferObject);
		}
		glGenFramebuffers(1, &m_finalPassFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_finalPassFrameBufferObject);

		glGenTextures(1, &m_finalPassTexture);
		glBindTexture(GL_TEXTURE_2D, m_finalPassTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_finalPassTexture, 0);


		glGenRenderbuffers(1, &m_finalPassDepthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_finalPassDepthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_finalPassDepthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glScissor(0, 0, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));

	}

	void GraphicsPipe::m_funcSetUpUnlitScreenFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		if (help->m_windowWidth <= 0 || help->m_windowHeight <= 0) //Skip if window is minimized
		{
			return;
		}

		if (m_unlitScreenTexture)
		{
			glDeleteTextures(1, &m_unlitScreenTexture);
		}
		if (m_finalPassDepthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_unlitScreenDepthBufferObject);
		}
		if (m_unlitScreenFrameBufferObject)
		{
			glDeleteFramebuffers(1, &m_unlitScreenFrameBufferObject);
		}
		glGenFramebuffers(1, &m_unlitScreenFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_unlitScreenFrameBufferObject);

		glGenTextures(1, &m_unlitScreenTexture);
		glBindTexture(GL_TEXTURE_2D, m_unlitScreenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_unlitScreenTexture, 0);


		glGenRenderbuffers(1, &m_unlitScreenDepthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_unlitScreenDepthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_unlitScreenDepthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glScissor(0, 0, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));

	}

	void GraphicsPipe::m_funcSetupGamePreviewFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();

		if (m_gamePreviewTexture)
		{
			glDeleteTextures(1, &m_gamePreviewTexture);
		}
		if (m_depthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_gamePreviewDepthBufferObject);
		}
		if (m_gamePreviewFrameBufferObject)
		{
			glDeleteFramebuffers(1, &m_gamePreviewFrameBufferObject);
		}
		glGenFramebuffers(1, &m_gamePreviewFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_gamePreviewFrameBufferObject);

		glGenTextures(1, &m_gamePreviewTexture);
		glBindTexture(GL_TEXTURE_2D, m_gamePreviewTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gamePreviewTexture, 0);


		glGenRenderbuffers(1, &m_gamePreviewDepthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_gamePreviewDepthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_gamePreviewDepthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glScissor(0, 0, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));

	}

	void GraphicsPipe::m_funcSetupMultiLightingFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();

		if (m_multiLightingTexture)
		{
			glDeleteTextures(1, &m_multiLightingTexture);
		}
		if (m_multiLightingDepthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_multiLightingDepthBufferObject);
		}
		if (m_multiLightingFrameBufferObject)
		{
			glDeleteFramebuffers(1, &m_multiLightingFrameBufferObject);
		}
		glGenFramebuffers(1, &m_multiLightingFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_multiLightingFrameBufferObject);

		glGenTextures(1, &m_multiLightingTexture);
		glBindTexture(GL_TEXTURE_2D, m_multiLightingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_multiLightingTexture, 0);


		glGenRenderbuffers(1, &m_multiLightingDepthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_multiLightingDepthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_multiLightingDepthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glScissor(0, 0, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));

	}

	void GraphicsPipe::m_funcSetupAdditiveLightingFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();

		if (m_additiveLightingTexture)
		{
			glDeleteTextures(1, &m_additiveLightingTexture);
		}
		if (m_additiveLightingDepthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_additiveLightingDepthBufferObject);
		}
		if (m_additiveLightingFrameBufferObject)
		{
			glDeleteFramebuffers(1, &m_additiveLightingFrameBufferObject);
		}
		glGenFramebuffers(1, &m_additiveLightingFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_additiveLightingFrameBufferObject);

		glGenTextures(1, &m_additiveLightingTexture);
		glBindTexture(GL_TEXTURE_2D, m_additiveLightingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_additiveLightingTexture, 0);


		glGenRenderbuffers(1, &m_additiveLightingDepthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_additiveLightingDepthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_additiveLightingDepthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glScissor(0, 0, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));

	}


	void GraphicsPipe::m_funcSetupCRTFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();

		if (m_crtTexture)
		{
			glDeleteTextures(1, &m_crtTexture);
		}
		if (m_crtDepthBufferObject)
		{
			glDeleteRenderbuffers(1, &m_crtDepthBufferObject);
		}
		if (m_crtFrameBufferObject)
		{
			glDeleteFramebuffers(1, &m_crtFrameBufferObject);
		}
		glGenFramebuffers(1, &m_crtFrameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_crtFrameBufferObject);

		glGenTextures(1, &m_crtTexture);
		glBindTexture(GL_TEXTURE_2D, m_crtTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_crtTexture, 0);


		glGenRenderbuffers(1, &m_crtDepthBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, m_crtDepthBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_crtDepthBufferObject);

#if DEBUG
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			LOGGING_INFO("Framebuffer successfully created");
		}
		else
		{
			LOGGING_INFO("Framebuffer has not been created");
		}
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glScissor(0, 0, static_cast<GLsizei>(help->m_windowWidth), static_cast<GLsizei>(help->m_windowHeight));

	}
}