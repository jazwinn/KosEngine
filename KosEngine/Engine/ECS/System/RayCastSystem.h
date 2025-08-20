/*!
\file      RayCastSystem.h
\author	   Jazwinn Ng
\par       jazwinn.ng@digipen.edu
\date      Jan 20, 2025
\brief     This file holds the declaration of the RayCastSystem class

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef RAYSYS_H
#define RAYSYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/RaycastComponent.h"
#include "../ECS/Component/NameComponent.h"

namespace ecs {

	class RayCastSystem : public ISystem {

	public:

		/******************************************************************/
		/*!
		\fn      void PhysicsSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the physics system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the physics system, enabling it to participate in physics calculations.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void PhysicsSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the physics system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the physics system, stopping physics calculations for it.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void PhysicsSystem::m_Init()
		\brief   Initializes the physics system.
		\details Prepares necessary resources and sets up the physics system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void PhysicsSystem::m_Update(const std::string& updateContext)
		\brief   Updates the physics system.
		\param   updateContext - The context or description of the update.
		\details Performs physics calculations on all entities registered in the system.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;


	private:
		/******************************************************************/
		/*!
		\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
		\brief   Stores pointers to TransformComponent instances for registered entities.
		\details Used to efficiently access and update the transform data during physics calculations.
		*/
		/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<RaycastComponent*> m_vecRigidBodyComponentPtr
		\brief   Stores pointers to RaycastComponent instances for registered entities.
		\details Used to manage and update the physical properties and dynamics of entities in the system.
		*/
		/******************************************************************/
		std::vector<RaycastComponent*> m_vecRaycastComponentPtr;
		std::vector<NameComponent*> m_vecNameComponentPtr;
	};

}



#endif MOVESYS_H


