/******************************************************************/
/*!
\file      RenderSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Jan 30, 2025
\brief     This file contains class for the Render System

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"

#include "../ECS.h"
#include "../ECS/Hierachy.h"
#include "../ECS/System/LightingSystem.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"

namespace ecs {

	void LightingSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecLightingComponentPtr.push_back((LightingComponent*)ecs->m_ECS_CombinedComponentPool[TYPELIGHTINGCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void LightingSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& LightingComponentPtr : m_vecLightingComponentPtr) {
			if (LightingComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecLightingComponentPtr.size() - 1;
		std::swap(m_vecLightingComponentPtr[IndexID], m_vecLightingComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecLightingComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void LightingSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPELIGHTINGCOMPONENT);
	}

	void LightingSystem::m_Update(const std::string& scene)
	{
		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();

		if (m_vecLightingComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecLightingComponentPtr.size(); n++) 
		{
			TransformComponent* transform = m_vecTransformComponentPtr[n];
			LightingComponent* light = m_vecLightingComponentPtr[n];
			NameComponent* nc = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((light->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(nc->m_Layer)) continue;

			//Clamp between 0.f and 1.f
			light->m_innerOuterRadius.m_x = light->m_innerOuterRadius.m_x > 1.f ? 1.f : light->m_innerOuterRadius.m_x < 0.f ? 0.f : light->m_innerOuterRadius.m_x;

			light->m_innerOuterRadius.m_y = light->m_innerOuterRadius.m_y > 1.f ? 1.f : light->m_innerOuterRadius.m_y < 0.f ? 0.f : light->m_innerOuterRadius.m_y;

			//TODO recalculate the entire thing
			vector2::Vec2 position = transform->m_position;
			vector2::Vec2 scale = transform->m_scale;
			float rotation = transform->m_rotation;


			//Light 
			vector2::Vec2 light_offset = light->m_light_OffSet;
			vector2::Vec2 light_scale = light->m_light_scale;
			float light_rotation = light->m_light_rotation;

			mat3x3::Mat3x3 posMatrix, scaleMatrix, rotMatrix, light_rotMatrix;
			vector2::Vec2 final_Pos = position + light_offset;
			vector2::Vec2 final_scale = scale * light_scale;

			mat3x3::Mat3x3 translateMatrix;
			mat3x3::Mat3x3 translateBackMatrix;
			mat3x3::Mat3x3 translateToOriginMatrix;
			mat3x3::Mat3x3 rotateMatrix;

			//Matrix
			mat3x3::Mat3Scale(scaleMatrix, final_scale.m_x, final_scale.m_y);
			mat3x3::Mat3RotDeg(rotateMatrix, rotation);
			mat3x3::Mat3RotDeg(light_rotMatrix, light_rotation);
			mat3x3::Mat3Translate(posMatrix, final_Pos.m_x, final_Pos.m_y);

			//transform->m_transformation = final_Matrix;
			if (transform->m_haveParent) {
				EntityID parentID = ecs::Hierachy::m_GetParent(transform->m_Entity).value();
				TransformComponent* parentComp{ static_cast<TransformComponent*>(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parentID)) };

				if (!parentComp) {
					continue;
				}

				vector2::Vec2 parentScale, parentTranslate;
				float parentRotate;
				mat3x3::Mat3Decompose(parentComp->m_transformation, parentTranslate, parentScale, parentRotate);

				mat3x3::Mat3x3 lightTransformation = mat3x3::Mat3Transform(transform->m_position, transform->m_scale, 0);
				
				lightTransformation.m_e20 += parentTranslate.m_x;
				lightTransformation.m_e21 += parentTranslate.m_y;

				mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
				mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
				mat3x3::Mat3Translate(translateToOriginMatrix, -parentTranslate.m_x, -parentTranslate.m_y);
				mat3x3::Mat3Translate(translateBackMatrix, parentTranslate.m_x, parentTranslate.m_y);
				lightTransformation = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * lightTransformation;

				mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
				mat3x3::Mat3Translate(translateToOriginMatrix, -lightTransformation.m_e20, -lightTransformation.m_e21);
				mat3x3::Mat3Translate(translateBackMatrix, lightTransformation.m_e20, lightTransformation.m_e21);

				lightTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * lightTransformation;

				/*mat3x3::Mat3RotDeg(rotateMatrix, light->m_light_rotation);
				mat3x3::Mat3Translate(translateToOriginMatrix, -lightTransformation.m_e20, -lightTransformation.m_e21);
				mat3x3::Mat3Translate(translateBackMatrix, lightTransformation.m_e20, lightTransformation.m_e21);*/

				//lightTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * lightTransformation;

				lightTransformation = lightTransformation * mat3x3::Mat3Transform(light->m_light_OffSet, light->m_light_scale, light->m_light_rotation);



				//transform->m_transformation = lightTransformation;

				if (light->m_lightType == graphicpipe::GLOW)
				{
					graphicsPipe->m_additiveLightingData.push_back({ { lightTransformation.m_e00,lightTransformation.m_e01,lightTransformation.m_e02,
										   lightTransformation.m_e10,lightTransformation.m_e11, lightTransformation.m_e12,
										   lightTransformation.m_e20, lightTransformation.m_e21, lightTransformation.m_e22 },
										{  light->m_colour.m_x,light->m_colour.m_y,light->m_colour.m_z ,1.f }, { light->m_innerOuterRadius.m_x ,light->m_innerOuterRadius.m_y },
										   light->m_intensity });
				}
				else
				{
					graphicsPipe->m_multiLightingData.push_back({ { lightTransformation.m_e00,lightTransformation.m_e01,lightTransformation.m_e02,
															   lightTransformation.m_e10,lightTransformation.m_e11, lightTransformation.m_e12,
															   lightTransformation.m_e20, lightTransformation.m_e21, lightTransformation.m_e22 },
															   { light->m_colour.m_x,light->m_colour.m_y,light->m_colour.m_z ,1.f }, { light->m_innerOuterRadius.m_x ,light->m_innerOuterRadius.m_y },
															   light->m_intensity });
				}
				

			}
			else {

				mat3x3::Mat3x3 lightTransformation = mat3x3::Mat3Transform(transform->m_position, transform->m_scale, 0);

				mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
				mat3x3::Mat3Translate(translateToOriginMatrix, -lightTransformation.m_e20, -lightTransformation.m_e21);
				mat3x3::Mat3Translate(translateBackMatrix, lightTransformation.m_e20, lightTransformation.m_e21);

				lightTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * lightTransformation;
				
				mat3x3::Mat3RotDeg(rotateMatrix, light->m_light_rotation);
				mat3x3::Mat3Translate(translateToOriginMatrix, -lightTransformation.m_e20, -lightTransformation.m_e21);
				mat3x3::Mat3Translate(translateBackMatrix, lightTransformation.m_e20, lightTransformation.m_e21);


				lightTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * lightTransformation;

				lightTransformation = lightTransformation * mat3x3::Mat3Transform(light->m_light_OffSet, light->m_light_scale, 0);

				if (light->m_lightType == graphicpipe::GLOW)
				{
					graphicsPipe->m_additiveLightingData.push_back({ { lightTransformation.m_e00,lightTransformation.m_e01,lightTransformation.m_e02,
															   lightTransformation.m_e10,lightTransformation.m_e11, lightTransformation.m_e12,
															   lightTransformation.m_e20, lightTransformation.m_e21, lightTransformation.m_e22 },
															   { light->m_colour.m_x,light->m_colour.m_y,light->m_colour.m_z ,1.f }, { light->m_innerOuterRadius.m_x ,light->m_innerOuterRadius.m_y },
															   light->m_intensity });
				}
				else
				{
					graphicsPipe->m_multiLightingData.push_back({ { lightTransformation.m_e00,lightTransformation.m_e01,lightTransformation.m_e02,
															   lightTransformation.m_e10,lightTransformation.m_e11, lightTransformation.m_e12,
															   lightTransformation.m_e20, lightTransformation.m_e21, lightTransformation.m_e22 },
															   { light->m_colour.m_x,light->m_colour.m_y,light->m_colour.m_z ,1.f }, { light->m_innerOuterRadius.m_x ,light->m_innerOuterRadius.m_y },
															   light->m_intensity });
				}

				
			}

			//ORIGINAL
			//graphicsPipe->m_multiLightingData.push_back({ { transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
			//										   transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
			//										   transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22 },
			//										{  light->m_colour.m_x,light->m_colour.m_y,light->m_colour.m_z ,1.f }, { light->m_innerOuterRadius.m_x ,light->m_innerOuterRadius.m_y },
			//										   light->m_intensity, light->m_lightType });
		

		}


	}


}

