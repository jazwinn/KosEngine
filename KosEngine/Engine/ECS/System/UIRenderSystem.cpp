/******************************************************************/
/*!
\file      RenderSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the RenderSystem
		   class. It passes the transform data into the graphics
		   pipeline for it to be rendered.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "../ECS.h"

#include "UIRenderSystem.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"

namespace ecs {

	void UIRenderSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecUISpriteComponentPtr.push_back((UISpriteComponent*)ecs->m_ECS_CombinedComponentPool[TYPEUISPRITECOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void UIRenderSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& SpriteComponentPtr : m_vecUISpriteComponentPtr) {
			if (SpriteComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}
			//test
		//index to the last element
		size_t IndexLast = m_vecUISpriteComponentPtr.size() - 1;
		std::swap(m_vecUISpriteComponentPtr[IndexID], m_vecUISpriteComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecUISpriteComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void UIRenderSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPEUISPRITECOMPONENT);
	}

	void UIRenderSystem::m_Update(const std::string& scene)
	{
		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		if (m_vecUISpriteComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecUISpriteComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			UISpriteComponent* sprite = m_vecUISpriteComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((sprite->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			//ECS* ecs = ECS::m_GetInstance();

			if (assetmanager->m_imageManager.m_imageMap.find(sprite->m_imageFile) == assetmanager->m_imageManager.m_imageMap.end()) continue;

			unsigned int textureid = assetmanager->m_imageManager.m_imageMap.find(sprite->m_imageFile)->second.m_imageID;
			
			if (ecs->m_ECS_EntityMap[sprite->m_Entity].test(TYPEANIMATIONCOMPONENT)) 
			{
				AnimationComponent* animation = (AnimationComponent*)ecs->m_ECS_CombinedComponentPool[TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(sprite->m_Entity);

				//UI ALWAYS NOT ILLUMINATED

				graphicsPipe->m_unlitModelData.push_back({ glm::mat3{transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
																transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
															transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22} ,
															textureid,animation->m_stripCount, animation->m_frameNumber ,sprite->m_layer, glm::vec4{sprite->m_color.m_x,sprite->m_color.m_y, sprite->m_color.m_z, sprite->m_alpha} });
				
			}
			else
			{
				graphicsPipe->m_unlitModelData.push_back({ glm::mat3{transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
																transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
															transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22},textureid, 1, 0 ,sprite->m_layer
															, glm::vec4{sprite->m_color.m_x,sprite->m_color.m_y, sprite->m_color.m_z, sprite->m_alpha} });
				
			}
			
		}


	}


}

