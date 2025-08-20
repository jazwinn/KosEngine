/******************************************************************/
/*!
\file      RenderTextSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the RenderTextSystem
		   class. It passes the text data into the graphics
		   pipeline for it to be rendered.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "../ECS.h"

#include "RenderTextSystem.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Helper/Helper.h"

namespace ecs {

	void RenderTextSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTextComponentPtr.begin(), m_vecTextComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTextComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecTextComponentPtr.push_back((TextComponent*)ecs->m_ECS_CombinedComponentPool[TYPETEXTCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void RenderTextSystem::m_DeregisterSystem(EntityID ID) {


		//search element location for the entity
		size_t IndexID{};
		for (auto& SpriteComponentPtr : m_vecTextComponentPtr) {
			if (SpriteComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecTextComponentPtr.size() - 1;
		std::swap(m_vecTextComponentPtr[IndexID], m_vecTextComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecTextComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void RenderTextSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPETEXTCOMPONENT);
	}

	void RenderTextSystem::m_Update(const std::string& scene)
	{
		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		//Helper::Helpers* helper = Helper::Helpers::GetInstance();
		Helper::Helpers* help =  Helper::Helpers::GetInstance();
		if (m_vecTextComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not match");
			return;
		}
		//loops through all vecoters pointing to component

		//assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		for (int n{}; n < m_vecTextComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			TextComponent* text = m_vecTextComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];

			//skip component not of the scene
			if ((NameComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			float red = text->m_color.m_x;
			float green = text->m_color.m_y;
			float blue = text->m_color.m_z;

			vector2::Vec2 translate{};
			vector2::Vec2 scale{};
			float rotate{};
			mat3x3::Mat3Decompose(transform->m_transformation, translate, scale, rotate);
			graphicsPipe->m_textData.push_back(graphicpipe::TextData{ text->m_text, transform->m_transformation.m_e20, transform->m_transformation.m_e21, rotate, text->m_fontSize * help->m_windowScalar , {scale.m_x , scale.m_y}, text->m_fontLayer, {red, green, blue}, text->m_fileName, text->m_isCentered});
		

		}


	}


}

