/******************************************************************/
/*!
\file      GraphicsFrameBufferUsage.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      17th Jan, 2025
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"
#include "../Helper/Helper.h"

namespace graphicpipe
{
	void GraphicsPipe::m_funcDrawGameFrameBuffer()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		glClearColor(static_cast<GLclampf>(help->m_colour.m_x * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_y * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_z * m_globalLightIntensity), static_cast<GLclampf>(1));
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();
		m_funcDrawParticles();

		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void GraphicsPipe::m_funcDrawWindow()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		glClearColor(static_cast<GLclampf>(help->m_colour.m_x * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_y * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_z * m_globalLightIntensity), static_cast<GLclampf>(1));
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();
		m_funcDrawParticles();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

	}

	void GraphicsPipe::m_drawMultiLightingTexture()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_multiLightingFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcRenderMultiLighting();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsPipe::m_drawAdditiveLightingTexture()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_additiveLightingFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcRenderAdditiveLighting();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GraphicsPipe::m_funcDrawUnlitObjectTexture()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_unlitScreenFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawUnlit();
		m_funcDrawText();
		m_funcDrawVideos();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		//Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(static_cast<GLclampf>(help->m_colour.m_x * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_y * m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_z * m_globalLightIntensity), static_cast<GLclampf>(1));
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		glClearColor(0, 0, 0, 1.f);

	}

	void GraphicsPipe::m_funcDrawDebugTexture()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, m_unlitScreenFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawUnlit();
		m_funcDrawText();
		m_funcDrawVideos();
		m_funcDrawDebug();
		m_funcDrawGrid();
		m_funcDrawGridCollider();
		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		//Helper::Helpers* help = Helper::Helpers::GetInstance();
		//glClearColor(help->m_colour.m_x, help->m_colour.m_y, help->m_colour.m_z, 1.f);
		//glClearColor(0, 0, 0, 1.f);
	}

	void GraphicsPipe::m_renderFinalPass()
	{

		Helper::Helpers* help = Helper::Helpers::GetInstance();
		glViewport(0, 0, (int)help->m_windowWidth, (int)help->m_windowHeight);
		m_funcDrawGameFrameBuffer();
		m_drawMultiLightingTexture();
		m_drawAdditiveLightingTexture();
		m_funcDrawUnlitObjectTexture();
		glClearColor(0, 0, 0, 1.f);

		glBindFramebuffer(GL_FRAMEBUFFER, m_finalPassFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*-----------------------------------------------------------------------------*/
		glUseProgram(m_finalPassShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_multiLightingTexture);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_unlitScreenTexture);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_additiveLightingTexture);

		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "additiveLightTexture"), 3);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "unlitScreenTexture"), 2);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "multiLightTexture"), 1);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "screenTexture"), 0);
		glUniform1f(glGetUniformLocation(m_finalPassShaderProgram, "globalBrightness"), m_globalLightIntensity);

		glBindVertexArray(m_screenTextureVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		/*------------------------------------------------------------------------------*/


		glViewport(help->m_viewportOffsetX, help->m_viewportOffsetY, (int)help->m_windowWidth, (int)help->m_windowHeight);
		

		if (true)
		{
			m_renderCRTPass();
		}
		else
		{
			glUseProgram(m_frameBufferShaderProgram);
			glBindVertexArray(m_screenTextureVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_finalPassTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		


		
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "Second Final Pass OpenGL Error: " << err << std::endl;
		}
	}

	void GraphicsPipe::m_renderCRTPass()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		glViewport(0, 0, (int)help->m_windowWidth, (int)help->m_windowHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, m_crtFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*-----------------------------------------------------------------------------*/
		glUseProgram(m_crtShaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_finalPassTexture);
		static float timePassed{};
		timePassed += help->m_fixedDeltaTime;
		glUniform1i(glGetUniformLocation(m_crtShaderProgram, "finalTexture"), 0);
		glUniform2f(glGetUniformLocation(m_crtShaderProgram, "iResolution"), help->m_windowWidth, help->m_windowHeight );
		glUniform1f(glGetUniformLocation(m_crtShaderProgram, "deltaTime"), timePassed);
		
		glBindVertexArray(m_screenTextureVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		/*------------------------------------------------------------------------------*/


		glViewport(help->m_viewportOffsetX, help->m_viewportOffsetY, (int)help->m_windowWidth, (int)help->m_windowHeight);

		glUseProgram(m_frameBufferShaderProgram);
		glBindVertexArray(m_screenTextureVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_crtTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "CRT OpenGL Error: " << err << std::endl;
		}
	}

	void GraphicsPipe::m_renderFinalPassWithDebug()
	{

		Helper::Helpers* help = Helper::Helpers::GetInstance();
		glViewport(0, 0, (int)help->m_windowWidth, (int)help->m_windowHeight);
		m_funcDrawWindow();
		m_drawMultiLightingTexture();
		m_drawAdditiveLightingTexture();
		m_funcDrawDebugTexture();
		glBindFramebuffer(GL_FRAMEBUFFER, m_gamePreviewFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*-----------------------------------------------------------------------------*/
		glUseProgram(m_finalPassShaderProgram);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "First Final Pass OpenGL Error: " << err << std::endl;
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_multiLightingTexture);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_unlitScreenTexture);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_additiveLightingTexture);

		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "additiveLightTexture"), 3);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "unlitScreenTexture"), 2);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "lightTexture"), 1);
		glUniform1i(glGetUniformLocation(m_finalPassShaderProgram, "screenTexture"), 0);
		glUniform1f(glGetUniformLocation(m_finalPassShaderProgram, "globalBrightness"), m_globalLightIntensity);

		glBindVertexArray(m_screenTextureVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*------------------------------------------------------------------------------*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(help->m_viewportOffsetX, help->m_viewportOffsetY, (int)help->m_windowWidth, (int)help->m_windowHeight);

		//err = glGetError();
		//if (err != GL_NO_ERROR) {
		//	//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
		//	std::cout << "Second Final Pass OpenGL Error: " << err << std::endl;
		//}
	}

	void GraphicsPipe::m_funcDrawGamePreviewWindow()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_gamePreviewFrameBufferObject);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_funcDrawTilemap();
		m_funcDraw();
		m_funcDrawText();
		m_funcRenderMultiLighting();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

	}
}