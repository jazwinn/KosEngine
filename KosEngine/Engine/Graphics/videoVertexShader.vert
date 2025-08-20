/******************************************************************/
/*!
\file      videoVertexShader.vert
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      1st March, 2025
\brief     A vertex shader used for video rendering. 

          

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


R"( #version 460 core


    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoords;

    uniform mat3 transformation;
    uniform mat3 projection;
    uniform mat3 view;
    uniform int layer;

void main()
{
    
    
    mat3 model = transformation;

    mat3 matrix = projection * view * model;

   
    gl_Position = vec4(vec2(matrix * vec3(aPos.xy, 1.f)),
					   -0.0001 * layer, 1.0);

    TexCoords = aTexCoord;
}

)"