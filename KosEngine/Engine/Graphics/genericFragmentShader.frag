/******************************************************************/
/*!
\file      genericFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     A basic fragment shader for rendering textured and animated
           sprites using instanced rendering.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

	layout (location=0) in vec4 color;
	layout (location=1) in vec2 texCoords;

	layout (location=2) flat in int textureID;

	layout (location=0) out vec4 fragColor;

	uniform sampler2D textures[192]; // Subscript indicates max texture amount
	uniform float globalBrightness;


	void main()
	{
		 vec4 texColor = texture(textures[textureID], texCoords) * vec4(color);

		 if(texColor.a < 0.1)
		 discard;
    
		fragColor = vec4(vec3(texColor.xyz),texColor.w);
	}
)"