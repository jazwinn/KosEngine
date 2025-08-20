/******************************************************************/
/*!
\file      AudioSystem.cpp
\author    Clarence Boey
\par
\date      Nov 27, 2024
\brief     This file contains the definition of the AudioSystem
           class. It handles the logic behind the Audio played in
           the engine and works with the Audio Manager interface.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "AudioSystem.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/Audio.h"

namespace ecs {

    void AudioSystem::m_RegisterSystem(EntityID ID) {
        ECS* ecs = ECS::m_GetInstance();

        if (std::find_if(m_vecAudioComponentPtr.begin(), m_vecAudioComponentPtr.end(),
            [ID](const auto& obj) { return obj->m_Entity == ID; }) == m_vecAudioComponentPtr.end()) {
            m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
            m_vecAudioComponentPtr.push_back((AudioComponent*)ecs->m_ECS_CombinedComponentPool[TYPEAUDIOCOMPONENT]->m_GetEntityComponent(ID));
            m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
        }
    }

    void AudioSystem::m_DeregisterSystem(EntityID ID) {
        size_t IndexID = 0;
        for (auto& audioCompPtr : m_vecAudioComponentPtr) {
            if (audioCompPtr->m_Entity == ID) {
                break;
            }
            IndexID++;
        }

        size_t IndexLast = m_vecAudioComponentPtr.size() - 1;
        std::swap(m_vecAudioComponentPtr[IndexID], m_vecAudioComponentPtr[IndexLast]);
        std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
        std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

        m_vecAudioComponentPtr.pop_back();
        m_vecTransformComponentPtr.pop_back();
        m_vecNameComponentPtr.pop_back();
    }

    void AudioSystem::m_Init() {
        m_SystemSignature.set(TYPEAUDIOCOMPONENT);
    }

    void AudioSystem::m_Update(const std::string& scene) {
        assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
        ECS* ecs = ECS::m_GetInstance();
        if (m_vecAudioComponentPtr.empty()) return;

        // Calculate delta time for smooth fading
        static auto lastUpdateTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastUpdateTime).count();
        lastUpdateTime = currentTime;

        // Update all FModAudio instances to process fading
        for (auto& soundPair : assetManager->m_audioManager.getSoundMap()) {
            fmodaudio::FModAudio* audio = soundPair.second.get();
            if (audio) {
                audio->Update(deltaTime);
            }
        }

        int n{ 0 };
        for (auto& audioCompPtr : m_vecAudioComponentPtr) {
            TransformComponent* transform = m_vecTransformComponentPtr[n];
            NameComponent* NameComp = m_vecNameComponentPtr[n];
            n++;
            if ((transform->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

            std::string entityIDStr = std::to_string(audioCompPtr->m_Entity);
            for (auto& audioFile : audioCompPtr->m_AudioFiles) {
                auto it = assetManager->m_audioManager.getSoundMap().find(audioFile.m_Name);
                if (it == assetManager->m_audioManager.getSoundMap().end()) continue;
                auto& sound = it->second;

                float adjustedVolume = audioFile.m_Volume;
                if (audioFile.m_IsBGM) {
                    adjustedVolume *= assetManager->m_audioManager.m_GlobalBGMVolume;
                }
                else if (audioFile.m_IsSFX) {
                    adjustedVolume *= assetManager->m_audioManager.m_GlobalSFXVolume;
                }
                if (adjustedVolume < 0) {
                    adjustedVolume = 0;
                }

                sound->m_SetVolume(entityIDStr, adjustedVolume);
                sound->m_SetLooping(entityIDStr, audioFile.m_Loop);

                if (audioFile.m_PlayOnStart && !sound->m_IsPlaying(entityIDStr)) {
                    sound->m_PlaySound(entityIDStr);
                    audioFile.m_PlayOnStart = false;
                }
            }
        }
    }



}
