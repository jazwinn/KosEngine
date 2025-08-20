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
#ifndef RENDERTEXTSYS_H
#define RENDERTEXTSYS_H


#include "System.h"
#include "../ECS/Component/TextComponent.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs {

	class RenderTextSystem : public ISystem {

	private:


	public:

		/******************************************************************/
/*!
\fn      void RenderTextSystem::m_RegisterSystem(EntityID)
\brief   Registers an entity with the render text system.
\param   EntityID - The ID of the entity to be registered.
\details Adds the entity to the render text system, allowing it to render text on the screen.
*/
/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void RenderTextSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the render text system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the render text system, stopping its text rendering.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void RenderTextSystem::m_Init()
		\brief   Initializes the render text system.
		\details Prepares necessary resources and sets up the render text system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void RenderTextSystem::m_Update(const std::string& updateContext)
		\brief   Updates the render text system.
		\param   updateContext - The context or description of the update.
		\details Performs rendering updates on all entities registered in the system for text display.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;



	private:
		/******************************************************************/
/*!
\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
\brief   Stores pointers to TransformComponent instances for registered entities.
\details Used to efficiently access and update the transform data during text rendering.
*/
/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<TextComponent*> m_vecTextComponentPtr
		\brief   Stores pointers to TextComponent instances for registered entities.
		\details Used to manage and render text for entities in the system.
		*/
		/******************************************************************/
		std::vector<TextComponent*> m_vecTextComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<NameComponent*> m_vecNameComponentPtr
		\brief   Stores pointers to NameComponent instances for registered entities.
		\details Used to identify and reference entities by name within the system.
		*/
		/******************************************************************/
		std::vector<NameComponent*> m_vecNameComponentPtr;
	};

}



#endif RENDERTEXTSYS_H
