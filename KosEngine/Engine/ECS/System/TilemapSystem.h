/******************************************************************/
/*!
\file      Image.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      20 Nov, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#ifndef TILEMAPSYS_H
#define TILEMAPSYS_H


#include "System.h"
#include "../ECS/ECSList.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs {

	class TilemapSystem : public ISystem {

	private:


	public:

		/******************************************************************/
		/*!
		\fn      void TilemapSystem::m_RegisterSystem(EntityID)
		\brief   Registers an entity with the tilemap system.
		\param   EntityID - The ID of the entity to be registered.
		\details Adds the entity to the tilemap system, allowing it to manage tilemap rendering and interactions.
		*/
		/******************************************************************/
		void m_RegisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void TilemapSystem::m_DeregisterSystem(EntityID)
		\brief   Deregisters an entity from the tilemap system.
		\param   EntityID - The ID of the entity to be deregistered.
		\details Removes the entity from the tilemap system, stopping its tilemap updates.
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID) override;

		/******************************************************************/
		/*!
		\fn      void TilemapSystem::m_Init()
		\brief   Initializes the tilemap system.
		\details Prepares necessary resources and sets up the tilemap system for use.
		*/
		/******************************************************************/
		void m_Init() override;

		/******************************************************************/
		/*!
		\fn      void TilemapSystem::m_Update(const std::string& updateContext)
		\brief   Updates the tilemap system.
		\param   updateContext - The context or description of the update.
		\details Performs updates on all entities registered in the system for tilemap rendering and interactions.
		*/
		/******************************************************************/
		void m_Update(const std::string&) override;



	private:

		/******************************************************************/
		/*!
		\var     std::vector<TransformComponent*> m_vecTransformComponentPtr
		\brief   Stores pointers to TransformComponent instances for registered entities.
		\details Used to efficiently access and update the transform data for entities in the tilemap system.
		*/
		/******************************************************************/
		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		/******************************************************************/
		/*!
		\var     std::vector<TilemapComponent*> m_vecTilemapComponentPtr
		\brief   Stores pointers to TilemapComponent instances for registered entities.
		\details Used to manage and render tilemaps for entities in the system.
		*/
		/******************************************************************/
		std::vector<TilemapComponent*> m_vecTilemapComponentPtr;

		std::vector<NameComponent*> m_vecNameComponentPtr;
	};

}



#endif