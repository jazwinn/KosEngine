/******************************************************************/
/*!
\file      frameBufferFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A fragment shader for framebuffer rendering.

           This shader samples from the screen texture using the 
           provided texture coordinates and outputs the final 
           color to the screen.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

uniform float globalBrightness;

void main()
{
    vec2 pixelCoord = gl_FragCoord.xy;
    vec4 col = texture(screenTexture, texCoords);
   
   int pixelY = int(pixelCoord.y);

    //if (pixelY % 4 == 0)
    //{
        //col = col * 0.5f;
    //}
     fragColor = col;
} 

)"