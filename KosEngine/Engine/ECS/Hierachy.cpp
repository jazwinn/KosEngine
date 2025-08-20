/********************************************************************/
/*!
\file      Hierachy.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file contains the definitions for the `Hierachy` class, which manages
		   hierarchical relationships between entities in the ECS framework.
		   - m_SetParent: Defines a parent-child relationship between entities.
		   - m_RemoveParent: Detaches an entity from its parent.
		   - m_GetParent: Returns the parent of a specified entity.
		   - m_GetChild: Provides a list of children for a specified parent entity.
		   - m_UpdateChildScene: Updates the scene for all child entities recursively.

This file supports efficient parent-child relationship management, enabling scene
organization and transformations of entities within the ECS system.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "Hierachy.h"
#include "../Asset Manager/SceneManager.h"

namespace ecs {

	void Hierachy::m_SetParent(EntityID parent, EntityID child) {

		ECS* ecs = ECS::m_GetInstance();

		m_RemoveParent(child);

		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		//checks if child is already in parent
		if (m_GetParent(child).has_value()) {
			return;
		}

		//checks if parent is getting dragged into its child
		EntityID id = parent;
		while (m_GetParent(id).has_value()) {
			EntityID checkParentid = m_GetParent(id).value();
			if (checkParentid == child) {
				LOGGING_WARN("Cannot assign parent to its own child");
				return;
			}
			id = checkParentid;

		}


		parentTransform->m_childID.push_back(child);

		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		childTransform->m_haveParent = true;
		childTransform->m_parentID = parent;
	}

	void Hierachy::m_RemoveParent(EntityID child) {
		// removes id from both the child and the parents vector
		ECS* ecs = ECS::m_GetInstance();

		if (!m_GetParent(child).has_value()) {
			// does not have parrent
			return;
		}

		EntityID parent = m_GetParent(child).value();
		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		size_t pos{};
		for (EntityID& id : parentTransform->m_childID) {
			if (child == id) {
				parentTransform->m_childID.erase(parentTransform->m_childID.begin() + pos);
				break;
			}
			pos++;
		}


		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		childTransform->m_haveParent = false;
		childTransform->m_parentID = 0;
	}

	std::optional<EntityID> Hierachy::m_GetParent(EntityID child)
	{
		ECS* ecs = ECS::m_GetInstance();
		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		if (!childTransform->m_haveParent) {
			return std::optional<EntityID>();
		}

		return childTransform->m_parentID;

	}

	std::optional<std::vector<EntityID>>Hierachy::m_GetChild(EntityID parent)
	{
		ECS* ecs = ECS::m_GetInstance();
		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		if (parentTransform->m_childID.size() <= 0) {
			return std::optional<std::vector<EntityID>>();
		}

		return parentTransform->m_childID;

	}

	void Hierachy::m_UpdateChildScene(EntityID parent)
	{
		scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
		std::string parentscene = scenemanager->GetSceneByEntityID(parent).value();
		const auto& child = m_GetChild(parent);

		if (child.has_value()) {
			auto& children = child.value();

			for (auto& childid : children) {
				//if child scene does not belong to parent scene change scene
				//create a update child function that recurse	
				std::string childscene = scenemanager->GetSceneByEntityID(childid).value();
				if (parentscene != childscene) {
					scenemanager->m_SwapScenes(childscene, parentscene, childid);
				}
				//check if child have more children and call a recursion
				if (m_GetChild(childid).has_value()) {
					m_UpdateChildScene(childid);
				}


			}


		}


		

	}

}