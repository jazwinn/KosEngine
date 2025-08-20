/******************************************************************/
/*!
\file      AnimationSystem.cpp
\author    Sean Tiu, s.tiu, 2303398
\par       s.tiu@digipen.edu
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
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"
#include "AnimationSystem.h"
#include "../Debugging/Logging.h"



namespace ecs {

	void AnimationSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecAnimationComponentPtr.begin(), m_vecAnimationComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecAnimationComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecAnimationComponentPtr.push_back((AnimationComponent*)ecs->m_ECS_CombinedComponentPool[TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(ID));
			m_vecSpriteComponentPtr.push_back((SpriteComponent*)ecs->m_ECS_CombinedComponentPool[TYPESPRITECOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void AnimationSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecAnimationComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecAnimationComponentPtr.size() - 1;

		std::swap(m_vecAnimationComponentPtr[IndexID], m_vecAnimationComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecSpriteComponentPtr[IndexID], m_vecSpriteComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecAnimationComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecSpriteComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void AnimationSystem::m_Init() {

		m_SystemSignature.set(TYPEANIMATIONCOMPONENT);
		m_SystemSignature.set(TYPESPRITECOMPONENT);

	}

	void AnimationSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecAnimationComponentPtr.size() != m_vecTransformComponentPtr.size() && m_vecAnimationComponentPtr.size() != m_vecSpriteComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		Helper::Helpers* helper = Helper::Helpers::GetInstance();
		for (int n{}; n < m_vecAnimationComponentPtr.size(); n++) {

			AnimationComponent* AniComp = m_vecAnimationComponentPtr[n];
			SpriteComponent* SpriteComp = m_vecSpriteComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];

			//skip component not of the scene
			if ((AniComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide ) continue;

			if (m_vecAnimationComponentPtr[n]->m_isAnimating && m_vecAnimationComponentPtr[n]->m_framesPerSecond)
			{
				for (int i = 0; i < helper->currentNumberOfSteps; ++i) //Fixed DT with Accumulation for Animations
				{
					m_vecAnimationComponentPtr[n]->m_frameTimer += helper->m_fixedDeltaTime;
				}
				float frameTime = 1.f / m_vecAnimationComponentPtr[n]->m_framesPerSecond;
				const auto& image = assetmanager->m_imageManager.m_imageMap.find(SpriteComp->m_imageFile);
				if (image == assetmanager->m_imageManager.m_imageMap.end()) continue;
				float spriteTotalTime = frameTime * m_vecAnimationComponentPtr[n]->m_stripCount;
				if (m_vecAnimationComponentPtr[n]->m_frameTimer > spriteTotalTime)
				{
					AniComp->m_frameTimer = 0;
				}
				if (frameTime < spriteTotalTime)
				{
					AniComp->m_frameNumber = static_cast<int>(AniComp->m_frameTimer / frameTime);
				}
			}
			else
			{
				m_vecAnimationComponentPtr[n]->m_frameTimer = 0;
			}
			

		}


	}


}
