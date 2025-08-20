/******************************************************************/
/*!
\file      gridVertexShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This vertex shader is used for rendering grid lines, 
           determining the grid line color and transforming the vertex position 
           based on view and projection matrices.
*/
/******************************************************************/

R"( #version 460 core

    layout (location = 0) in vec3 aPos;

    uniform mat3 view;
    uniform mat3 projection;
    uniform vec3 debugColor;

    layout (location = 0) out vec4 col;

    void main() 
    {
         gl_Position = vec4(vec2(projection * vec3(view * vec3(aPos.xy, 1.f))), -0.99, 1.0);
         if ((aPos.x < 0.1 && aPos.x > -0.1) || (aPos.y < 0.1 && aPos.y > -0.1))
         {
            col = vec4(0,0,0,1);
         }
         else if (debugColor.x > 0.01)
         {
            col = vec4(debugColor, 1.0);
         }
         else
         {
             col = vec4(0.5,0.5,0.5,1.f);
         }
    }

)"