/******************************************************************/
/*!
\file      RenderDebugSystem.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains class for the DebugDrawingSystem


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef DEBUGDRAWSYS_H
#define DEBUGDRAWSYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/ColliderComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class DebugDrawingSystem : public ISystem {

	public:
		/******************************************************************/
		/*!
		\fn      void DebugDrawingSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the debug drawing system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the debug drawing system, allowing it to be visualized for debugging purposes.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void DebugDrawingSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the debug drawing system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the debug drawing system, stopping its debug visualization.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void DebugDrawingSystem::m_Init()
		\brief   Initializes the debug drawing system.
		\details Prepares necessary resources and sets up the debug drawing system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void DebugDrawingSystem::m_Update(const std::string& updateContext)
		\brief   Updates the debug drawing system.
		\param   updateContext - The context or description of the update.
		\details Performs updates on all entities registered in the debug drawing system for visual debugging.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;


	private:
		/******************************************************************/
		/*!
		\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
		\brief   Stores pointers to TransformComponent instances for registered entities.
		\details Used to efficiently access and update the transform data for debugging visual representation.
		*/
		/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<ColliderComponent*> m_vecColliderComponentPtr
		\brief   Stores pointers to ColliderComponent instances for registered entities.
		\details Used to visualize collider data for entities, aiding in debugging collision-related issues.
		*/
		/******************************************************************/
		std::vector<ColliderComponent*> m_vecColliderComponentPtr;

		std::vector<NameComponent*> m_vecNameComponentPtr;
	};

}



#endif DEBUGDRAWSYS_H
