/******************************************************************/
/*!
\file      VideoSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      March 01, 2025
\brief     This file contains the defination for the video system.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "VideoSystem.h"
#include "../ECS.h"
#include "../Graphics/GraphicsPipe.h"


namespace ecs {

	void VideoSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecVideoComponentPtr.begin(), m_vecVideoComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecVideoComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
			m_vecVideoComponentPtr.push_back((VideoComponent*)ecs->m_ECS_CombinedComponentPool[TYPEVIDEOCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void VideoSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecVideoComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecVideoComponentPtr.size() - 1;

		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);
		std::swap(m_vecVideoComponentPtr[IndexID], m_vecVideoComponentPtr[IndexLast]);
		//popback the vector;
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
		m_vecVideoComponentPtr.pop_back();

		//clear video map if not yet cleared
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		auto& videomap = assetmanager->m_videoManager.m_videoMap;
		videomap.erase(ID);
	}

	void VideoSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPEVIDEOCOMPONENT);
		//SystemSignature.set();

	}

	void VideoSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		auto& videomap = assetmanager->m_videoManager.m_videoMap;


		for (int n{}; n < m_vecTransformComponentPtr.size(); n++)
		{
			TransformComponent* transform = m_vecTransformComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			VideoComponent* VideoComp = m_vecVideoComponentPtr[n];

			//skip component not of the scene
			if ((transform->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

			if (VideoComp->play == false) {

				videomap.erase(VideoComp->m_Entity);

				continue;
			}


			if (videomap.find(VideoComp->m_Entity) == videomap.end()) {

				auto it = assetmanager->m_videoManager.m_videopath.find(VideoComp->filename);

				if (it == assetmanager->m_videoManager.m_videopath.end()) continue;

				std::bitset<video::VIDEO_FLAGS::TOTAL> videoFlags;
				videoFlags.set(video::VIDEO_FLAGS::AUDIO);
				if (VideoComp->loop) {
					videoFlags.set(video::VIDEO_FLAGS::LOOP);
				}
				videomap[VideoComp->m_Entity] = std::make_unique<video::Video>(it->second.string(), graphicsPipe->m_videoShaderProgram, videoFlags);

				
			}


			
			assetmanager->m_videoManager.m_videoMap[VideoComp->m_Entity]->DecodeAndUpdateVideo(VideoComp->pause);
			

			graphicsPipe->m_videoData.push_back(graphicpipe::VideoData(glm::mat3{ transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
																		transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
																		transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22 },
																		videomap[VideoComp->m_Entity]->yTexture, videomap[VideoComp->m_Entity]->uTexture, videomap[VideoComp->m_Entity]->vTexture,
																		videomap[VideoComp->m_Entity]->locTransformation, videomap[VideoComp->m_Entity]->locView, videomap[VideoComp->m_Entity]->locProjection,
																		videomap[VideoComp->m_Entity]->unilayer, VideoComp->layer
																		));


			//check if video finish playing
			if (videomap[VideoComp->m_Entity]->HasStopped()) {
				VideoComp->play = false;
			}
		}

	}


}
