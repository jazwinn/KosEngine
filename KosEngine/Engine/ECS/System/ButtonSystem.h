/******************************************************************/
/*!
\file      ButtonSystem.h
\author	   Sean Tiu
\co-author  Rayner Tan
\par
\date      Oct 02, 2024
\brief     This file contains class for the Button system


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef BUTTONSYS_H
#define BUTTONSYS_H

#include "System.h"
#include "../ECS/Component/ButtonComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class ButtonSystem : public ISystem {

	public:

		/******************************************************************/
		/*!
		\fn      void ButtonSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the button system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the button system, allowing it to be updated for button interaction.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void ButtonSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the button system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the button system, stopping its updates for button interaction.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void ButtonSystem::m_Init()
		\brief   Initializes the button system.
		\details Prepares necessary resources and sets up the button system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void ButtonSystem::m_Update(const std::string& updateContext)
		\brief   Updates the button system.
		\param   updateContext - The context or description of the update.
		\details Performs updates on all entities registered to the button system for button interaction.
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
		\var     std::vector<ButtonComponent*> m_vecButtonComponentPtr
		\brief   Stores pointers to ButtonComponent instances for registered entities.
		\details Used to manage and update button interactions for entities in the system.
		*/
		/******************************************************************/
		std::vector<ButtonComponent*> m_vecButtonComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<NameComponent*> m_vecNameComponentPtr
		\brief   Stores pointers to NameComponent instances for registered entities.
		\details Used to reference and identify entities by name within the button system.
		*/
		/******************************************************************/
		std::vector<NameComponent*> m_vecNameComponentPtr;
	};
}

#endif CAMERASYS_H

