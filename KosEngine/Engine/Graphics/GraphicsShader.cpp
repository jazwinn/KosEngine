/******************************************************************/
/*!
\file      GraphicsShader.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file contains functions related to compiling, setting up, and deleting shaders
		   used in the graphics pipeline for rendering operations.

\details   The file provides utility functions for compiling and setting up vertex and fragment
		   shaders, as well as managing shader program creation for rendering tasks. The shaders
		   are compiled from source code, linked into a program, and then validated. In case of
		   compilation or linking errors, detailed error messages are printed. Additionally, functions
		   for cleaning up shader programs after use are provided.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"

namespace graphicpipe
{

	GLuint GraphicsPipe::m_funcCompileShader(GLuint type, const std::string& shader)
	{
		GLuint lvID = glCreateShader(type);
		const char* source = shader.c_str();
		glShaderSource(lvID, 1, &source, nullptr);
		glCompileShader(lvID);

		int result;
		glGetShaderiv(lvID, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			int length;
			glGetShaderiv(lvID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(lvID, length, &length, message);	
			LOGGING_ERROR("Failed to Compile Shader: {}", message); 
			glDeleteShader(lvID);
			free(message);
			//TOCHECK
			//std::exit;
			return 0;
		}

		return lvID;
	}

	unsigned int GraphicsPipe::m_funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint lvProgram = glCreateProgram();
		GLuint lvVertexShaderID = m_funcCompileShader(GL_VERTEX_SHADER, vertexShader);
		GLuint lvFragmentShaderID = m_funcCompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(lvProgram, lvVertexShaderID);
		glAttachShader(lvProgram, lvFragmentShaderID);
		glLinkProgram(lvProgram);
		glValidateProgram(lvProgram);

		glDeleteShader(lvVertexShaderID);
		glDeleteShader(lvFragmentShaderID);

		GLint success;
		glGetProgramiv(lvProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(lvProgram, 512, NULL, infoLog);
			LOGGING_ERROR("Error linking shader program: {}", infoLog);
		}
		else
		{
			LOGGING_INFO("Shader compiled successfully");
		}

		return lvProgram;
	}

	unsigned int GraphicsPipe::m_funcSetupComputerShader(const std::string& computerShader)
	{
		GLuint computeShader = m_funcCompileShader(GL_COMPUTE_SHADER, computerShader);

		GLuint program = glCreateProgram();
		glAttachShader(program, computeShader);
		glLinkProgram(program);

		// Check linking status
		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			LOGGING_ERROR("Error linking shader program: {}", infoLog);
		}
		else
		{
			LOGGING_INFO("Shader compiled successfully");
		}

		return program;
	}


	void GraphicsPipe::m_funcDeleteShader()
	{
		glDeleteProgram(m_genericShaderProgram);
	}
}