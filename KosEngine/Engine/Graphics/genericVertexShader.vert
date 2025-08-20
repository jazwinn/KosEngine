/******************************************************************/
/*!
\file      genericVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A basic vertex shader for rendering textured and animated
           sprites using instanced arrays.

           This shader computes the position of each vertex using 
           a model matrix and handles animation frames for sprites 
           based on texture strip count and frame number.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core


layout (location=0) in vec2 vertexPosition;

layout (location=2) in vec2 vertexTexCoords;


layout (location=4) in ivec3 aSpriteData; // Strip Count, Frame Number, Texture ID

layout (location=10) in int aLayer;
layout (location=11) in vec4 aColor;

layout (location=6) in mat3 modelMatrix;

layout (location=0) out vec4 color; 
layout (location=1) out vec2 texCoords;
layout (location=2) flat out int textureID;

uniform mat3 projection;
uniform mat3 view;

void main()
{

	mat3 matrix = projection * view * modelMatrix;

	float frameWidth = 1.0 / aSpriteData.x; // Process Sprite Sheet

	float frameOffset = frameWidth * aSpriteData.y;

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.0001 * aLayer, 1.0);

	texCoords = vec2(vertexTexCoords.x * frameWidth + frameOffset, vertexTexCoords.y);
	
	color = aColor;
	textureID = aSpriteData.z;
}

)"