/******************************************************************/
/*!
\file      Image.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      Nov 20, 2024
\brief     

\dependencies
		 - STB image library for loading images.
		 - RapidJSON for serialization/deserialization.
		 - OpenGL for texture handling.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "../ECS.h"

#include "TilemapSystem.h"
#include "../ECS/Component/TilemapComponent.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"

namespace ecs {

	void TilemapSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecTilemapComponentPtr.push_back((TilemapComponent*)ecs->m_ECS_CombinedComponentPool[TYPETILEMAPCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void TilemapSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& TilemapComponentPtr : m_vecTilemapComponentPtr) {
			if (TilemapComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecTilemapComponentPtr.size() - 1;
		std::swap(m_vecTilemapComponentPtr[IndexID], m_vecTilemapComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecTilemapComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void TilemapSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPETILEMAPCOMPONENT);
	}

	void TilemapSystem::m_Update(const std::string& scene)
	{
		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		if (m_vecTilemapComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not match");
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecTilemapComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			TilemapComponent* tile = m_vecTilemapComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();


			//skip component not of the scene
			if ((tile->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			//ECS* ecs = ECS::m_GetInstance();

			if (assetmanager->m_imageManager.m_imageMap.find(tile->m_tilemapFile) == assetmanager->m_imageManager.m_imageMap.end()) continue;


			transform->m_position.m_x = floor(transform->m_position.m_x);
			transform->m_position.m_y = floor(transform->m_position.m_y);

			transform->m_transformation.m_e20 = floor(transform->m_transformation.m_e20) + 0.5f;
			transform->m_transformation.m_e21 = floor(transform->m_transformation.m_e21) + 0.5f;

			//float imageWidth = assetmanager->m_imageManager.m_imageMap.find(tile->m_tilemapFile)->second.m_width / 2.f;
			float imageHeight = static_cast<float>(assetmanager->m_imageManager.m_imageMap.find(tile->m_tilemapFile)->second.m_height);

			transform->m_scale = vector2::Vec2{ static_cast<float>(pipe->m_unitHeight) / imageHeight , static_cast<float>(pipe->m_unitHeight) / imageHeight };

			unsigned int textureid = assetmanager->m_imageManager.m_imageMap.find(tile->m_tilemapFile)->second.m_imageID;
			graphicsPipe->m_tilemapData.push_back({ glm::mat3{transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
																transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
															transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22},textureid, glm::ivec2{tile->m_rowLength,tile->m_columnLength}, glm::ivec2{tile->m_pictureRowLength, tile->m_pictureColumnLength }, tile->m_tileIndex
															, tile->m_tileLayer , glm::vec4{tile->m_color.m_x,tile->m_color.m_y, tile->m_color.m_z, tile->m_alpha} });
			graphicsPipe->m_tilemapIndexArrays.push_back(tile->m_tilePictureIndex);
			

		}


	}


}

