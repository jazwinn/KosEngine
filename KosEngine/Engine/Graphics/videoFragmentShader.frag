/******************************************************************/
/*!
\file      videoFragmentShader.frag
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      1st March, 2025
\brief     A fragment shader used for video rendering. 

          

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

    in vec2 TexCoords;
    out vec4 FragColor;

    uniform sampler2D yTexture;
    uniform sampler2D uTexture;
    uniform sampler2D vTexture;

	mat4 rec601 = mat4(
		1.16438,  0.00000,  1.59603, -0.87079,
		1.16438, -0.39176, -0.81297,  0.52959,
		1.16438,  2.01723,  0.00000, -1.08139,
		0, 0, 0, 1
	);
	  
	void main() {
		float y = texture2D(yTexture, TexCoords).r;
		float cb = texture2D(uTexture, TexCoords).r;
		float cr = texture2D(vTexture, TexCoords).r;

		FragColor = vec4(y, cb, cr, 1.0) * rec601;
	}
)"