/******************************************************************/
/*!
\file      PathfindingComponent.h
\author    Clarence Boey
\par
\date      Jan 15, 2024
\brief     



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#ifndef PATHCOM_H
#define PATHCOM_H

#include "Component.h"
#include "../Pathfinding/AStarPathfinding.h"
#include <vector>
#include <string>

namespace ecs {

    class PathfindingComponent : public Component {
    public:
        vector2::Vec2 m_StartPos{ 0, 0 };        // Start position as {x, y}
        vector2::Vec2 m_TargetPos{ 0, 0 };       // Target position as {x, y}
        int m_GridKey;                           // Identifier for the grid
        //uint m_WaypointID;          // Calculated path


        REFLECTABLE(PathfindingComponent, m_StartPos, m_TargetPos, m_GridKey)
    };
}

#endif // PATHCOM_H