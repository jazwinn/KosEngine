/******************************************************************/
/*!
\file      GraphicsVAO.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file contains functions for setting up Vertex Array Objects (VAOs)
		   and Buffers for various shapes (e.g., squares, circles, grid) used in
		   the graphics pipeline for rendering.

\details   The file includes functions for creating and setting up VAOs, VBOs, and
		   EBOs for rendering different shapes, including squares, circles, grids,
		   and text. Each shape is stored with its respective vertex positions,
		   texture coordinates, and colors. The grid and circle functions also
		   generate vertices dynamically based on specified parameters (e.g., grid size, circle radius).


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
namespace graphicpipe
{
	void GraphicsPipe::m_funcSetupFrameBufferVao()
	{
		float vertices[] =
		{
			//Coords		texCoords
			1.0f,-1.0f,		1.0f,0.f,
			- 1.0f,-1.0f,	0.f,0.f,
			-1.0f,1.0f,		0.f,1.f,

			1.0f,1.0f,		1.0f,1.0f,
			1.0f,-1.0f,		1.0f,0.f,
			-1.0f,1.0f,		0.0f,1.0f


		};

		unsigned int vbo;
		glGenVertexArrays(1, &m_screenTextureVAO);
		glGenBuffers(1, &vbo);
		glBindVertexArray(m_screenTextureVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcSetupSquareLinesVao()
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<GLushort>idx_vtx;

		lvPosVtx = { glm::vec2(0.5f, -0.5f),
					glm::vec2(0.5f, 0.5f),
					glm::vec2(-0.5f, 0.5f),
					glm::vec2(-0.5f, -0.5f) };



		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());


		unsigned int lvVboId{};

		glCreateBuffers(1, &lvVboId);


		glNamedBufferStorage(lvVboId, position_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());

		glCreateVertexArrays(1, &m_squareLinesMesh.m_vaoId);
		glEnableVertexArrayAttrib(m_squareLinesMesh.m_vaoId, 0);
		glVertexArrayVertexBuffer(m_squareLinesMesh.m_vaoId, 0, lvVboId,
			position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(m_squareLinesMesh.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_squareLinesMesh.m_vaoId, 0, 0);

		m_squareLinesMesh.m_primitiveType = GL_LINE_LOOP;
		idx_vtx = { 0, 1, 2, 3 };


		m_squareLinesMesh.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());
		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * m_squareLinesMesh.m_indexElementCount,
			reinterpret_cast<GLvoid*>(idx_vtx.data()),
			GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(m_squareLinesMesh.m_vaoId, ebo_hdl);
		glBindVertexArray(0);
	}

	void GraphicsPipe::m_funcSetupCircleLinesVao()
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<GLushort> idx_vtx;

		int numVertices = 36; // 36 vertices
		float radius = 0.5f;  // Circle radius

		// Generate circle vertices
		for (int i = 0; i < numVertices; ++i) {
			float angle = 2.0f * 3.1415f * i / numVertices; // Calculate angle for each vertex
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			lvPosVtx.push_back(glm::vec2(x, y));
			idx_vtx.push_back(static_cast<GLushort>(i));
		}

		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());

		unsigned int lvVboId{};
		glCreateBuffers(1, &lvVboId);

		// Allocate storage for vertex buffer and fill with circular vertices
		glNamedBufferStorage(lvVboId, position_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());

		glCreateVertexArrays(1, &m_circleLinesMesh.m_vaoId);
		glEnableVertexArrayAttrib(m_circleLinesMesh.m_vaoId, 0);
		glVertexArrayVertexBuffer(m_circleLinesMesh.m_vaoId, 0, lvVboId, position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(m_circleLinesMesh.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(m_circleLinesMesh.m_vaoId, 0, 0);

		m_circleLinesMesh.m_primitiveType = GL_LINE_LOOP;
		m_circleLinesMesh.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());

		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * m_circleLinesMesh.m_indexElementCount,
			idx_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(m_circleLinesMesh.m_vaoId, ebo_hdl);

		glBindVertexArray(0);
	}

	void GraphicsPipe::m_funcSetupGridVao()
	{
		int const gridSize = 1000;
		float const spacing = 1.f;
		std::vector<glm::vec3> vertices;
		m_funcSetupGrid(vertices, gridSize, spacing);

		glGenVertexArrays(1, &m_lineMesh.m_vaoId);
		glGenBuffers(1, &m_gridBuffer);

		glBindVertexArray(m_lineMesh.m_vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, m_gridBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcSetupVideoVAO()
	{
		float vertices[] = {
			// Positions     // Texture Coords
			-1.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // Bottom-left
			 1.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // Bottom-right
			 1.0f,  1.0f, 0.0f,  1.0f, 0.0f,  // Top-right
			-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,   // Top-left
		};

		GLuint indices[] = {
			0, 1, 2, // First triangle
			2, 3, 0  // Second triangle
		};

		m_videoMesh;
		GLuint EBO;
		glGenVertexArrays(1, &m_videoMesh.m_vaoId);
		glGenBuffers(1, &m_videoBuffer);
		glGenBuffers(1, &EBO);

		glBindVertexArray(m_videoMesh.m_vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, m_videoBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

	}

	

	void GraphicsPipe::m_funcSetupGrid(std::vector<glm::vec3>& vertices, int gridSize, float spacing)
	{
		for (int i = -gridSize; i <= gridSize; ++i)
		{
			// Lines parallel to X-axis
			vertices.push_back(glm::vec3(i * spacing, -gridSize * spacing, 0.f));
			vertices.push_back(glm::vec3(i * spacing, gridSize * spacing, 0.f));

			// Lines parallel to Y-axis
			vertices.push_back(glm::vec3(-gridSize * spacing, i * spacing, 0.f));
			vertices.push_back(glm::vec3(gridSize * spacing, i * spacing, 0.f));
		}
		// Center Grid X axis
		vertices.push_back(glm::vec3(0.01f, -gridSize * spacing, 0.f));
		vertices.push_back(glm::vec3(0.01f, gridSize * spacing, 0.f));
		vertices.push_back(glm::vec3(-0.01f, -gridSize * spacing, 0.f));
		vertices.push_back(glm::vec3(-0.01f, gridSize * spacing, 0.f));

		// Center Grid Y axis
		vertices.push_back(glm::vec3(-gridSize * spacing, 0.01f, 0.f));
		vertices.push_back(glm::vec3(gridSize * spacing, 0.01f, 0.f));
		vertices.push_back(glm::vec3(-gridSize * spacing, -0.01f, 0.f));
		vertices.push_back(glm::vec3(gridSize * spacing, -0.01f, 0.f));
	}

	void GraphicsPipe::m_funcSetupTextVao()
	{

		glGenVertexArrays(1, &m_textMesh.m_vaoId);
		glGenBuffers(1, &m_textBuffer);

		glBindVertexArray(m_textMesh.m_vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, m_textBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void GraphicsPipe::m_funcSetupVao(Mesh& shape)
	{
		std::vector<glm::vec2> lvPosVtx;
		std::vector<glm::vec2> lvNDCVtx;
		std::vector<glm::vec2> lvTexCoords;
		std::vector<GLushort>idx_vtx;


		lvPosVtx = { glm::vec2(0.5f, -0.5f),
						glm::vec2(0.5f, 0.5f),
						glm::vec2(-0.5f, 0.5f),
						glm::vec2(-0.5f, -0.5f) };
		lvNDCVtx = { glm::vec2(1.f, -1.f),
						glm::vec2(1.f, 1.f),
						glm::vec2(-1.f, 1.f),
						glm::vec2(-1.f, -1.f) };
		lvTexCoords = { glm::vec2(1.f, 0.f),
						glm::vec2(1.f, 1.f),
						glm::vec2(0.f, 1.f),
						glm::vec2(0.f, 0.f) };



		GLsizei position_data_offset = 0;
		GLsizei position_attribute_size = sizeof(glm::vec2);
		GLsizei position_data_size = position_attribute_size * static_cast<GLsizei>(lvPosVtx.size());
		GLsizei NDC_data_offset = position_data_size;
		GLsizei NDC_attribute_size = sizeof(glm::vec2);
		GLsizei NDC_data_size = position_attribute_size * static_cast<GLsizei>(lvNDCVtx.size());
		GLsizei texcoord_data_offset = position_data_size + NDC_data_size;
		GLsizei texcoord_attribute_size = sizeof(glm::vec2);
		GLsizei texcoord_data_size = texcoord_attribute_size * static_cast<GLsizei>(lvTexCoords.size());


		unsigned int lvVboId{};

		glCreateBuffers(1, &lvVboId);

		glNamedBufferStorage(lvVboId, position_data_size + NDC_data_size  + texcoord_data_size, nullptr, GL_DYNAMIC_STORAGE_BIT);

		glNamedBufferSubData(lvVboId, position_data_offset, position_data_size, lvPosVtx.data());
		glNamedBufferSubData(lvVboId, NDC_data_offset, NDC_data_size, lvNDCVtx.data());
		glNamedBufferSubData(lvVboId, texcoord_data_offset, texcoord_data_size, lvTexCoords.data());

		glCreateVertexArrays(1, &shape.m_vaoId);
		glEnableVertexArrayAttrib(shape.m_vaoId, 0);
		glVertexArrayVertexBuffer(shape.m_vaoId, 0, lvVboId,
			position_data_offset, position_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 0, 0);

		glEnableVertexArrayAttrib(shape.m_vaoId, 1);
		glVertexArrayVertexBuffer(shape.m_vaoId, 1, lvVboId,
			NDC_data_offset, NDC_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 1, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 1, 1);

		glEnableVertexArrayAttrib(shape.m_vaoId, 2);
		glVertexArrayVertexBuffer(shape.m_vaoId, 2, lvVboId, texcoord_data_offset,
			texcoord_attribute_size);
		glVertexArrayAttribFormat(shape.m_vaoId, 2, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(shape.m_vaoId, 2, 2);



		shape.m_primitiveType = GL_TRIANGLES;
		idx_vtx = { 0, 1, 2, 2, 3, 0 };


		shape.m_indexElementCount = static_cast<unsigned short>(idx_vtx.size());
		unsigned int ebo_hdl;
		glCreateBuffers(1, &ebo_hdl);
		glNamedBufferStorage(ebo_hdl, sizeof(unsigned short) * shape.m_indexElementCount,
			reinterpret_cast<GLvoid*>(idx_vtx.data()),
			GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(shape.m_vaoId, ebo_hdl);
		glBindVertexArray(0);
	}
}