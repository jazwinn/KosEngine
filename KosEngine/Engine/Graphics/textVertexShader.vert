/******************************************************************/
/*!
\file      textVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A vertex shader for rendering text.

           This shader processes vertex positions and texture 
           coordinates for text rendering. It applies a projection 
           matrix to position the text in 2D space and adjusts 
           the z-value to ensure text is rendered in front.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform mat3 projection;

uniform mat3 view;

uniform mat3 rotate;

uniform vec2 point;

uniform int layer;

void main()
{
    vec3 pos = vec3(vertex.xy, 1.0);
    pos = vec3(pos.x - point.x, pos.y - point.y, pos.z);
    pos = rotate * pos;  // Apply rotation
    pos = vec3(pos.x + point.x, pos.y + point.y, pos.z);
    pos = view * pos;      // Apply view
    pos = projection * pos; // Apply projection
    gl_Position = vec4(pos.xy, -0.0001 * layer, 1.0);
    texCoords = vertex.zw;
}  

)"