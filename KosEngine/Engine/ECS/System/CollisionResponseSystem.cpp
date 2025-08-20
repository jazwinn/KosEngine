/******************************************************************/
/*!
\file      CollisionResponseSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\co-author Elijah Teo (teo.e, 2301530), Rayner Tan(raynerweichen.tan, 2301449)
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination for the collision
		   respoonse system. It retrieves for collision data from
		   the physics pipeline and reach accordingly.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "../ECS.h"

#include "CollisionResponseSystem.h"
#include "../Physics/Physics.h"
#include "../ECS/Hierachy.h"
#include "../Physics/PhysicsCollisionResponse.h"
#include "../Graphics/GraphicsPipe.h"

#include "../Helper/Helper.h"

namespace ecs {

	void CollisionResponseSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecRigidBodyComponentPtr.begin(), m_vecRigidBodyComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecRigidBodyComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
			m_vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void CollisionResponseSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecRigidBodyComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecRigidBodyComponentPtr.size() - 1;

		std::swap(m_vecColliderComponentPtr[IndexID],  m_vecColliderComponentPtr[IndexLast]);
		std::swap(m_vecRigidBodyComponentPtr[IndexID], m_vecRigidBodyComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecRigidBodyComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void CollisionResponseSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		m_SystemSignature.set(TYPERIGIDBODYCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void CollisionResponseSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();
		//Helper::Helpers* help = Helper::Helpers::GetInstance();

		if (m_vecRigidBodyComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}

		//TODO instead of retrieving every loop, just get the address of the vector in the physics pipeline
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();

		//gets collided
		PhysicsPipeline->m_Update();
		
		std::vector <std::shared_ptr<physicspipe::PhysicsData>> vecCollisionEntity = PhysicsPipeline->m_RetrievePhysicsData();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();
		std::vector < std::pair < std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>, std::pair<vector2::Vec2, float>>> vecCollisionEntityPairWithVector = PhysicsPipeline->m_RetrievePhysicsDataPairWithVector();
		std::unordered_set<ecs::EntityID> ids;

		std::vector<ecs::EntityID> respondedids;

		for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++)
		{
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			RigidBodyComponent* rigidComp = m_vecRigidBodyComponentPtr[n];
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			TransformComponent* transform = m_vecTransformComponentPtr[n];


			respondedids.push_back(NameComp->m_Entity);

			ColComp->m_collidedWith.clear();
			ColComp->m_blockedFlag = -1;
			//skip component not of the scene
			if ((rigidComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;


			EntityID obj1_EntityID = rigidComp->m_Entity;

			std::vector<EntityID> colidedwith;

			while (true) {

				EntityID obj2_EntityID;

				bool swap = false;


				auto valit = std::find_if(vecCollisionEntityPairWithVector.begin(), vecCollisionEntityPairWithVector.end(), [&](const auto& x) { 
					
					if (static_cast<ecs::EntityID>(x.first.first.get()->m_ID) == obj1_EntityID) {
						if (std::find(colidedwith.begin(), colidedwith.end(), static_cast<ecs::EntityID>(x.first.second.get()->m_ID)) == colidedwith.end()) return true;
					}
					return false;
					
					});

				if (valit == vecCollisionEntityPairWithVector.end()) {

					valit = std::find_if(vecCollisionEntityPairWithVector.begin(), vecCollisionEntityPairWithVector.end(), [&](const auto& x) {

						if (static_cast<ecs::EntityID>(x.first.second.get()->m_ID) == obj1_EntityID) {
							if (std::find(colidedwith.begin(), colidedwith.end(), static_cast<ecs::EntityID>(x.first.first.get()->m_ID)) == colidedwith.end()) return true;
						}
						return false;

						});

					if (valit != vecCollisionEntityPairWithVector.end()) {
						obj2_EntityID = valit->first.first.get()->m_ID;
						swap = true;
					}
					else {
						break;
					}
				}
				else {
					obj2_EntityID = valit->first.second.get()->m_ID;
				}


				colidedwith.push_back(obj2_EntityID);

				if (std::find(ColComp->m_collidedWith.begin(), ColComp->m_collidedWith.end(), static_cast<ecs::EntityID>(obj2_EntityID)) == ColComp->m_collidedWith.end()) {
					ColComp->m_collidedWith.push_back(obj2_EntityID);
				}
				

				//if (std::find(respondedids.begin(), respondedids.end(), obj2_EntityID) != respondedids.end()) continue;




				//TransformComponent* obj2_TC = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(obj2_EntityID);
				ColliderComponent* obj2_CC = (ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(obj2_EntityID);
				//RigidBodyComponent* obj2_RC = (RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(obj2_EntityID);

				//obj2_CC->m_isCollided = 1.0f;
				ColComp->m_isCollided = 1.f;

				/*if (ColComp->m_collisionResponse == false && obj2_CC->m_collisionResponse == false)
					continue;*/


				if (ColComp->m_collisionResponse == false)
					continue;

				if (obj2_CC->m_collisionResponse == false)
					continue;

				if (ColComp->m_collisionResponse) {


					rigidComp->m_Velocity = {0.f,0.f};
					rigidComp->m_Acceleration = { 0.f,0.f };

					ColComp->m_isCollided = 1.0f;
				}


				if (std::find_if(m_vecRigidBodyComponentPtr.begin(), m_vecRigidBodyComponentPtr.end(), [obj1_EntityID](const auto& obj) { return obj->m_Entity == obj1_EntityID; })
					!= m_vecRigidBodyComponentPtr.end()) {
					if (ColComp->m_collisionResponse) {
						vector2::Vec2 toMove{};
						if (swap) {
							toMove = valit->second.first * valit->second.second;
						}
						else {
							toMove = -valit->second.first * valit->second.second;
						}
						//std::cout << "x move:" << toMove.m_x << std::endl;
						//std::cout << "y move:" << toMove.m_y << std::endl;

						transform->m_position += toMove;
					}
					//obj1_RC->m_Velocity = -obj1_RC->m_Velocity;
				}
			}


			











			//const EntityID check_ID = ColComp->m_Entity;
			//const auto& iterator = std::find_if(vecCollisionEntityPair.begin(), vecCollisionEntityPair.end(), [check_ID](const auto pair) { return (static_cast<int>(check_ID) == pair.first->m_ID); });
			//if (iterator != vecCollisionEntityPair.end()) {
			//	ColComp->m_collidedWith.push_back(iterator->second->m_ID);
			//	ColComp->m_isCollided = true;
			//	ColComp->m_blockedFlag = iterator->first->m_collisionFlags;
			//}

		}
		//if (!vecCollisionEntityPairWithVector.empty()) {
		//	for (int i{}; i < vecCollisionEntityPairWithVector.size(); ++i) {
		//		std::pair < std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData> >, std::pair<vector2::Vec2, float>> val = vecCollisionEntityPairWithVector[i];
		//		EntityID obj1_EntityID = val.first.first.get()->m_ID;
		//		EntityID obj2_EntityID = val.first.second.get()->m_ID;

		//		//NameComponent* obj1_NC = (NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(obj1_EntityID);
		//		//NameComponent* obj2_NC = (NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(obj2_EntityID);
		//		TransformComponent* obj1_TC = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(obj1_EntityID);
		//		TransformComponent* obj2_TC = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(obj2_EntityID);
		//		ColliderComponent* obj1_CC = (ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(obj1_EntityID);
		//		ColliderComponent* obj2_CC = (ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(obj2_EntityID);
		//		RigidBodyComponent* obj1_RC = (RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(obj1_EntityID);
		//		RigidBodyComponent* obj2_RC = (RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(obj2_EntityID);

		//		//if (obj2_NC->m_entityTag != "Wall") {
		//		//	obj2_CC->m_isCollided = 1.0f;
		//		//}
		//		//if (obj1_NC->m_entityTag != "Wall") {
		//		//	obj1_CC->m_isCollided = 1.0f;
		//		//}

		//		obj2_CC->m_isCollided = 1.0f;
		//		obj1_CC->m_isCollided = 1.0f;


		//		if (obj1_CC->m_CollisionCheck == false && obj2_CC->m_CollisionCheck == false)
		//			continue;

		//		if (obj1_CC->m_CollisionCheck && obj2_CC->m_CollisionCheck && obj1_RC != NULL && obj2_RC != NULL) {
		//			obj1_TC->m_position += (-val.second.first * val.second.second);
		//			obj2_TC->m_position += (val.second.first * val.second.second);
		//			obj1_RC->m_Velocity = {};
		//			obj2_RC->m_Velocity = {};
		//			obj2_CC->m_isCollided = 1.0f;
		//			obj1_CC->m_isCollided = 1.0f;
		//		}

		//		

		//				if (std::find_if(m_vecRigidBodyComponentPtr.begin(), m_vecRigidBodyComponentPtr.end(), [obj2_EntityID](const auto& obj) { return obj->m_Entity == obj2_EntityID; })
		//					!= m_vecRigidBodyComponentPtr.end()) {
		//					if (obj2_CC->m_CollisionCheck) {
		//						vector2::Vec2 toMove = val.second.first * val.second.second; //DISTANCE TO SHIFT BACK
		//						obj2_TC->m_position += toMove;
		//					}
		//					//obj2_RC->m_Velocity = -obj2_RC->m_Velocity;
		//				}
		//			

		//				if (std::find_if(m_vecRigidBodyComponentPtr.begin(), m_vecRigidBodyComponentPtr.end(), [obj1_EntityID](const auto& obj) { return obj->m_Entity == obj1_EntityID; })
		//					!= m_vecRigidBodyComponentPtr.end()) {
		//					if (obj1_CC->m_CollisionCheck) {
		//						vector2::Vec2 toMove = -val.second.first * val.second.second;
		//						obj1_TC->m_position += toMove;
		//					}
		//					//obj1_RC->m_Velocity = -obj1_RC->m_Velocity;
		//				}
		//			
		//		
		//	}
		//}

	}
}
