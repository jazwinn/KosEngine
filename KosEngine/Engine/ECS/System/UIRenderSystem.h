/******************************************************************/
/*!
\file      RenderSystem.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains class for the Render System

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef UIRENDERSYS_H
#define UIRENDERSYS_H


#include "System.h"
#include "../ECS/ECSList.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs{

	class UIRenderSystem : public ISystem {

	private:


	public:

		/***********s*******************************************************/
		/*!
		\fn      void RenderSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the render system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the render system, allowing it to be rendered on the screen.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void RenderSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the render system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the render system, stopping its rendering.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void RenderSystem::m_Init()
		\brief   Initializes the render system.
		\details Prepares necessary resources and sets up the render system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void RenderSystem::m_Update(const std::string& updateContext)
		\brief   Updates the render system.
		\param   updateContext - The context or description of the update.
		\details Performs rendering updates on all entities registered in the system for visual display.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;



	private:


		/******************************************************************/
		/*!
		\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
		\brief   Stores pointers to TransformComponent instances for registered entities.
		\details Used to efficiently access and update the transform data during rendering.
		*/
		/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<SpriteComponent*> m_vecSpriteComponentPtr
		\brief   Stores pointers to SpriteComponent instances for registered entities.
		\details Used to manage and render sprites for entities in the system.
		*/
		/******************************************************************/
		std::vector<UISpriteComponent*> m_vecUISpriteComponentPtr;

		std::vector<NameComponent*> m_vecNameComponentPtr;
	};
}


#endif