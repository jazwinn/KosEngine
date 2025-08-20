/******************************************************************/
/*!
\file      lightingFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      31st Jan, 2025
\brief     Shader program for lighting attenuation and falloff based on light parameters. 

           This shader calculates lighting attenuation based on the distance from a light source. 
           The light source has both an inner radius and an outer radius, within which the 
           light intensity decreases smoothly. If the fragment is within the inner radius, 
           it receives full brightness, and between the inner and outer radius, the intensity 
           decreases following a quadratic falloff.

           The shader works with a directional light model, where the light's direction is 
           provided along with its intensity, inner radius, and outer radius.

           The output is a fragColor that includes the lighting color, taking into account 
           attenuation based on distance from the light source.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

R"( #version 460 core

layout(location=0) in vec4 color;
layout(location=1) in vec2 lightPos;
layout(location=2) in vec3 lightParams; //Intensity, Inner Radius, Outer Radius //Change outer Radius To Type // Change Inner Radius to Feather Amount

layout(location=5) in vec2 lightDirection;
layout (location=0) out vec4 fragColor;

void main() 
{
    float distance = length(lightDirection);

    float attenuation = 0.0;

    float innerRadius = lightParams.y;
    float outerRadius = lightParams.z;

    if (distance < innerRadius)
    {
        attenuation = 1.0;
    }
    else if (distance < outerRadius)
    {
        float t = (distance - innerRadius) / (outerRadius - innerRadius);
        attenuation = pow(clamp(1.0 - t, 0.0, 1.0), 4.0); // Quadratic falloff
    }

    vec3 lighting = color.rgb;
    fragColor = vec4(lighting, attenuation * lightParams.x);
}

)"