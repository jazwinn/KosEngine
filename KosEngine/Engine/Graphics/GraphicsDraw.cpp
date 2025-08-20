/******************************************************************/
/*!
\file      GraphicsDraw.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     Implementation of rendering functions for the GraphicsPipe class,
		   which manages various draw calls and prepares OpenGL resources
		   for rendering complex scenes, including 2D shapes, text, and debug views.

\details   This file includes functions for:
		   - Rendering primary and debug elements in separate framebuffers.
		   - Binding and updating buffer data, including matrices, texture orders,
			 and colors, for instanced drawing.
		   - Text rendering, including character positioning, scaling, and color.
		   - Tilemap rendering for rendering multiple tiles as 1 object.
		   - Utility functions for setting polygon modes and drawing grid overlays.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"


namespace graphicpipe
{
	void GraphicsPipe::m_funcDraw()
	{
		if (!m_modelMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
			glNamedBufferData(m_modelMatrixArrayBuffer, m_modelMatrix.size() * sizeof(glm::mat3), &m_modelMatrix[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_iVec3Buffer);
			glNamedBufferData(m_iVec3Buffer, m_iVec3Array.size() * sizeof(glm::ivec3), &m_iVec3Array[0], GL_DYNAMIC_DRAW); //Strip Count, FrameNumber, Texture Order

			glBindBuffer(GL_ARRAY_BUFFER, m_layerBuffer);
			glNamedBufferData(m_layerBuffer, m_layers.size() * sizeof(int), &m_layers[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
			glNamedBufferData(m_colorBuffer, m_colors.size() * sizeof(glm::vec4), &m_colors[0], GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glUseProgram(m_genericShaderProgram);

			GLint lvUniformVarLoc1 = glGetUniformLocation(m_genericShaderProgram, "textures");

			if (lvUniformVarLoc1 >= 0)
			{
				glUniform1iv(lvUniformVarLoc1, static_cast<GLsizei>(m_textureIDs.size()), (GLint*)&m_textureIDs[0]);
			}
			else
			{
				LOGGING_ERROR("Uniform variable location: %d", lvUniformVarLoc1);
				LOGGING_ERROR("Uniform variable 'textures' doesn't exist!");
				std::exit(EXIT_FAILURE);
			}

			for (int i = 0; i < m_textureIDs.size(); ++i)
			{
				glActiveTexture(GL_TEXTURE0 + m_textureIDs[i]);
				glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);
			
			}

			glUniformMatrix3fv(glGetUniformLocation(m_genericShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
			glUniformMatrix3fv(glGetUniformLocation(m_genericShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));
			
			glUniform1f(glGetUniformLocation(m_genericShaderProgram, "globalBrightness"), m_globalLightIntensity);

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_modelMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		
		
		}
		
		
	}

	void GraphicsPipe::m_funcDrawUnlit()
	{
		if (!m_unlitModelMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
			glNamedBufferData(m_modelMatrixArrayBuffer, m_unlitModelMatrix.size() * sizeof(glm::mat3), &m_unlitModelMatrix[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_iVec3Buffer);
			glNamedBufferData(m_iVec3Buffer, m_unlitModelParams.size() * sizeof(glm::ivec3), &m_unlitModelParams[0], GL_DYNAMIC_DRAW); //Strip Count, FrameNumber, Texture Order

			glBindBuffer(GL_ARRAY_BUFFER, m_layerBuffer);
			glNamedBufferData(m_layerBuffer, m_unlitLayers.size() * sizeof(int), &m_unlitLayers[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
			glNamedBufferData(m_colorBuffer, m_unlitColors.size() * sizeof(glm::vec4), &m_unlitColors[0], GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glUseProgram(m_genericShaderProgram);

			GLint lvUniformVarLoc1 = glGetUniformLocation(m_genericShaderProgram, "textures");

			if (lvUniformVarLoc1 >= 0)
			{
				glUniform1iv(lvUniformVarLoc1, static_cast<GLsizei>(m_textureIDs.size()), (GLint*)&m_textureIDs[0]);
			}
			else
			{
				LOGGING_ERROR("Uniform variable location: %d", lvUniformVarLoc1);
				LOGGING_ERROR("Uniform variable 'textures' doesn't exist!");
				std::exit(EXIT_FAILURE);
			}

			for (int i = 0; i < m_textureIDs.size(); ++i)
			{
				glActiveTexture(GL_TEXTURE0 + m_textureIDs[i]);
				glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);

			}

			glUniformMatrix3fv(glGetUniformLocation(m_genericShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
			glUniformMatrix3fv(glGetUniformLocation(m_genericShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glUniform1f(glGetUniformLocation(m_genericShaderProgram, "globalBrightness"), m_globalLightIntensity);

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_unlitModelMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);



		}
	}

	void GraphicsPipe::m_funcDrawVideos()
	{
		for (VideoData x : m_videoData) {

			glUseProgram(m_videoShaderProgram);

			//set uniform
			glUniformMatrix3fv(x.locTransformation, 1, GL_FALSE, glm::value_ptr(x.transformation));
			glUniformMatrix3fv(x.locView, 1, GL_FALSE, glm::value_ptr(graphicpipe::GraphicsCamera::m_currViewMatrix));
			glUniformMatrix3fv(x.locProjection, 1, GL_FALSE, glm::value_ptr(graphicpipe::GraphicsCamera::m_currOrthoMatrix));
			glUniform1iv(x.unilayer, 1, &x.layer);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, x.yTexture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, x.uTexture);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, x.vTexture);


			glBindVertexArray(m_videoMesh.m_vaoId);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 10);

		}
		
	}

	void GraphicsPipe::m_funcDrawDebug()
	{


		if (!m_debugBoxToNDCMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
			glNamedBufferData(m_debugCollisionCheckBuffer, m_debugBoxCollisionChecks.size() * sizeof(float), &m_debugBoxCollisionChecks[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(m_debugShaderProgram);

			glUniformMatrix3fv(glGetUniformLocation(m_debugShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
			glUniformMatrix3fv(glGetUniformLocation(m_debugShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
			glNamedBufferData(m_debugMatrixArrayBuffer, m_debugBoxToNDCMatrix.size() * sizeof(glm::mat3), &m_debugBoxToNDCMatrix[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(m_squareLinesMesh.m_vaoId);
			glDrawElementsInstanced(m_squareLinesMesh.m_primitiveType, m_squareLinesMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_debugBoxToNDCMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			
		}
		if (!m_debugCircleToNDCMatrix.empty())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_debugCollisionCheckBuffer);
			glNamedBufferData(m_debugCollisionCheckBuffer, m_debugCircleCollisionChecks.size() * sizeof(float), &m_debugCircleCollisionChecks[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glUseProgram(m_debugShaderProgram);

			glUniformMatrix3fv(glGetUniformLocation(m_debugShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
			glUniformMatrix3fv(glGetUniformLocation(m_debugShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glBindBuffer(GL_ARRAY_BUFFER, m_debugMatrixArrayBuffer);
			glNamedBufferData(m_debugMatrixArrayBuffer, m_debugCircleToNDCMatrix.size() * sizeof(glm::mat3), &m_debugCircleToNDCMatrix[0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(m_circleLinesMesh.m_vaoId);
			glDrawElementsInstanced(m_circleLinesMesh.m_primitiveType, m_circleLinesMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_debugCircleToNDCMatrix.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	

	void GraphicsPipe::m_funcDrawText()
	{

		if (!m_textData.empty() && GraphicsCamera::m_windowHeight > 0 && GraphicsCamera::m_windowWidth > 0) {
			for (auto& textData : m_textData) {
				// Activate corresponding render state
				glUseProgram(m_textShaderProgram);

				glUniform1i(glGetUniformLocation(m_textShaderProgram, "layer"), textData.m_layer);

				glUniformMatrix3fv(glGetUniformLocation(m_textShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

				glUniformMatrix3fv(glGetUniformLocation(m_textShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

				glm::vec2 point = { textData.m_x, textData.m_y };

				float angle = textData.m_rotate * 3.1451f / 180.f;

				glm::mat3 rotationMatrix = {
				cos(angle), -sin(angle), 0.0f,
				sin(angle), cos(angle),  0.0f,
				0.0f,       0.0f,       1.0 };

				glUniformMatrix3fv(glGetUniformLocation(m_textShaderProgram, "rotate"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));

				glUniform2fv(glGetUniformLocation(m_textShaderProgram, "point"), 1, glm::value_ptr(point));

				// Set the text color
				glUniform3f(glGetUniformLocation(m_textShaderProgram, "textColor"), textData.m_color.x, textData.m_color.y, textData.m_color.z);

				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(m_textMesh.m_vaoId);
				float origin{ textData.m_x };

				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				float originX{ textData.m_x };
				float originY{ textData.m_y };

				Helper::Helpers* help = Helper::Helpers::GetInstance();
				//float scale = help->m_windowHeight / static_cast<float>(help->m_currWindowHeight);
				//scale /= (help->m_windowWidth / 1920.f); ///Recalibrate if needed

				if (textData.m_isCentered)
				{
					// Step 1: Calculate total width and height of the text
					float totalWidth = 0.0f;
					float maxAscent = 0.0f;
					float maxDescent = 0.0f;
					for (int i = 0; i < textData.m_text.size(); ++i) // To calculate the total width of the text
					{
						char c = textData.m_text[i];
						text::CharacterData ch = assetmanager->m_fontManager.m_fonts[textData.m_fileName][c];
						if (i == textData.m_text.size() - 1) // If it's the last letter, add only the letter size
						{
							totalWidth += ((ch.m_size.x * textData.m_scale * textData.m_xyScale.x) / ((static_cast<float>(help->m_currWindowHeight))));
						}
						else // Add the letter size and the space
						{
							totalWidth += ((ch.m_advance >> 6) * textData.m_scale * textData.m_xyScale.x) / ((static_cast<float>(help->m_currWindowHeight)));
						}
						maxAscent = std::max(maxAscent, (ch.m_bearing.y * textData.m_scale * textData.m_xyScale.y) / ((static_cast<float>(help->m_currWindowHeight))));
						maxDescent = std::max(maxDescent, ((ch.m_size.y - ch.m_bearing.y) * textData.m_scale * textData.m_xyScale.y) / ((static_cast<float>(help->m_currWindowHeight))));
					}

					float totalHeight = maxAscent + maxDescent;



					// Adjust starting position to center the text
					textData.m_x = originX - totalWidth / 2.0f;  // Horizontal centering
					textData.m_y = originY + maxAscent / 2.0f - totalHeight / 2.0f;  // Vertical centering
				}

				else
				{
					float xpos = (assetmanager->m_fontManager.m_fonts[textData.m_fileName][textData.m_text[0]].m_bearing.x / ((static_cast<float>(help->m_currWindowHeight))) * (textData.m_scale * textData.m_xyScale.x));
					textData.m_x -= xpos;
				}
				

				// Step 2: Render the text
				for (const char& c : textData.m_text)
				{
					text::CharacterData ch = assetmanager->m_fontManager.m_fonts[textData.m_fileName][c];

					// Calculate position and size for each character quad
					float xpos = (textData.m_x + ch.m_bearing.x / ((static_cast<float>(help->m_currWindowHeight))) * (textData.m_scale * textData.m_xyScale.x));
					float ypos = (textData.m_y - ((float)ch.m_size.y - (float)ch.m_bearing.y) / ((static_cast<float>(help->m_currWindowHeight))) * (textData.m_scale * textData.m_xyScale.y));
					float w = ch.m_size.x * textData.m_scale * textData.m_xyScale.x / ((static_cast<float>(help->m_currWindowHeight)));
					float h = ch.m_size.y * textData.m_scale * textData.m_xyScale.y / ((static_cast<float>(help->m_currWindowHeight)));

					// Update VBO for each character with texture coordinates from the atlas
					float vertices[6][4] =
					{
						{ xpos,     ypos + h,   ch.m_topLeftTexCoords.x, ch.m_topLeftTexCoords.y },
						{ xpos,     ypos,       ch.m_topLeftTexCoords.x, ch.m_bottomRightTexCoords.y },
						{ xpos + w, ypos,       ch.m_bottomRightTexCoords.x, ch.m_bottomRightTexCoords.y },

						{ xpos,     ypos + h,   ch.m_topLeftTexCoords.x, ch.m_topLeftTexCoords.y },
						{ xpos + w, ypos,       ch.m_bottomRightTexCoords.x, ch.m_bottomRightTexCoords.y },
						{ xpos + w, ypos + h,   ch.m_bottomRightTexCoords.x, ch.m_topLeftTexCoords.y }
					};

					// Bind the texture atlas (once for all characters)
					glBindTexture(GL_TEXTURE_2D, ch.m_textureID);

					// Update the content of the VBO memory
					glBindBuffer(GL_ARRAY_BUFFER, m_textBuffer);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					// Render quad
					glDrawArrays(GL_TRIANGLES, 0, 6);

					// Advance 
					//  for next glyph
					textData.m_x += ((ch.m_advance >> 6) * textData.m_scale * textData.m_xyScale.x) / ((static_cast<float>(help->m_currWindowHeight)));
				}
				textData.m_x = origin;

				// Unbind for safety
				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	void GraphicsPipe::m_funcDrawGrid() 
	{
		glUseProgram(m_gridShaderProgram);

		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));
		glUniform3f(glGetUniformLocation(m_gridShaderProgram, "debugColor"), 0.f, 0.f, 0.f);
		glBindVertexArray(m_lineMesh.m_vaoId);
		glDrawArrays(GL_LINES, 0, 1000 * 8 + 12); // Number of vertices of the grid
		glBindVertexArray(0);
			
	}

	void GraphicsPipe::m_funcDrawLine(glm::vec3 p0, glm::vec3 p1)
	{

		std::vector<glm::vec3> vertices;

		vertices.push_back(p0);
		vertices.push_back(p1);

		unsigned int vaoid;
		unsigned int vboid;
		glGenVertexArrays(1, &vaoid);
		glGenBuffers(1, &vboid);

		glBindVertexArray(vaoid);

		glBindBuffer(GL_ARRAY_BUFFER, vboid);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glUseProgram(m_gridShaderProgram);

		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));
		glUniformMatrix3fv(glGetUniformLocation(m_gridShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));
		glUniform3f(glGetUniformLocation(m_gridShaderProgram, "debugColor"), 0.5f , 0.1f, 0.1f);
		glDrawArrays(GL_LINES, 0, 2); // Number of vertices
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcDrawGridCollider()
	{

		glUseProgram(m_gridDebugShaderProgram);

		for (int i{}; i < m_colliderGridData.size() && !m_gridColliderChecks.empty(); ++i)
		{
			if (m_gridColliderChecks[i].empty())
			{
				continue;
			}

			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#02 OpenGL Error: " << err << std::endl;
			}

			glBindBuffer(GL_ARRAY_BUFFER, m_gridColliderBuffer);
			glNamedBufferData(m_gridColliderBuffer, m_gridColliderChecks[i].size() * sizeof(int), &m_gridColliderChecks[i][0], GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glUniform1i(glGetUniformLocation(m_gridDebugShaderProgram, "tilemapRows"), m_colliderGridData[i].m_gridDimensions.x);

			GLenum err2 = glGetError();
			if (err2 != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#03 OpenGL Error: " << err2 << std::endl;
			}

			glUniform1i(glGetUniformLocation(m_gridDebugShaderProgram, "tilemapColumns"), m_colliderGridData[i].m_gridDimensions.y);

			glUniformMatrix3fv(glGetUniformLocation(m_gridDebugShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_gridDebugShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_gridDebugShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_colliderGridData[i].m_transformation));

			glBindVertexArray(m_squareLinesMesh.m_vaoId);
			glDrawElementsInstanced(m_squareLinesMesh.m_primitiveType, m_squareLinesMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_colliderGridData[i].m_gridDimensions.x * m_colliderGridData[i].m_gridDimensions.y));
			glBindVertexArray(0);
		}
	}

	void GraphicsPipe::m_funcDrawTilemap()
	{
		glUseProgram(m_tilemapShaderProgram);

		for (int i{}; i < m_transformedTilemaps.size() && !m_tileIndexes.empty(); ++i)
		{
			if (m_tileIndexes[i].empty())
			{
				continue;
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_tileIndexBuffer);
			glNamedBufferData(m_tileIndexBuffer, m_tileIndexes[i].size() * sizeof(int), &m_tileIndexes[i][0], GL_DYNAMIC_DRAW);

			//glBindBuffer(GL_ARRAY_BUFFER, m_gridColliderBuffer);
			//glNamedBufferData(m_gridColliderBuffer, m_gridColliderChecks[i].size() * sizeof(int), &m_gridColliderChecks[i][0], GL_DYNAMIC_DRAW);
			//glBindBuffer(GL_ARRAY_BUFFER, 0);


			glActiveTexture(GL_TEXTURE0 + m_textureIDs[m_transformedTilemaps[i].m_textureID]); // Activate each texture unit
			glBindTexture(GL_TEXTURE_2D, m_textureIDs[m_transformedTilemaps[i].m_textureID]);  // Unbind the 2D texture from that unit
			
			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "textureID"), m_textureIDs[m_transformedTilemaps[i].m_textureID]);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "layer"), m_transformedTilemaps[i].m_layer);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "index"), m_transformedTilemaps[i].m_tileIndex);
		
			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilemapRows"), m_transformedTilemaps[i].m_tilemapDimensions.x);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilemapColumns"), m_transformedTilemaps[i].m_tilemapDimensions.y);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilePicSizeX"), m_transformedTilemaps[i].m_tilemapPictureSize.x);

			glUniform1i(glGetUniformLocation(m_tilemapShaderProgram, "tilePicSizeY"), m_transformedTilemaps[i].m_tilemapPictureSize.y);

			glUniform4f(glGetUniformLocation(m_tilemapShaderProgram, "modelColor"), m_transformedTilemaps[i].m_color.r, m_transformedTilemaps[i].m_color.g, m_transformedTilemaps[i].m_color.b, m_transformedTilemaps[i].m_color.a);

			glUniformMatrix3fv(glGetUniformLocation(m_tilemapShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_tilemapShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_tilemapShaderProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_transformedTilemaps[i].m_transformation));

			glUniform1f(glGetUniformLocation(m_tilemapShaderProgram, "globalBrightness"), m_globalLightIntensity);

			

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_transformedTilemaps[i].m_tilemapDimensions.x * m_transformedTilemaps[i].m_tilemapDimensions.y));
			glBindVertexArray(0);
			
		}

	}

	

	void GraphicsPipe::m_funcRenderMultiLighting()
	{
		if (!m_multiLightingTransforms.empty())
		{
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE); //USE THIS BLENDING FOR LIGHTS
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


			glUseProgram(m_lightingShaderProgram);

			glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
			glNamedBufferData(m_modelMatrixArrayBuffer, m_multiLightingTransforms.size() * sizeof(glm::mat3), &m_multiLightingTransforms[0], GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, m_vec3Buffer);
			glNamedBufferData(m_vec3Buffer, m_multiLightingParams.size() * sizeof(glm::vec3), &m_multiLightingParams[0], GL_DYNAMIC_DRAW); // Light Intensity, Inner/Outer Radius

			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
			glNamedBufferData(m_colorBuffer, m_multiLightingColors.size() * sizeof(glm::vec4), &m_multiLightingColors[0], GL_DYNAMIC_DRAW);
	

			glUniformMatrix3fv(glGetUniformLocation(m_lightingShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_lightingShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glUniform1f(glGetUniformLocation(m_lightingShaderProgram, "globalBrightness"), m_globalLightIntensity);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#04 OpenGL Error: " << err << std::endl;
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			

			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_multiLightingTransforms.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLenum err2 = glGetError();
			if (err2 != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#05 OpenGL Error : " << err2 << std::endl;
			}

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}
	void GraphicsPipe::m_funcRenderAdditiveLighting()
	{
		if (!m_additiveLightingTransforms.empty())
		{
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE); //USE THIS BLENDING FOR LIGHTS
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


			glUseProgram(m_lightingShaderProgram);

			glBindBuffer(GL_ARRAY_BUFFER, m_modelMatrixArrayBuffer);
			glNamedBufferData(m_modelMatrixArrayBuffer, m_additiveLightingTransforms.size() * sizeof(glm::mat3), &m_additiveLightingTransforms[0], GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, m_vec3Buffer);
			glNamedBufferData(m_vec3Buffer, m_additiveLightingParams.size() * sizeof(glm::vec3), &m_additiveLightingParams[0], GL_DYNAMIC_DRAW); // Light Intensity, Inner/Outer Radius

			glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
			glNamedBufferData(m_colorBuffer, m_additiveLightingColors.size() * sizeof(glm::vec4), &m_additiveLightingColors[0], GL_DYNAMIC_DRAW);


			glUniformMatrix3fv(glGetUniformLocation(m_lightingShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

			glUniformMatrix3fv(glGetUniformLocation(m_lightingShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

			glUniform1f(glGetUniformLocation(m_lightingShaderProgram, "globalBrightness"), m_globalLightIntensity);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#04 OpenGL Error: " << err << std::endl;
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);



			glBindVertexArray(m_squareMesh.m_vaoId);
			glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, static_cast<GLsizei>(m_additiveLightingTransforms.size()));
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLenum err2 = glGetError();
			if (err2 != GL_NO_ERROR) {
				//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
				std::cout << "#05 OpenGL Error : " << err2 << std::endl;
			}

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void GraphicsPipe::m_funcDrawFullScreenQuad(unsigned int texture)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(m_frameBufferShaderProgram);
		glBindVertexArray(m_screenTextureVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void GraphicsPipe::m_funcDrawParticles()
	{
		glUseProgram(m_particleShaderProgram);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_particleSSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_particleSSBO);


		glUniformMatrix3fv(glGetUniformLocation(m_particleShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currViewMatrix));

		glUniformMatrix3fv(glGetUniformLocation(m_particleShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(GraphicsCamera::m_currOrthoMatrix));

		GLint lvUniformVarLoc1 = glGetUniformLocation(m_genericShaderProgram, "textures");

		if (lvUniformVarLoc1 >= 0)
		{
			glUniform1iv(lvUniformVarLoc1, static_cast<GLsizei>(m_textureIDs.size()), (GLint*)&m_textureIDs[0]);
		}
		else
		{
			LOGGING_ERROR("Uniform variable location: %d", lvUniformVarLoc1);
			LOGGING_ERROR("Uniform variable 'textures' doesn't exist!");
			std::exit(EXIT_FAILURE);
		}

		for (int i = 0; i < m_textureIDs.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + m_textureIDs[i]);
			glBindTexture(GL_TEXTURE_2D, m_textureIDs[i]);

		}

		glBindVertexArray(m_squareMesh.m_vaoId);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_particleSSBO);
		glDrawElementsInstanced(m_squareMesh.m_primitiveType, m_squareMesh.m_indexElementCount, GL_UNSIGNED_SHORT, NULL, MAX_PARTICLES);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		GLenum err2 = glGetError();
		if (err2 != GL_NO_ERROR) {
			//LOGGING_ERROR("First OpenGL Error: 0x%X", err);
			std::cout << "Particle OpenGL Error : " << err2 << std::endl;
		}

		
	}

	void GraphicsPipe::m_funcSetDrawMode(GLenum mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode);
	}

}


