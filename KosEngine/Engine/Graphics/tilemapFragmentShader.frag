/******************************************************************/
/*!
\file      tilemapFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      29th Nov, 2024
\brief     A fragment shader for tilemaps that blends vertex colors 
           with a texture and handles transparency by discarding fragments
           with very low alpha values.

Uniforms:
           - `textureID`: A `sampler2D` representing the texture to sample.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

	layout (location=0) in vec4 color;
	layout (location=1) in vec2 texCoords;

	layout (location=0) out vec4 fragColor;

	uniform sampler2D textureID;
	uniform float globalBrightness;


	void main()
	{
		 vec4 texColor = texture(textureID, texCoords) * vec4(color);

		 if(texColor.a < 0.1)
		 discard;
    
		fragColor = vec4(vec3(texColor.xyz),texColor.w);
	}
)"