/******************************************************************/
/*!
\file      AnimationSystem.h
\author    Sean Tiu, s.tiu, 2303398
\par       s.tiu@digipen.edu
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
/********************************************************************/
#ifndef ANIMATIONSYS_H
#define ANIMATIONSYS_H

#include "System.h"
#include "../ECS/Component/AnimationComponent.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/SpriteComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class AnimationSystem : public ISystem {

	public:

		/******************************************************************/
		/*!
		\fn      void AnimationSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the animation system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the animation system, allowing it to be updated.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void AnimationSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the animation system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the animation system, stopping its updates.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void AnimationSystem::m_Init()
		\brief   Initializes the animation system.
		\details Prepares necessary resources and sets up the animation system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void AnimationSystem::m_Update(const std::string& updateContext)
		\brief   Updates the animation system.
		\param   updateContext - The context or description of the update.
		\details Performs updates on all entities registered to the animation system.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;


	private:

		/******************************************************************/
		/*!
		\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
		\brief   Stores pointers to TransformComponent instances for registered entities.
		\details Used to efficiently access and update the transform data of entities.
		*/
		/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<AnimationComponent*> m_vecAnimationComponentPtr
		\brief   Stores pointers to AnimationComponent instances for registered entities.
		\details Used to manage and update animations of entities in the system.
		*/
		/******************************************************************/
		std::vector<AnimationComponent*> m_vecAnimationComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<SpriteComponent*> m_vecSpriteComponentPtr
		\brief   Stores pointers to SpriteComponent instances for registered entities.
		\details Used to handle sprite rendering details for animated entities.
		*/
		/******************************************************************/
		std::vector<SpriteComponent*> m_vecSpriteComponentPtr;

		std::vector<NameComponent*> m_vecNameComponentPtr;

	};

}



#endif ANIMATIONSYS_H
