/******************************************************************/
/*!
\file      finalPassVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      17th Jan, 2025
\brief     A vertex shader used for debugging the grid system rendering.  

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
R"( #version 460 core

layout (location=0) in vec2 vertexPosition;
layout (location=14) in int collidable;

uniform mat3 projection;
uniform mat3 view;
uniform int tilemapRows;
uniform int tilemapColumns;
uniform mat3 modelMatrix;

layout (location=0) out vec4 color; 

void main()
{
	mat3 matrix = projection * view * modelMatrix;

	int xStep = gl_InstanceID % tilemapRows;

	int yStep = gl_InstanceID / tilemapRows;

	vec2 translate = vec2(matrix * vec3(xStep,-yStep, 0.f));

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.995, 1.0);

	gl_Position.x += translate.x;
	gl_Position.y += translate.y;

	
	if (collidable < 0.1)
	{
		color = vec4(0.0f,0.0f,1.0f,1.0f); //blue
	}
	else
	{
		color = vec4(1.0f,0.f,0.f,1.f); //red
	}
}

)"