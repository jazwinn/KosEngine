/******************************************************************/
/*!
\file      RenderDebugSystem.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the RenderDebugSystem
		   class. It passes the debauging data into the graphics
		   pipeline for it to be rendered.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "../ECS.h"

#include "CameraSystem.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"


namespace ecs {

	void CameraSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecCameraComponentPtr.begin(), m_vecCameraComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecCameraComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecCameraComponentPtr.push_back((CameraComponent*)ecs->m_ECS_CombinedComponentPool[TYPECAMERACOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void CameraSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecCameraComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecCameraComponentPtr.size() - 1;

		std::swap(m_vecCameraComponentPtr[IndexID], m_vecCameraComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecCameraComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void CameraSystem::m_Init() {

		m_SystemSignature.set(TYPECAMERACOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void CameraSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecTransformComponentPtr.size() != m_vecCameraComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}


		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((transform->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			//CameraComponent* cam = m_vecCameraComponentPtr[n];

			mat3x3::Mat3x3 cameraTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20, transform->m_transformation.m_e21 }, vector2::Vec2{ transform->m_scale.m_x, transform->m_scale.m_y }, transform->m_rotation);

			graphicpipe::GraphicsCamera::m_cameras.push_back({ glm::mat3{cameraTransformation.m_e00 ,cameraTransformation.m_e01,cameraTransformation.m_e02,
															cameraTransformation.m_e10,cameraTransformation.m_e11 , cameraTransformation.m_e12,
														cameraTransformation.m_e20, cameraTransformation.m_e21, cameraTransformation.m_e22}});
			graphicpipe::GraphicsCamera::m_currCameraRotate = transform->m_rotation;
			graphicpipe::GraphicsCamera::m_currCameraScaleX = transform->m_scale.m_x;
			graphicpipe::GraphicsCamera::m_currCameraScaleY = transform->m_scale.m_y;
			graphicpipe::GraphicsCamera::m_currCameraTranslateX = transform->m_transformation.m_e20;
			graphicpipe::GraphicsCamera::m_currCameraTranslateY = transform->m_transformation.m_e21;

			mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20, transform->m_transformation.m_e21 }, vector2::Vec2{ transform->m_scale.m_x * 2 * (1.f / graphicpipe::GraphicsCamera::m_aspectRatio), transform->m_scale.m_y * 2 }, transform->m_rotation);
			//change camera debug box tobe of different colour
			graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
															debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
														debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
													2.0f, graphicpipe::GraphicsPipe::SQUARE_LINES });

			

		}
	}


}
