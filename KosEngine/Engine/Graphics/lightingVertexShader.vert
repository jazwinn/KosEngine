/******************************************************************/
/*!
\file      lightingVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      31st January 2025
\brief     Vertex shader for transforming model vertices and passing lighting data to the fragment shader. 

           This vertex shader applies a model-view-projection transformation to the vertex positions 
           and passes various lighting-related attributes such as the light position, direction, and 
           associated light parameters to the fragment shader for further processing. The light direction 
           and position are derived from the model matrix, while the color and lighting parameters are 
           passed as input attributes.

           The shader is used for rendering objects with lighting, including their interaction with the light 
           source, which is necessary for effects like lighting attenuation and intensity computations.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core


layout (location=0) in vec2 vertexPosition;
layout (location=1) in vec2 vertexNDCCoords;
layout (location=2) in vec2 vertexTexCoords;

layout (location=5) in vec3 aLightParams;
layout (location=11) in vec4 aColor;

layout (location=6) in mat3 modelMatrix;

layout (location=0) out vec4 color; 
layout (location=1) out vec2 lightPos;
layout (location=2) out vec3 lightParams;
layout (location=5) out vec2 lightDirection;

uniform mat3 projection;
uniform mat3 view;

void main()
{
	mat3 matrix = projection * view * modelMatrix;

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.999 - (gl_InstanceID * 0.000001) , 1.0);

	lightDirection = vertexNDCCoords;

	lightPos = vec2(modelMatrix[0][2],modelMatrix[1][2]);

	color = aColor;
	lightParams = aLightParams;
}

)"