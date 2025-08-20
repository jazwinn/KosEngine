/******************************************************************/
/*!
\file      particleFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th March, 2025
\brief     This fragment shader is used to render the particles
*/
/******************************************************************/

R"( #version 430 core

	layout (location=0) in vec4 color;
	layout (location=1) in vec2 texCoords;

	layout (location=2) flat in int textureID;

	layout (location=3) in float isActive;

	layout (location=0) out vec4 fragColor;

	uniform sampler2D textures[192]; // Subscript indicates max texture amount

	void main()
	{
		if (isActive < 2.f)
		{
			discard;
			return;
		}
	 
		if (textureID < 200)
		{
			vec4 texColor = texture(textures[textureID], texCoords) * vec4(color);

			if(texColor.a < 0.1)
			discard;
			fragColor = vec4(vec3(texColor.xyz),texColor.w);
		}
		else
		{
			fragColor = color;
		}
		
	}
)"