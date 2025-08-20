/******************************************************************/
/*!
\file      RenderDebugSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
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

#include "RenderDebugSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"
#include "../ECS/Hierachy.h"

namespace ecs {

	void DebugDrawingSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void DebugDrawingSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecColliderComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecColliderComponentPtr.size() - 1;

		std::swap(m_vecColliderComponentPtr[IndexID], m_vecColliderComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void DebugDrawingSystem::m_Init() {

		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void DebugDrawingSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecTransformComponentPtr.size() != m_vecColliderComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}
	

		graphicpipe::GraphicsPipe * graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {

			//TransformComponent* transform = m_vecTransformComponentPtr[n];
			ColliderComponent* collider = m_vecColliderComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((collider->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;


			mat3x3::Mat3x3 translateMatrix;
			mat3x3::Mat3x3 translateBackMatrix;
			mat3x3::Mat3x3 translateToOriginMatrix;
			mat3x3::Mat3x3 scaleMatrix;
			mat3x3::Mat3x3 rotateMatrix;

			if (collider->m_drawDebug) {
				graphicsPipe->m_debugBoxData.push_back({ glm::mat3{collider->m_collider_Transformation.m_e00,collider->m_collider_Transformation.m_e01,collider->m_collider_Transformation.m_e02,
																collider->m_collider_Transformation.m_e10,collider->m_collider_Transformation.m_e11, collider->m_collider_Transformation.m_e12,
															collider->m_collider_Transformation.m_e20, collider->m_collider_Transformation.m_e21, collider->m_collider_Transformation.m_e22} ,
														collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
			}

			/*if (transform->m_haveParent) {
				EntityID parentID = ecs::Hierachy::m_GetParent(transform->m_Entity).value();
				TransformComponent* parentComp{ nullptr };
				ColliderComponent* parentCollider{ nullptr };
				for (auto& com : m_vecTransformComponentPtr) {
					if (com->m_Entity == parentID) {
						parentComp = com;
					}
				}
				for (auto& com : m_vecColliderComponentPtr) {
					if (com->m_Entity == parentID) {
						parentCollider = com;
					}
				}

				if (!parentComp) continue;
				if (!parentCollider) continue;*/
				
				//if ((collider->m_type == physicspipe::EntityType::RECTANGLE)) {
				//	mat3x3::Mat3x3 child_Transform = mat3x3::Mat3Transform(transform->m_position, transform->m_scale, 0);

				//	child_Transform.m_e20 += parentComp->m_position.m_x;
				//	child_Transform.m_e21 += parentComp->m_position.m_y;

				//	mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
				//	mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
				//	mat3x3::Mat3Translate(translateToOriginMatrix, -parentComp->m_position.m_x, -parentComp->m_position.m_y);
				//	mat3x3::Mat3Translate(translateBackMatrix, parentComp->m_position.m_x, (parentComp->m_position.m_y));
				//	child_Transform = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * child_Transform;

				//	mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
				//	mat3x3::Mat3Translate(translateToOriginMatrix, -child_Transform.m_e20, -child_Transform.m_e21);
				//	mat3x3::Mat3Translate(translateBackMatrix, child_Transform.m_e20, child_Transform.m_e21);

				//	child_Transform = translateBackMatrix * rotateMatrix * translateToOriginMatrix * child_Transform;

				//	mat3x3::Mat3x3 final_Transform = child_Transform * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_Size.m_x , collider->m_Size.m_y }, 0);

				//	mat3x3::Mat3x3 debugTransformation = final_Transform;


				//	collider->m_collider_Transformation = debugTransformation;
				//}
				//else if ((collider->m_type == physicspipe::EntityType::CIRCLE)) {
				//	mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(transform->m_position, vector2::Vec2{ 1.f, 1.f }, 0);

				//	debugTransformation.m_e20 += parentComp->m_position.m_x;
				//	debugTransformation.m_e21 += parentComp->m_position.m_y;

				//	mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
				//	mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
				//	mat3x3::Mat3Translate(translateToOriginMatrix, -parentComp->m_position.m_x, -parentComp->m_position.m_y);
				//	mat3x3::Mat3Translate(translateBackMatrix, parentComp->m_position.m_x, (parentComp->m_position.m_y));
				//	debugTransformation = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * debugTransformation;


				//	mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
				//	mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
				//	mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

				//	debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;


				//	debugTransformation = debugTransformation * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_radius * 2.f / parentComp->m_scale.m_x, collider->m_radius * 2.f / parentComp->m_scale.m_y }, 0);
				//	collider->m_collider_Transformation = debugTransformation;
				//}

				//if (collider->m_drawDebug) {
				//	graphicsPipe->m_debugBoxData.push_back({ glm::mat3{collider->m_collider_Transformation.m_e00,collider->m_collider_Transformation.m_e01,collider->m_collider_Transformation.m_e02,
				//													collider->m_collider_Transformation.m_e10,collider->m_collider_Transformation.m_e11, collider->m_collider_Transformation.m_e12,
				//												collider->m_collider_Transformation.m_e20, collider->m_collider_Transformation.m_e21, collider->m_collider_Transformation.m_e22} ,
				//											collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				//}

				/*if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::RECTANGLE))
				{
					
					mat3x3::Mat3x3 child_Transform = mat3x3::Mat3Transform(transform->m_position, transform->m_scale , 0);

					child_Transform.m_e20 += parentComp->m_position.m_x;
					child_Transform.m_e21 += parentComp->m_position.m_y;

					mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
					mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -parentComp->m_position.m_x, -parentComp->m_position.m_y);
					mat3x3::Mat3Translate(translateBackMatrix, parentComp->m_position.m_x, (parentComp->m_position.m_y));
					child_Transform = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * child_Transform;

					mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -child_Transform.m_e20, -child_Transform.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, child_Transform.m_e20, child_Transform.m_e21);

					child_Transform = translateBackMatrix * rotateMatrix * translateToOriginMatrix * child_Transform;

					mat3x3::Mat3x3 final_Transform = child_Transform * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_Size.m_x , collider->m_Size.m_y }, 0);

					mat3x3::Mat3x3 debugTransformation = final_Transform;

					
					collider->m_collider_Transformation = debugTransformation;
					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}
				else if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::CIRCLE))
				{
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform( transform->m_position , vector2::Vec2{ 1.f, 1.f }, 0);

					debugTransformation.m_e20 += parentComp->m_position.m_x;
					debugTransformation.m_e21 += parentComp->m_position.m_y;

					mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
					mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -parentComp->m_position.m_x, -parentComp->m_position.m_y);
					mat3x3::Mat3Translate(translateBackMatrix, parentComp->m_position.m_x, (parentComp->m_position.m_y));
					debugTransformation = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * debugTransformation;


					mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

					debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;
					

					debugTransformation = debugTransformation * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_radius * 2.f / parentComp->m_scale.m_x, collider->m_radius * 2.f / parentComp->m_scale.m_y }, 0);
					collider->m_collider_Transformation = debugTransformation;

					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}*/
			//}
			//else {

				//if (collider->m_type == physicspipe::EntityType::RECTANGLE) {
				//	mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20 , transform->m_transformation.m_e21 }, transform->m_scale, 0);

				//	mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
				//	mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
				//	mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

				//	debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;

				//	debugTransformation = debugTransformation * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_Size.m_x , collider->m_Size.m_y }, 0);


				//	collider->m_collider_Transformation = debugTransformation;
				//}
				//else if (collider->m_type == physicspipe::EntityType::CIRCLE) {
				//	mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(transform->m_position, vector2::Vec2{ 1.f, 1.f }, 0);

				//	mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
				//	mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
				//	mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

				//	debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;


				//	debugTransformation = debugTransformation * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_radius * 2.f, collider->m_radius * 2.f }, 0);
				//	collider->m_collider_Transformation = debugTransformation;
				//}


				//if (collider->m_drawDebug) {
				//	graphicsPipe->m_debugBoxData.push_back({ glm::mat3{collider->m_collider_Transformation.m_e00,collider->m_collider_Transformation.m_e01,collider->m_collider_Transformation.m_e02,
				//													collider->m_collider_Transformation.m_e10,collider->m_collider_Transformation.m_e11, collider->m_collider_Transformation.m_e12,
				//												collider->m_collider_Transformation.m_e20, collider->m_collider_Transformation.m_e21, collider->m_collider_Transformation.m_e22} ,
				//											collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				//}

				/*if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::RECTANGLE))
				{

					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20 , transform->m_transformation.m_e21 }, transform->m_scale, 0);

					mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

					debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;

					debugTransformation = debugTransformation * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_Size.m_x , collider->m_Size.m_y }, 0);

					
					collider->m_collider_Transformation = debugTransformation;
					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}
				else if (collider->m_drawDebug && (collider->m_type == physicspipe::EntityType::CIRCLE))
				{
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(transform->m_position, vector2::Vec2{ 1.f, 1.f }, 0);

					mat3x3::Mat3RotDeg(rotateMatrix, transform->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

					debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;


					debugTransformation = debugTransformation * mat3x3::Mat3Transform(collider->m_OffSet, vector2::Vec2{ collider->m_radius * 2.f, collider->m_radius * 2.f}, 0);
					collider->m_collider_Transformation = debugTransformation;

					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, static_cast<graphicpipe::GraphicsPipe::ShapeType>(collider->m_type) });
				}*/
			//}

			
		}
	}


}
