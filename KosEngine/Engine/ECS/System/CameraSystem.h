/******************************************************************/
/*!
\file      CameraSystem.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      Oct 02, 2024
\brief     This file contains class for the DebugDrawingSystem


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef CAMERASYS_H
#define CAMERASYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/CameraComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class CameraSystem : public ISystem {

	public:

		/******************************************************************/
		/*!
		\fn      void CameraSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the camera system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the camera system, enabling it to be used for rendering or scene navigation.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void CameraSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the camera system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the camera system, stopping its updates for camera functionality.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void CameraSystem::m_Init()
		\brief   Initializes the camera system.
		\details Prepares necessary resources and sets up the camera system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void CameraSystem::m_Update(const std::string& updateContext)
		\brief   Updates the camera system.
		\param   updateContext - The context or description of the update.
		\details Performs updates on all entities registered in the camera system, adjusting their camera attributes.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;


	private:

		/******************************************************************/
		/*!
		\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
		\brief   Stores pointers to TransformComponent instances for registered entities.
		\details Used to efficiently access and update the transform data for entities with camera functionality.
		*/
		/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<CameraComponent*> m_vecCameraComponentPtr
		\brief   Stores pointers to CameraComponent instances for registered entities.
		\details Used to manage and update the camera settings and properties of entities in the system.
		*/
		/******************************************************************/
		std::vector<CameraComponent*> m_vecCameraComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<NameComponent*> m_vecNameComponentPtr
		\brief   Stores pointers to NameComponent instances for registered entities.
		\details Used to identify and manage entities by name within the camera system.
		*/
		/******************************************************************/
		std::vector<NameComponent*> m_vecNameComponentPtr;
	};

}



#endif CAMERASYS_H

