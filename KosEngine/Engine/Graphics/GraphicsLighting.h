/******************************************************************/
/*!
\file      GraphicLighting.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file implements the `GraphicsPipe` class functions for handling model transformations
		   and managing debug box and circle data within a graphics pipeline.

\details   The `LightType` enum represents different types 
            of lighting effects that can be used within the
            graphics pipeline. This file includes necessary
            dependencies such as glm for vector operations.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef GRAPHLIGHT_H
#define GRAPHLIGHT_H

#include <glm.hpp>
#include <vector>
#include <memory>

namespace graphicpipe

{
    enum LightType
    {
        MULTIPLY,
        GLOW
    };
}

#endif GRAPHLIGHT_H