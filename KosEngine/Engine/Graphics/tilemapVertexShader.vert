/******************************************************************/
/*!
\file      tilemapVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      29th Nov, 2024
\brief     A vertex shader for rendering a tilemap using instanced 
           drawing, supporting transformations, layering, and texture 
           coordinate adjustments.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

layout (location=0) in vec2 vertexPosition;
layout (location=2) in vec2 vertexTexCoords;
layout (location=15) in int index;

uniform mat3 projection;
uniform mat3 view;
//uniform int index;
uniform int tilemapRows;
uniform int tilemapColumns;
uniform mat3 modelMatrix;
uniform vec4 modelColor;
uniform int layer;
uniform int tilePicSizeX;
uniform int tilePicSizeY;

layout (location=0) out vec4 color; 
layout (location=1) out vec2 texCoords;

void main()
{

	mat3 matrix = projection * view * modelMatrix;

	float tileWidth = 1.0 / tilePicSizeX;

	float tileHeight = 1.0 / tilePicSizeY;

	float y = (tilePicSizeY - 1) - floor(index / tilePicSizeX); 

	int x = index % tilePicSizeX;

	float frameXOffset = tileWidth * x;

	float frameYOffset = tileHeight * y;

	int xStep = gl_InstanceID % tilemapRows;

	int yStep = gl_InstanceID / tilemapRows;

	vec2 translate = vec2(matrix * vec3(xStep,-yStep, 0.f));

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.0001 * layer, 1.0);

	gl_Position.x += translate.x;
	gl_Position.y += translate.y;

	texCoords = vec2(vertexTexCoords.x * tileWidth + frameXOffset, vertexTexCoords.y * tileHeight + frameYOffset);
	
	color = modelColor;
}

)"