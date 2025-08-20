/******************************************************************/
/*!
\file      Image.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      15 Jan, 2025
\brief


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "../ECS.h"
#include "GridSystem.h"
#include "../ECS/Component/GridComponent.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"

#include "../Pathfinding/AStarPathfinding.h"

namespace ecs {

	void GridSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecGridComponentPtr.push_back((GridComponent*)ecs->m_ECS_CombinedComponentPool[TYPEGRIDCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void GridSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& GridComponentPtr : m_vecGridComponentPtr) {
			if (GridComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecGridComponentPtr.size() - 1;
		std::swap(m_vecGridComponentPtr[IndexID], m_vecGridComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecGridComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void GridSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPEGRIDCOMPONENT);
	}

	void GridSystem::m_Update(const std::string& scene)
	{
		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		//assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance(); //commented cuz unused

		if (m_vecGridComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not match");
			return;
		}


		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecGridComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			GridComponent* grid = m_vecGridComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			//graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance(); //unused


			//skip component not of the scene
			if ((transform->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			grid->m_Anchor.m_x = floor(transform->m_transformation.m_e20);
			grid->m_Anchor.m_y = floor(transform->m_transformation.m_e21) + 1.0f;

			transform->m_position.m_x = floor(transform->m_position.m_x);
			transform->m_position.m_y = floor(transform->m_position.m_y);

			transform->m_transformation.m_e20 = floor(transform->m_transformation.m_e20) + 0.5f;
			transform->m_transformation.m_e21 = floor(transform->m_transformation.m_e21) + 0.5f;

			//ECS* ecs = ECS::m_GetInstance();

			graphicsPipe->m_colliderGridData.push_back({ glm::mat3{transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
																transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
															transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22},glm::ivec2{grid->m_GridRowLength,grid->m_GridColumnLength} });

			graphicsPipe->m_gridColliderArrays.push_back(grid->m_IsWall);

			

		}


	}


}

