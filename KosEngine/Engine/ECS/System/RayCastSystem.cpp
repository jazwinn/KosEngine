#include "../Config/pch.h"
#include "RayCastSystem.h"

/*!
\file      RayCastSystem.cp
\author	   Jazwinn Ng
\par       jazwinn.ng@digipen.edu
\date      Jan 20, 2025
\brief     This file holds the declaration of the RayCastSystem class

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "../ECS.h"

#include "RayCastSystem.h"
#include "../Helper/Helper.h"
#include "../Physics/Physics.h"


namespace ecs {

	void RayCastSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecRaycastComponentPtr.push_back((RaycastComponent*)ecs->m_ECS_CombinedComponentPool[TYPERAYCASTINGCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void RayCastSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& RbComponentPtr : m_vecRaycastComponentPtr) {
			if (RbComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecRaycastComponentPtr.size() - 1;
		std::swap(m_vecRaycastComponentPtr[IndexID], m_vecRaycastComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecRaycastComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void RayCastSystem::m_Init() {
		m_SystemSignature.set(TYPERAYCASTINGCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void RayCastSystem::m_Update(const std::string& scene) {
		if (m_vecTransformComponentPtr.size() != m_vecRaycastComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();
		ECS* ecs = ECS::m_GetInstance();

		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			RaycastComponent* raycast = m_vecRaycastComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((raycast->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;



			//calcuate distance between entity and target
			for (auto& ray : raycast->m_raycast) {
				if (!ray.m_isRaycasting) {
					ray.m_targetReached = true;
					ray.m_hitposition = { 0,0 };
					continue;
				}

				ray.m_targetReached = true;
				//vector2::Vec2 direction = ray.m_targetPosition - transform->m_position;
				ray.m_distance = abs(vector2::Vec2::m_funcVec2DDistance(ray.m_targetPosition, transform->m_position));

				PhysicsPipeline->IsLineIntersecting(raycast->m_Entity ,transform->m_position, ray.m_targetPosition, ray.m_Layers, ray.m_targetReached, ray.m_hitposition);

			}
			


		}

	}

}
