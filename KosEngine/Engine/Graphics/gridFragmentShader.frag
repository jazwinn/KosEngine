/******************************************************************/
/*!
\file      gridFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This fragment shader is responsible for rendering grid lines 
           with a gray color.

\details   The shader takes an input color (`col`) as a `vec4` and outputs the 
           color to the fragment (`fragColor`). It is designed for rendering 
           grid lines in a 2D space, where the grid lines are rendered using 
           the RGB components of the input color. The alpha component (`col.w`) 
           is preserved for transparency control.
*/
/******************************************************************/

R"( #version 460 core


	layout (location = 0) in vec4 col;
	layout (location = 0) out vec4 fragColor;

	void main() 
	{
		fragColor = vec4(col.x, col.y, col.z, col.w); // Gray color for grid lines
	}

)"