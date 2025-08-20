/******************************************************************/
/*!
\file      PathfindingSystem.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 29, 2024
\brief

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../ECS.h"

#include "PhysicsSystem.h"
#include "../Helper/Helper.h"


namespace ecs {

	void PhysicsSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void PhysicsSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& RbComponentPtr : m_vecRigidBodyComponentPtr) {
			if (RbComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecRigidBodyComponentPtr.size() - 1;
		std::swap(m_vecRigidBodyComponentPtr[IndexID], m_vecRigidBodyComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);
		//popback the vector;
		m_vecRigidBodyComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void PhysicsSystem::m_Init() {
		m_SystemSignature.set(TYPERIGIDBODYCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void PhysicsSystem::m_Update(const std::string& scene) {
		ECS* ecs = ECS::m_GetInstance();
		//physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::getInstance();
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		if (m_vecRigidBodyComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}
		//std::cout << help->currentNumberOfSteps << std::endl;
		for (int i = 0; i < help->currentNumberOfSteps; ++i) {
		// Loop through all vectors pointing to components
		for (int n = 0; n < m_vecRigidBodyComponentPtr.size(); n++) {
			RigidBodyComponent* rigidBody = m_vecRigidBodyComponentPtr[n];
			TransformComponent* transform = m_vecTransformComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];

			//skip component not of the scene
			if ((rigidBody->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;


			if (rigidBody->m_IsKinematic || rigidBody->m_IsStatic) {
				// If kinematic or static, skip physics calculations
				return;
			}
			
			
				// Integrate linear motion
				vector2::Vec2 acceleration = rigidBody->m_Acceleration + rigidBody->m_Force * rigidBody->m_InverseMass;
				rigidBody->m_Velocity += acceleration * help->m_fixedDeltaTime;
				rigidBody->m_Velocity *= rigidBody->m_LinearDamping; // Apply linear damping

				// Integrate angular motion
				float angularAcceleration = rigidBody->m_Torque * rigidBody->m_InverseMass;
				rigidBody->m_AngularVelocity += angularAcceleration * help->m_fixedDeltaTime;
				rigidBody->m_AngularVelocity *= rigidBody->m_AngularDamping;

				//store previous position
				rigidBody->m_PrevPos = transform->m_position;

				transform->m_position += rigidBody->m_Velocity * help->m_fixedDeltaTime;
				transform->m_rotation += rigidBody->m_AngularVelocity * help->m_fixedDeltaTime;

				rigidBody->m_Force = vector2::Vec2{ 0.0f, 0.0f };
				rigidBody->m_Torque = 0.0f;

				

				if (!rigidBody->m_IsStatic && !rigidBody->m_IsKinematic) {
					transform->m_position += rigidBody->m_Velocity * help->m_fixedDeltaTime;
					transform->m_rotation += rigidBody->m_AngularVelocity * help->m_fixedDeltaTime;
				}

				
				rigidBody->m_DirectionVector = transform->m_position - rigidBody->m_PrevPos;
				if (!(rigidBody->m_DirectionVector.m_x == 0.f && rigidBody->m_DirectionVector.m_y == 0.f)) {
					rigidBody->m_PrevDirVec = rigidBody->m_DirectionVector;
				}
				

				//update physics pipline
			
				//TODO optimize,  causing longer load time
				
			}
		}
	}
}

