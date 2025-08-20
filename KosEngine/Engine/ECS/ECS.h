/******************************************************************/
/*!
\file      ECS.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the ECS class that is the factor that
		   creates, destroy and duplicates entity. It is also the point
		   to retrieve component pointers of all component type.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef ECS_H
#define ECS_H

#include "../Config/pch.h"

#include "../ECS/Component/ComponentPool.h"
#include "../ECS/Component/Component.h"

#include "../ECS/System/System.h"
#include "Layers.h"

#include "ECSList.h"
#include "../ECS/System/SystemList.h"


namespace ecs {

	class ECS {

	private:



		ECS() = default;



	public:
		//singleton
		static ECS* m_GetInstance() {
			if (!m_InstancePtr) {
				m_InstancePtr.reset(new ECS{});
			}
			return m_InstancePtr.get();
		}

		/******************************************************************/
		/*!
		\def       m_Load()
		\brief     Allocate memory to the the heap and storing it on a map
		*/
		/******************************************************************/
		void m_Load();

		/******************************************************************/
		/*!
		\def       m_Init()
		\brief     Call on all systems initialization
		*/
		/******************************************************************/
		void m_Init();

		/******************************************************************/
		/*!
		\def       m_Update(const std::string&)
		\brief     Call on all systems update function
		*/
		/******************************************************************/
		void m_Update(float DeltaTime);

		/******************************************************************/
		/*!
		\def       m_Unload()
		\brief     Release all memeory allocated
		*/
		/******************************************************************/
		void m_Unload();

		/******************************************************************/
		/*!
		\def       m_CreateEntity()
		\brief     Creates an entity with a name & transform component
		*/
		/******************************************************************/
		EntityID m_CreateEntity(std::string scene);
		/******************************************************************/
		/*!
		\def       m_DuplicateEntity()
		\brief     Creates entity and duplicate the data to the new entity
		*/
		/******************************************************************/
		EntityID m_DuplicateEntity(EntityID, std::string scene = {});
		/******************************************************************/
		/*!
		\def       m_DeleteEntity()
		\brief     Removes entities component pointer from all system
		*/
		/******************************************************************/
		bool m_DeleteEntity(EntityID);

		/******************************************************************/
		/*!
		\fn      bool ECS::m_RestoreEntity(EntityID)
		\brief   Restores a previously deleted entity.
		\param   EntityID - The ID of the entity to be restored.
		\return  True if the entity was successfully restored, false otherwise.
		*/
		/******************************************************************/
		bool m_RestoreEntity(EntityID);


		/******************************************************************/
		/*!
		\fn      template <typename T> void ECS::m_AddComponentToECS(ComponentType type)
		\brief   Adds a component type to the ECS component pool.
		\param   type - The type of component to add to the ECS.
		\details Allocates memory for a new component type and adds it to the ECS, making it available for assignment to entities.
		*/
		/******************************************************************/
		template <typename T>
		void m_AddComponentToECS(ComponentType);


		/******************************************************************/
		/*!
		\def       m_AddComponent()
		\brief     Assign a component to the entity and update the entity
				   map bitflag
		\return	   pointer to the newly created component
		*/
		/******************************************************************/
		void* m_AddComponent(ComponentType, EntityID);

		/******************************************************************/
		/*!
		\fn      bool ECS::m_RemoveComponent(ComponentType Type, EntityID ID)
		\brief   Removes a component from an entity.
		\param   Type - The type of component to remove.
		\param   ID - The ID of the entity from which the component will be removed.
		\return  True if the component was successfully removed, false otherwise.
		*/
		/******************************************************************/
		bool m_RemoveComponent(ComponentType Type, EntityID ID);

		/******************************************************************/
		/*!
		\def       m_RegisterSystems()
		\brief     Checks entity map bitflag against all the system's signature
				   if any matches, the entitie's component pointer is stored
				   in the system
		*/
		/******************************************************************/
		void m_RegisterSystems(EntityID);
		/******************************************************************/
		/*!
		\def       m_DeregisterSystem()
		\brief     Removes entity's component from the system
		*/
		/******************************************************************/
		void m_DeregisterSystem(EntityID);

		/******************************************************************/
		/*!
		\var     std::unordered_map<ComponentType, std::shared_ptr<IComponentPool>> m_ECS_CombinedComponentPool
		\brief   Stores pools for each component type.
		\details Used to manage memory allocation and retrieval for components of different types.
		*/
		/******************************************************************/
		std::unordered_map<ComponentType, std::shared_ptr<IComponentPool>> m_ECS_CombinedComponentPool;

		/******************************************************************/
		/*!
		\var     std::map<TypeSystem, std::shared_ptr<ISystem>> m_ECS_SystemMap
		\brief   Stores pointers to each registered system.
		\details Used to manage and update all systems within the ECS framework.
		*/
		/******************************************************************/
		std::map<TypeSystem, std::shared_ptr<ISystem>> m_ECS_SystemMap;

		/******************************************************************/
		/*!
		\var     std::unordered_map<EntityID, std::bitset<TOTALTYPECOMPONENT>> m_ECS_EntityMap
		\brief   Maps entity IDs to their component bitsets.
		\details Used to track which components each entity has, facilitating system registration.
		*/
		/******************************************************************/
		std::unordered_map<EntityID, std::bitset<TOTALTYPECOMPONENT>> m_ECS_EntityMap;

		struct SceneID {
			bool m_isActive{ true };
			bool m_isPrefab{ false };
			ecs::EntityID m_prefabID = 0;
			std::vector<EntityID> m_sceneIDs;

		};

		/******************************************************************/
		/*!
		\var     std::unordered_map<std::string, SceneID> m_ECS_SceneMap
		\brief   Stores scene data, including entity IDs and scene properties.
		\details Used to manage entities and their states within each scene.
		*/
		/******************************************************************/
		std::unordered_map<std::string, SceneID> m_ECS_SceneMap{};

		//store type conversion
		//using ActionFunction = std::function<void(void*, void (*)(void*))>;
		//std::unordered_map<ComponentType, ActionFunction> m_callFunctionToComponent_Map;// TODO in future, std::any doesnt work

		/******************************************************************/
		/*!
		\var     layer::LayerStack m_layersStack
		\brief   Manages different layers within the ECS.
		\details Used to organize and manage layers that entities can belong to.
		*/
		/******************************************************************/
		layer::LayerStack m_layersStack;

		EntityID m_EntityCount{};

		float m_DeltaTime{};

		GAMESTATE m_nextState{ STOP };
		GAMESTATE m_getState() { return m_state; };

		std::unordered_map<GAMESTATE, std::bitset<TOTALTYPESYSTEM>> m_gameStateSystems;

	private:
		static std::unique_ptr<ECS> m_InstancePtr;

		//modify from set next state
		GAMESTATE m_state{ STOP };

		/******************************************************************/
		/*!
		\var     std::vector<std::pair<ecs::EntityID, std::bitset<TOTALTYPECOMPONENT>>> m_deletedentity
		\brief   Stores deleted entities and their associated component bitsets.
		\details Allows for restoration of deleted entities if needed.
		*/
		/******************************************************************/
		std::vector<std::pair<ecs::EntityID, std::bitset<TOTALTYPECOMPONENT>>> m_deletedentity;

	};
}

#endif  ECS_H