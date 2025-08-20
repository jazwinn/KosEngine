/******************************************************************/
/*!
\file      TransformSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Sept 29, 2024
\brief   



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "../ECS.h"

#include "TransformSystem.h"
#include "../ECS/Hierachy.h"
#include "../Graphics/GraphicsCamera.h"


namespace ecs {

	void TransformSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void TransformSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecTransformComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecTransformComponentPtr.size() - 1;

		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);
		//popback the vector;
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void TransformSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		//SystemSignature.set();

	}

	void TransformSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();


		for (int n{}; n < m_vecTransformComponentPtr.size(); n++)
		{
			TransformComponent* transformComp = m_vecTransformComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];

			//skip component not of the scene
			if ((transformComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			transformComp->m_transformation = mat3x3::Mat3Transform(transformComp->m_position, transformComp->m_scale, transformComp->m_rotation);
			vector2::Vec2 pos{}, scale_s{};
			float rot{};
			mat3x3::Mat3Decompose(transformComp->m_transformation, pos, scale_s, rot);
			//std::cout << rot << std::endl;
			//std::cout << scale_s.m_x << " " << scale_s.m_y << std::endl;
			if (!transformComp->m_haveParent) {
				continue;
			}
			//get parents coordinate
			if (!ecs::Hierachy::m_GetParent(transformComp->m_Entity).has_value()) {
				// no parnet
				return;
			}

			EntityID parentID = ecs::Hierachy::m_GetParent(transformComp->m_Entity).value();
			TransformComponent* parentComp{nullptr};
			for (auto& com : m_vecTransformComponentPtr) {
				if (com->m_Entity == parentID) {
					parentComp = com;
				}
			}
			if (!parentComp) continue;


			mat3x3::Mat3x3 parentTransformation = parentComp->m_transformation;
			
			vector2::Vec2 translate;
			vector2::Vec2 scale;
			float rotate;
			mat3x3::Mat3Decompose(parentTransformation, translate, scale, rotate);
			mat3x3::Mat3x3 translateMatrix;
			mat3x3::Mat3x3 translateBackMatrix;
			mat3x3::Mat3x3 translateToOriginMatrix;
			mat3x3::Mat3x3 scaleMatrix;
			mat3x3::Mat3x3 rotateMatrix;

			transformComp->m_localChildTransformation = mat3x3::Mat3Transform(transformComp->m_position, vector2::Vec2{ transformComp->m_scale.m_x, transformComp->m_scale.m_y }, transformComp->m_rotation);

			transformComp->m_transformation = mat3x3::Mat3Transform(translate, vector2::Vec2{ transformComp->m_scale.m_x, transformComp->m_scale.m_y }, 0);

			//Set Child Position to Follow Parent
			transformComp->m_transformation.m_e20 += transformComp->m_position.m_x;
			transformComp->m_transformation.m_e21 += transformComp->m_position.m_y;

			
			mat3x3::Mat3Scale(scaleMatrix, scale.m_x, scale.m_y);
			mat3x3::Mat3RotDeg(rotateMatrix, rotate);
			mat3x3::Mat3Translate(translateToOriginMatrix, - translate.m_x,  - translate.m_y);
			mat3x3::Mat3Translate(translateBackMatrix, translate.m_x, (translate.m_y));
			transformComp->m_transformation = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * transformComp->m_transformation;

			mat3x3::Mat3RotDeg(rotateMatrix, transformComp->m_rotation);
			mat3x3::Mat3Scale(scaleMatrix, transformComp->m_scale.m_x, transformComp->m_scale.m_y);
			mat3x3::Mat3Translate(translateToOriginMatrix, -transformComp->m_transformation.m_e20, -transformComp->m_transformation.m_e21);
			mat3x3::Mat3Translate(translateBackMatrix, transformComp->m_transformation.m_e20, transformComp->m_transformation.m_e21);

			transformComp->m_transformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * transformComp->m_transformation;


			//transformComp->m_transformation = parentTransformation * transformComp->m_transformation;
		}

	}


}
