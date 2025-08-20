/******************************************************************/
/*!
\file      textFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A fragment shader for rendering text.

           This shader samples a texture and applies a color 
           to the text based on its RGB values, allowing for 
           customizable text colors during rendering.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core


    in vec2 texCoords;
    out vec4 color;

    uniform sampler2D text;
    uniform vec3 textColor;

    void main()
    {    
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);

        if (sampled.a < 0.5)
        discard;

        color = vec4(textColor, 1.0) * sampled;
    } 

)"