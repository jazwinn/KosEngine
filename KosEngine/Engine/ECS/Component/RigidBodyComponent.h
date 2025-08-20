/******************************************************************/
/*!
\file      RigidBodyComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
           Clarence Boey, c.boey, 2301547
\par       jazwinn.ng@digipen.edu
           c.boey@digipen.edu
\date      Oct 13, 2024
\brief     This file contains data for rigid body


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef RIGIDCOM_H
#define RIGIDCOM_H

#include "Component.h"

namespace ecs {

    /******************************************************************/
    /*!
    \class     RigidBodyComponent
    \brief     Contains rigid body data for 2D physics in the game.
    */
    /******************************************************************/
    class RigidBodyComponent : public Component {

    public:

        // Linear velocity in the 2D world
        vector2::Vec2 m_Velocity{};

        // Linear acceleration in the 2D world
        vector2::Vec2 m_Acceleration{};

        // Angle of rotation in radians or degrees, where it faces.
        float m_Rotation{};

        // Angular velocity (rate of rotation change), like how fast it rotates
        float m_AngularVelocity{};

        // Angular acceleration, rate of change of angular velocity
        float m_AngularAcceleration{};

        // Mass and inverse mass (0 means infinite mass for static objects)
        float m_Mass{ 1.0f };
        float m_InverseMass{ 1.0f }; //Inverse mass is the reciprocal of the Mass, so it's 1/Mass.

        // Linear and angular damping (to simulate friction or air resistance, can EDIT when we test)
        float m_LinearDamping{ 0.99f };
        float m_AngularDamping{ 0.99f };

        // Force accumulator for this frame
        vector2::Vec2 m_Force{};

        // Torque accumulator for this frame
        float m_Torque{};

        // Is Kinematic (if true, physics simulation is bypassed)
        bool m_IsKinematic{ false };

        // Is Static (if true, the object is immovable and not affected by forces)
        bool m_IsStatic{ false };

        //Previous Position 
        vector2::Vec2 m_PrevPos{};


        //Direction Vector
        vector2::Vec2 m_DirectionVector{};

        vector2::Vec2 m_PrevDirVec{0.f,1.f};

        REFLECTABLE(RigidBodyComponent, m_Velocity, m_Acceleration, m_Rotation, m_AngularVelocity, m_AngularAcceleration, m_Mass, m_InverseMass, m_LinearDamping, m_AngularDamping, m_Force, m_Torque, m_IsKinematic, m_IsStatic)
    };
}

#endif // RIGIDCOM_H
