/******************************************************************/
/*!
\file      debugVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A vertex shader used for debug rendering.

           This shader calculates vertex positions based on the 
           model matrix and passes a collision status to the fragment 
           shader to aid in debugging collision detection.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core


layout (location=0) in vec2 vertexPosition;

layout (location=4) in float isCollided;

layout (location=7) in mat3 modelMatrix;

layout (location=0) out float collided;

uniform mat3 projection;
uniform mat3 view;

void main()
{

	mat3 matrix = projection * view * modelMatrix;

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -1.0, 1.0);

	collided = isCollided;
}

)"