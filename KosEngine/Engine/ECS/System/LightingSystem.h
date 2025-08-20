/******************************************************************/
/*!
\file      RenderSystem.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Jan 30, 2025
\brief     This file contains class for the Render System

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef LIGHTSYS_H
#define LIGHTSYS_H

#include "System.h"
#include "../ECS/ECSList.h"

namespace ecs {

    /******************************************************************/
    /*!
    \class   CollisionSystem
    \brief   Manages the collision detection between entities in the ECS.

             The CollisionSystem processes the collision interactions
             between entities that have a TransformComponent and
             ColliderComponent. It handles the registration and
             deregistration of entities and updates the system every
             frame to check for and respond to collisions.
    */
    /******************************************************************/
    class LightingSystem : public ISystem {

    public:

        /******************************************************************/
        /*!
        \fn      void CollisionSystem::m_RegisterSystem(EntityID)
        \brief   Registers an entity with the CollisionSystem.

                 This function adds the entity's relevant components,
                 such as TransformComponent and ColliderComponent, to
                 the internal storage for processing collision detection.

        \param   EntityID - The unique identifier of the entity to be registered.
        */
        /******************************************************************/
        void m_RegisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void CollisionSystem::m_DeregisterSystem(EntityID)
        \brief   Deregisters an entity from the CollisionSystem.

                 Removes the entity and its components from the collision
                 processing system, stopping further collision checks
                 for this entity.

        \param   EntityID - The unique identifier of the entity to be deregistered.
        */
        /******************************************************************/
        void m_DeregisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void CollisionSystem::m_Init()
        \brief   Initializes the CollisionSystem.

                 Prepares the system for collision detection by setting up
                 necessary structures and ensuring all registered entities
                 are ready for collision processing.
        */
        /******************************************************************/
        void m_Init() override;

        /******************************************************************/
        /*!
        \fn      void CollisionSystem::m_Update(const std::string&)
        \brief   Updates the CollisionSystem on every frame.

                 This function processes collision detection for all registered
                 entities, checking for intersections between colliders and
                 handling the necessary collision responses.
        */
        /******************************************************************/
        void m_Update(const std::string&) override;

    private:
        //! Vector storing pointers to the TransformComponent of registered entities
        std::vector<TransformComponent*> m_vecTransformComponentPtr;
        //! Vector storing pointers to the ColliderComponent of registered entities
       // std::vector<ColliderComponent*> m_vecColliderComponentPtr;
       // std::vector<NameComponent*>m_vecNameComponentPtr;
        std::vector<LightingComponent*> m_vecLightingComponentPtr;
        std::vector<NameComponent*> m_vecNameComponentPtr;

    };

}


#endif
