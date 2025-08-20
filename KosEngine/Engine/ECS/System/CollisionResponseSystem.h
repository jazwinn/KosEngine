/******************************************************************/
/*!
\file      CollsionResponseSystem.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains class for collision response system


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef COLLRESPSYS_H
#define COLLRESPSYS_H

#include "System.h"
#include "../ECS/Component/RigidBodyComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {
	/******************************************************************/
	/*!
	\class     CollisionResponseSystem
	\brief     Runs the logic of the collision response
	*/
	/******************************************************************/
	class CollisionResponseSystem : public ISystem {

	public:
		/******************************************************************/
		/*!
		\fn      void CollisionResponseSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the collision response system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the collision response system, enabling collision detection and response.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void CollisionResponseSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the collision response system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the collision response system, stopping its collision responses.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void CollisionResponseSystem::m_Init()
		\brief   Initializes the collision response system.
		\details Prepares necessary resources and sets up the collision response system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void CollisionResponseSystem::m_Update(const std::string& updateContext)
		\brief   Updates the collision response system.
		\param   updateContext - The context or description of the update.
		\details Processes collision responses for all entities registered in the system.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;


	private:
		/******************************************************************/
		/*!
		\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
		\brief   Stores pointers to TransformComponent instances for registered entities.
		\details Used to efficiently access and update the transform data of entities involved in collisions.
		*/
		/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<ColliderComponent*> m_vecColliderComponentPtr
		\brief   Stores pointers to ColliderComponent instances for registered entities.
		\details Used to manage and update the collider data of entities in the system.
		*/
		/******************************************************************/
		std::vector<ColliderComponent*> m_vecColliderComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<RigidBodyComponent*> m_vecRigidBodyComponentPtr
		\brief   Stores pointers to RigidBodyComponent instances for registered entities.
		\details Used to manage and update the physical properties and dynamics of entities in the system.
		*/
		/******************************************************************/
		std::vector<RigidBodyComponent*> m_vecRigidBodyComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<NameComponent*> m_vecNameComponentPtr
		\brief   Stores pointers to NameComponent instances for registered entities.
		\details Used to identify entities by name within the collision response system, aiding in debugging and tracking.
		*/
		/******************************************************************/
		std::vector<NameComponent*> m_vecNameComponentPtr;

	};

}



#endif COLLRESPSYS_H
