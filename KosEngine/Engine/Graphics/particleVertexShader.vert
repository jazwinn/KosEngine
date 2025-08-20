/******************************************************************/
/*!
\file      particleFragmentShader.vert
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th March, 2025
\brief     This fragment shader is used to calcualte the particles
           coordinates
*/
/******************************************************************/

R"( #version 430 core


layout (location=0) in vec2 vertexPosition;
layout (location=2) in vec2 vertexTexCoords;


 struct Particle 
{
    float lifeSpan;      
    float rotation;
    float isActive;      
    int textureID;      
    int stripCount;     
    int frameNumber;    
    int layer;        
    float initialEmissionAngle; 
    vec2 position;       
    vec2 velocity;       
    vec2 acceleration;  
    vec2 scale;          
    vec4 color;
    float friction;
    int framesPerSecond;
    float frameTimer;
    float padding3;
};

layout(std430, binding = 0) buffer ParticleBuffer 
{
    Particle particles[];
};

layout (location=0) out vec4 color; 
layout (location=1) out vec2 texCoords;
layout (location=2) flat out int textureID;
layout (location=3) out float isActive;

uniform mat3 projection;
uniform mat3 view;

void main()
{
    uint id = gl_InstanceID;

     if (id >= particles.length())
    {
        return;
    }

    if (particles[id].isActive < 1.f)
    {
       gl_Position = vec4(0.0, 0.0, 0.0, 0.0);
       return;
    }

    vec2 particlePos = particles[id].position.xy;

    mat3 particleTranslateMat = mat3(1,0, 0,
                                    0,1,0,
                                    particlePos.x,particlePos.y,1);

    mat3 particleScaleMat = mat3(particles[id].scale.x, 0, 0,
                                0,particles[id].scale.y, 0,
                                0,0,1);

    float rotation = particles[id].rotation;
    mat3 rotMat = mat3(cos(rotation), -sin(rotation), 0,
                       sin(rotation), cos(rotation), 0,
                        0,0,1);

    mat3 particleMatrix =  particleTranslateMat * rotMat * particleScaleMat;

    mat3 matrix = projection * view * particleMatrix;



    gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.0001 * particles[id].layer, 1.0);

    float frameWidth = 1.0f;

    if (particles[id].stripCount > 0)
    {
        frameWidth = 1.0 / particles[id].stripCount;
    }

    float frameOffset = frameWidth * particles[id].frameNumber;

    texCoords = vec2(vertexTexCoords.x * frameWidth + frameOffset, vertexTexCoords.y);

    color = particles[id].color;

    textureID = particles[id].textureID;

    isActive = 2.f;


}

)"