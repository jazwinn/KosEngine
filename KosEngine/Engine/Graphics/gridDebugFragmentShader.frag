/******************************************************************/
/*!
\file      gridDebugFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      17th Jan, 2025
\brief     A fragment shader used for debugging the grid system rendering.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
R"( #version 460 core

	layout (location=0) in vec4 color;
	layout (location=0) out vec4 fragColor;


	void main()
	{
		fragColor = color;
	}
)"