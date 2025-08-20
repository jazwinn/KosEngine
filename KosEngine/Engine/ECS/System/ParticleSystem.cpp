/******************************************************************/
/*!
\file      ParticleSystem.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      Feb 30, 2025
\brief     This file contains class defination for the particle System

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"

#include "../ECS.h"

#include "../ECS/System/ParticleSystem.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"

namespace ecs {

	void ParticleSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecParticleComponentPtr.push_back((ParticleComponent*)ecs->m_ECS_CombinedComponentPool[TYPEPARTICLECOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void ParticleSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& ParticleComponentPtr : m_vecParticleComponentPtr) {
			if (ParticleComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecParticleComponentPtr.size() - 1;
		std::swap(m_vecParticleComponentPtr[IndexID], m_vecParticleComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecParticleComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void ParticleSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPEPARTICLECOMPONENT);
	}

	void ParticleSystem::m_Update(const std::string& scene)
	{
		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		//assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		if (m_vecParticleComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecParticleComponentPtr.size(); n++)
		{
			TransformComponent* transform = m_vecTransformComponentPtr[n];
			ParticleComponent* particle = m_vecParticleComponentPtr[n];
			NameComponent* nc = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((particle->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(nc->m_Layer)) continue;

			unsigned int textureid{};
			if (assetmanager->m_imageManager.m_imageMap.find(particle->m_imageFile) == assetmanager->m_imageManager.m_imageMap.end())
			{
				textureid = 300;
			}
			else
			{
				textureid = assetmanager->m_imageManager.m_imageMap.find(particle->m_imageFile)->second.m_imageID;
			}
			
		
			if (particle->m_willSpawn)
			{
				float x, y;
				if (transform->m_haveParent)
				{
					
					x = transform->m_transformation.m_e20;
					y = transform->m_transformation.m_e21;
				}
				else
				{
					x = transform->m_transformation.m_e20;
					y = transform->m_transformation.m_e21;
				}
				
				graphicsPipe->m_emitterData.push_back({ particle->m_noOfParticles, particle->m_lifeSpan, {x, y}, {particle->m_velocity.m_x,particle->m_velocity.m_y}, {particle->m_acceleration.m_x,particle->m_acceleration.m_y},
				{particle->m_particleSize.m_x, particle->m_particleSize.m_y} , {particle->m_color.m_x,particle->m_color.m_y,particle->m_color.m_z, 1.f }, transform->m_rotation, particle->m_coneRotation, particle->m_coneAngle /*Change this later*/,particle->m_randomFactor,
				textureid, particle->m_stripCount, particle->m_frameNumber, particle->m_fps, particle->m_loopAnimation,particle->m_layer, particle->m_friction});
				particle->m_willSpawn = false;
			}
			



		}


	}


}

