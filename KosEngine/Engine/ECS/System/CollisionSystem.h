/******************************************************************/
/*!
\file      CollisionSystem.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 29, 2024
\brief     This file contains the declaration of the CollisionSystem class,
           which is part of the Entity Component System (ECS) framework.

           The CollisionSystem class is responsible for managing and
           processing collisions between entities that have both
           Transform and Collider components. It registers and
           deregisters entities with collision-related components and
           updates the system to handle collisions during the game loop.

           The class extends the ISystem interface, ensuring it adheres
           to the system management structure within the ECS framework.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef COLLSYS_H
#define COLLSYS_H

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
    class CollisionSystem : public ISystem {

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
        std::vector<ColliderComponent*> m_vecColliderComponentPtr;

        /******************************************************************/
        /*!
        \var     std::vector<NameComponent*> m_vecNameComponentPtr
        \brief   Stores pointers to NameComponent instances for registered entities.
        \details Used to identify and manage entities by name during collision processing, aiding in debugging and filtering.
        */
        /******************************************************************/
        std::vector<NameComponent*>m_vecNameComponentPtr;


    };

}

#endif // COLLSYS_H
