/******************************************************************/
/*!
\file      ButtonSystem.cpp
\author    Rayner and Sean
\par       
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

#include "ButtonSystem.h"
#include "../Inputs/Input.h"
#include "../Helper/Helper.h"
#include "../Graphics/GraphicsCamera.h"
#include "../Graphics/GraphicsPipe.h"


namespace ecs {
	void ButtonSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecButtonComponentPtr.begin(), m_vecButtonComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecButtonComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecButtonComponentPtr.push_back((ButtonComponent*)ecs->m_ECS_CombinedComponentPool[TYPEBUTTONCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void ButtonSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecButtonComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
 			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecButtonComponentPtr.size() - 1;

		std::swap(m_vecButtonComponentPtr[IndexID], m_vecButtonComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);
		//popback the vector;
		m_vecButtonComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void ButtonSystem::m_Init() {
		m_SystemSignature.set(TYPEBUTTONCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
	}

	void ButtonSystem::m_Update(const std::string& scene) {
		//ECS* ecs = ECS::m_GetInstance();
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		if (m_vecTransformComponentPtr.size() != m_vecButtonComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//update box
		float mouseX = Input::InputSystem::MousePosition.m_x; //Screen Coordinates
		float mouseY = Input::InputSystem::MousePosition.m_y; //Screen Coordinates
		float minX, maxX, minY, maxY;
		ECS* ecs = ECS::m_GetInstance();

		for (int i = 0; i < m_vecButtonComponentPtr.size(); ++i) {
			ButtonComponent* button = m_vecButtonComponentPtr[i];
			TransformComponent* transform = m_vecTransformComponentPtr[i];
			NameComponent* NameComp = m_vecNameComponentPtr[i];

			//reset button
			button->m_IsClick = false;
			button->m_IsHover = false;

			if ((transform->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			bool clicked = (Input::InputSystem::m_isKeyTriggered(keys::LMB) || Input::InputSystem::m_isKeyPressed(keys::LMB));

			if (clicked || button->m_EnableHover) {


				float rotate = -graphicpipe::GraphicsCamera::m_currCameraRotate * (3.14151f / 180.f);

				float windowCordinateX = ((transform->m_transformation.m_e20 - graphicpipe::GraphicsCamera::m_currCameraMatrix[2][0] + graphicpipe::GraphicsCamera::m_currCameraScaleX * (1.f/graphicpipe::GraphicsCamera::m_aspectRatio)) / (graphicpipe::GraphicsCamera::m_currCameraScaleX * ((1.f / graphicpipe::GraphicsCamera::m_aspectRatio) * 2))) * help->m_windowWidth;
				float windowCordinateY = ((transform->m_transformation.m_e21 - graphicpipe::GraphicsCamera::m_currCameraMatrix[2][1] + graphicpipe::GraphicsCamera::m_currCameraScaleY) / (graphicpipe::GraphicsCamera::m_currCameraScaleY * (1.f * 2)))* help->m_windowHeight;
				
				button->m_Position = { windowCordinateX , windowCordinateY };

				float translateX = windowCordinateX - (help->m_windowWidth/2);
				float translateY = windowCordinateY - (help->m_windowHeight/2);

				float newTranslateX = translateX * cos(rotate) - translateY * sin(rotate);
				float newTranslateY = translateX * sin(rotate) + translateY * cos(rotate);

				translateX = newTranslateX + help->m_windowWidth/2.f;
				translateY = newTranslateY + help->m_windowHeight/2.f;

				button->m_Position = { translateX , translateY };

				//button->m_Scale = { 360,360 };
				vector2::Vec2 pixelBox = { (graphicpipe::GraphicsCamera::m_windowHeight / 2 * button->m_Scale.m_x) / graphicpipe::GraphicsCamera::m_currCameraScaleX , (graphicpipe::GraphicsCamera::m_windowHeight / 2 * button->m_Scale.m_y) / graphicpipe::GraphicsCamera::m_currCameraScaleY };
				//std::cout << "Scale " << graphicpipe::GraphicsCamera::m_currCameraScaleX << std::endl;

				minX = button->m_Position.m_x - (pixelBox.m_x / 2.f) + help->m_viewportOffsetX;
				maxX = button->m_Position.m_x + (pixelBox.m_x / 2.f) + help->m_viewportOffsetX;
				minY = button->m_Position.m_y - (pixelBox.m_y / 2.f) + help->m_viewportOffsetY;
				maxY = button->m_Position.m_y + (pixelBox.m_y / 2.f) + help->m_viewportOffsetY;
				/*std::cout << "Mouse position ";
				std::cout << mouseX << " " << mouseY << std::endl;
				std::cout << "Window position ";
				std::cout << help->m_windowWidth << " " << help->m_windowHeight << std::endl;
				std::cout << "object position ";
				std::cout << transform->m_position.m_x << " " << transform->m_position.m_y << std::endl;
				std::cout << "button position ";
		 		std::cout << button->m_Position.m_x << " " << button->m_Position.m_y << std::endl;
				std::cout << minX << " " << minY << " " << maxX << " " << maxY << std::endl;*/
				if ((minX <= mouseX && mouseX <= maxX) && (minY <= mouseY && mouseY <= maxY)) {
					if (clicked) {
						button->m_IsClick = true;
						LOGGING_INFO("Button Clicked");
					}
					if (button->m_EnableHover) {
						button->m_IsHover = true;
					}

					
					//std::cout << " clicked " << std::endl;
				}
			}
		}

		for (int i = 0; i < m_vecButtonComponentPtr.size(); ++i) {
			ButtonComponent* button = m_vecButtonComponentPtr[i];
			TransformComponent* transform = m_vecTransformComponentPtr[i];

			//skip component not of the scene
			if (button->m_scene != scene) continue;


			mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20, transform->m_transformation.m_e21 }, vector2::Vec2{ button->m_Scale.m_x, button->m_Scale.m_y }, 0);

			graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
			pipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
															debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
														3.f, 0 });

			
		}
	}
}


 