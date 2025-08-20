/******************************************************************/
/*!
\file      finalPassFragmentShader.frag
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      17th Jan, 2025
\brief     A fragment shader used for final pass rendering.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
R"( #version 460 core


out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D multiLightTexture;
uniform sampler2D unlitScreenTexture;
uniform sampler2D additiveLightTexture;
uniform float globalBrightness;

void main()
{
    vec4 sceneColor = texture(screenTexture, texCoords);
    vec4 multipliedLightColor = texture(multiLightTexture, texCoords);
    vec4 additiveLightColor = texture(additiveLightTexture, texCoords);
    vec4 unlitSceneColor = texture(unlitScreenTexture, texCoords);

    vec4 darkenedScene = sceneColor * globalBrightness;  
    vec4 restoredBrightness = vec4 (0,0,0,0.f);
    restoredBrightness.r = mix(darkenedScene.r, sceneColor.r, multipliedLightColor.r);
    restoredBrightness.g = mix(darkenedScene.g, sceneColor.g, multipliedLightColor.g);
    restoredBrightness.b = mix(darkenedScene.b, sceneColor.b, multipliedLightColor.b);
    restoredBrightness.a = mix(darkenedScene.a, sceneColor.a, multipliedLightColor.a);


    restoredBrightness.r = mix(restoredBrightness.r, sceneColor.r,additiveLightColor.r);
    restoredBrightness.g = mix(restoredBrightness.g, sceneColor.g, additiveLightColor.g);
    restoredBrightness.b = mix(restoredBrightness.b, sceneColor.b,additiveLightColor.b);
    restoredBrightness.a = mix(restoredBrightness.a, sceneColor.a, additiveLightColor.a);
       

    vec4 light = vec4(multipliedLightColor.rgba);

    vec4 multipliedLighting = restoredBrightness;

    float r = max(darkenedScene.r, multipliedLighting.r);
    float g = max(darkenedScene.g, multipliedLighting.g);
    float b = max(darkenedScene.b, multipliedLighting.b);
    float a = max(darkenedScene.a, multipliedLighting.a);

    // I should add another pass here to add glowing lights

    vec4 final = vec4(r,g,b,1.0) + additiveLightColor;
    vec4 litScene = clamp(final, 0.0, 1.0);
    fragColor = litScene;

    if(unlitSceneColor.r != 0 || unlitSceneColor.g != 0 || unlitSceneColor.b != 0 || unlitSceneColor.a != 0)
    {
        fragColor = unlitSceneColor;
    }
} 

)"