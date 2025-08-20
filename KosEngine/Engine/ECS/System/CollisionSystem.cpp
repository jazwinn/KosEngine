/******************************************************************/
/*!
\file      CollisionSystem.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 29, 2024
\brief     This file contains the definition of the CollisionSystem class,
		   which is part of the Entity Component System (ECS) framework.

		   The CollisionSystem class is responsible for managing and
		   processing collisions between entities that have both
		   Transform and Collider components. It registers and
		   deregisters entities with collision-related components and
		   updates the system to handle collisions during the game loop.
		   
		   The class extends the ISystem interface, ensuring it adheres
		   to the system management structure within the ECS framework.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "../ECS.h"

#include "CollisionSystem.h"
#include "../ECS/Hierachy.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Debugging/Logging.h"

namespace ecs {

	void CollisionSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
			
		}
	}

	void CollisionSystem::m_DeregisterSystem(EntityID ID) {
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

	void CollisionSystem::m_Init() {

		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);

	}

	void CollisionSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecColliderComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			//std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}
		
		//TODO find better way to fill up data
		for (int n{}; n < m_vecColliderComponentPtr.size(); n++)
		{
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			ColComp->m_isCollided = false;
		}

		//create physics;
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();
		PhysicsPipeline->m_ClearEntites();
		PhysicsPipeline->m_ClearPair();

		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";
			
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			TransformComponent* TransComp = m_vecTransformComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			EntityID id = ColComp->m_Entity;

			//skip component not of the scene
			if ((ColComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;
			


			//if movement component is present, do dynamic collision
			vector2::Vec2 velocity{};
			vector2::Vec2 prevPos{};
			if (ecs->m_ECS_EntityMap[id].test(TYPERIGIDBODYCOMPONENT)) {

				RigidBodyComponent* rigidComp = static_cast<RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(id));

				 velocity = rigidComp->m_Velocity;
				 prevPos = rigidComp->m_PrevPos;

			}

			vector2::Vec2 position{ TransComp->m_transformation.m_e20,TransComp->m_transformation.m_e21 };

			mat3x3::Mat3x3 translateMatrix;
			mat3x3::Mat3x3 translateBackMatrix;
			mat3x3::Mat3x3 translateToOriginMatrix;
			mat3x3::Mat3x3 scaleMatrix;
			mat3x3::Mat3x3 rotateMatrix;
			vector2::Vec2 pos{}, scale{};
			float rot{};

			//If Entity Has A Parent
			if (TransComp->m_haveParent) {
				EntityID parentID = ecs::Hierachy::m_GetParent(TransComp->m_Entity).value();
				TransformComponent* parentComp{ static_cast<TransformComponent*>(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parentID)) };

				if (!parentComp) {
					continue;
				}
				if (!parentComp) continue;
							
				if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(TransComp->m_position, vector2::Vec2{ 1.f, 1.f }, 0);

					debugTransformation.m_e20 += parentComp->m_position.m_x;
					debugTransformation.m_e21 += parentComp->m_position.m_y;

					mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
					mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -parentComp->m_position.m_x, -parentComp->m_position.m_y);
					mat3x3::Mat3Translate(translateBackMatrix, parentComp->m_position.m_x, (parentComp->m_position.m_y));
					debugTransformation = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * debugTransformation;


					mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

					debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;


					debugTransformation = debugTransformation * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_radius * 2.f / parentComp->m_scale.m_x, ColComp->m_radius * 2.f / parentComp->m_scale.m_y }, 0);
					ColComp->m_collider_Transformation = debugTransformation;
					
					mat3x3::Mat3Decompose(ColComp->m_collider_Transformation, pos, scale, rot);


					//dont pass data if collision check is false
					if (!ColComp->m_collisionCheck) continue;
					PhysicsPipeline->m_SendPhysicsData(ColComp->m_radius, pos,prevPos, scale, velocity, id, NameComp->m_Layer);
				}
				else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {

					//get parent scale, rotate translate
					vector2::Vec2 parentScale, parentTranslate;
					float parentRotate;
					mat3x3::Mat3Decompose(parentComp->m_transformation, parentTranslate, parentScale, parentRotate);

					mat3x3::Mat3x3 child_Transform = mat3x3::Mat3Transform(TransComp->m_position, TransComp->m_scale, 0);

					child_Transform.m_e20 += parentTranslate.m_x;
					child_Transform.m_e21 += parentTranslate.m_y;

					mat3x3::Mat3Scale(scaleMatrix, parentScale.m_x, parentScale.m_y);
					mat3x3::Mat3RotDeg(rotateMatrix, parentRotate);
					mat3x3::Mat3Translate(translateToOriginMatrix, -parentTranslate.m_x, -parentTranslate.m_y);
					mat3x3::Mat3Translate(translateBackMatrix, parentTranslate.m_x, (parentTranslate.m_y));
					child_Transform = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * child_Transform;

					mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -child_Transform.m_e20, -child_Transform.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, child_Transform.m_e20, child_Transform.m_e21);

					child_Transform = translateBackMatrix * rotateMatrix * translateToOriginMatrix * child_Transform;

					mat3x3::Mat3x3 final_Transform = child_Transform * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_Size.m_x , ColComp->m_Size.m_y }, 0);

					mat3x3::Mat3x3 debugTransformation = final_Transform;


					ColComp->m_collider_Transformation = debugTransformation;

					//dont pass data if collision check is false
					if (!ColComp->m_collisionCheck) continue;
					mat3x3::Mat3Decompose(ColComp->m_collider_Transformation, pos, scale, rot);
					PhysicsPipeline->m_SendPhysicsData(scale.m_y, scale.m_x, rot, pos,  prevPos , scale, velocity, id, NameComp->m_Layer);
				}
				else {
					LOGGING_ERROR("NO ENTITY TYPE");
				}
			}
			//If Entity Doesnt Have A Parent
			else {
				if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {

					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(TransComp->m_position, vector2::Vec2{ 1.f, 1.f }, 0);

					mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

					debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;


					debugTransformation = debugTransformation * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_radius * 2.f, ColComp->m_radius * 2.f }, 0);
					ColComp->m_collider_Transformation = debugTransformation;


					//dont pass data if collision check is false
					if (!ColComp->m_collisionCheck) continue;
					mat3x3::Mat3Decompose(ColComp->m_collider_Transformation, pos, scale, rot);
					PhysicsPipeline->m_SendPhysicsData(ColComp->m_radius, pos,prevPos, scale, velocity, id, NameComp->m_Layer);
				}
				else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ TransComp->m_transformation.m_e20 , TransComp->m_transformation.m_e21 }, TransComp->m_scale, 0);

					mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
					mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
					mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

					debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;

					debugTransformation = debugTransformation * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_Size.m_x , ColComp->m_Size.m_y }, 0);


					ColComp->m_collider_Transformation = debugTransformation;

					//dont pass data if collision check is false
					if (!ColComp->m_collisionCheck) continue;
					mat3x3::Mat3Decompose(ColComp->m_collider_Transformation, pos, scale, rot);
					PhysicsPipeline->m_SendPhysicsData(scale.m_y, scale.m_x, rot, pos,  prevPos , scale, velocity, id, NameComp->m_Layer);
				}
				else {
					LOGGING_ERROR("NO ENTITY TYPE");
				}
			}
		}
	}
		

}
