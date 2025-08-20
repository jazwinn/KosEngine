/******************************************************************/
/*!
\file      Audio.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 30, 2024
\brief     This file contains the implementation of the Audio class,
           which provides audio

This file implements a simple wrapper class FModAudio for handling 
audio playback using the FMOD sound system. It provides basic 
functionalities such as initializing the FMOD system, creating and 
playing sounds, controlling volume and panning, pausing and stopping 
playback, looping, and fading sound. Additionally, the file handles 
proper resource management by cleaning up the FMOD system and releasing 
resources when the audio operations are finished or when the object 
is destroyed.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../ECS/ECS.h"
#include "Audio.h"
//#include <fmod_errors.h>

namespace fmodaudio {
    FModAudio::FModAudio(FMOD::System* system) : m_system(system), m_sound(nullptr) {
        m_InitializeChannelPool();
    }

    FModAudio::~FModAudio() {
        float fadeOutTime = 0.3f;
        m_StopAllSounds(fadeOutTime);
        m_Shutdown();
    }

    bool FModAudio::m_Init() {
        FMOD_RESULT result = FMOD::System_Create(&m_system);
        if (result != FMOD_OK) {
            return false;
        }

        result = m_system->init(64, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            return false;
        }
        return true;
    }

    void FModAudio::m_Shutdown() {
        float fadeOutTime = 0.3f;
        m_StopAllSounds(fadeOutTime);
        if (m_system) {
            m_system->release();
            m_system = nullptr;
        }
    }

    void FModAudio::Update(float deltaTime [[maybe_unused]] ) {
        auto currentTime = std::chrono::steady_clock::now();

        for (auto& info : m_channelPool) {
            if (info.channel && info.isFading) {
                float elapsedTime = std::chrono::duration<float>(
                    currentTime - info.fadeStartTime).count();

                if (elapsedTime >= info.fadeDuration) {
                    info.channel->setVolume(info.fadeTargetVolume);
                    info.isFading = false;

                    if (info.isFadingOut && info.fadeTargetVolume == 0.0f) {
                        info.channel->stop();
                        auto it = m_entityChannels.find(info.entityId);
                        if (it != m_entityChannels.end()) {
                            m_entityChannels.erase(it);
                        }
                        info.isActive = false;
                        info.isFadingOut = false;
                        info.channel = nullptr;
                    }
                }
                else {
                    float t = elapsedTime / info.fadeDuration;
                    float newVolume = info.fadeStartVolume +
                        (info.fadeTargetVolume - info.fadeStartVolume) * t;
                    info.channel->setVolume(newVolume);
                }
            }
        }

        m_ReleaseUnusedChannels();
    }

    bool FModAudio::m_CreateSound(const char* soundFile) {
        FMOD_RESULT result = m_system->createSound(soundFile, FMOD_DEFAULT, nullptr, &m_sound);
        if (result != FMOD_OK) {
            return false;
        }
        return true;
    }

    bool FModAudio::m_PlaySound(const std::string& entityId) {
        if (!m_system || !m_sound) {
            return false;
        }
        //m_PrintChannelStatus();

        m_ReleaseUnusedChannels();

        FMOD::Channel* channel = nullptr;
        int channelIndex = -1;

        for (size_t i = 0; i < m_channelPool.size(); ++i) {
            if (!m_channelPool[i].isActive || !m_channelPool[i].channel) {
                channelIndex = static_cast<int>(i);
                break;
            }
        }

        if (channelIndex == -1) {
            for (size_t i = 0; i < m_channelPool.size(); ++i) {
                if (m_channelPool[i].channel) {
                    bool isPlaying = false;
                    m_channelPool[i].channel->isPlaying(&isPlaying);
                    if (!isPlaying) {
                        channelIndex = static_cast<int>(i);
                        channel = m_channelPool[i].channel;
                        break;
                    }
                }
            }
        }

        if (channelIndex == -1) {
            channelIndex = m_FindLeastImportantChannel();
            if (channelIndex >= 0) {
                channel = m_channelPool[channelIndex].channel;
                if (channel) {
                    channel->stop();
                }
            }
        }

        if (channelIndex >= 0) {
            FMOD_RESULT result;
            if (channel) {
                channel->stop();  
                result = m_system->playSound(m_sound, nullptr, false, &channel);
            }
            else {
                result = m_system->playSound(m_sound, nullptr, false, &channel);
            }

            if (result != FMOD_OK) {
                return false;
            }

            auto& info = m_channelPool[channelIndex];
            info.channel = channel;
            info.entityId = entityId;
            info.isActive = true;
            info.lastUsed = std::chrono::steady_clock::now();

            m_entityChannels[entityId] = channel;

            if (channel) {
                channel->setVolume(1.0f);
            }

            return true;
        }

        return false;
    }

    int FModAudio::m_GetActiveChannelCount() const {
        int count = 0;
        for (const auto& info : m_channelPool) {
            if (info.isActive && info.channel) {
                bool isPlaying = false;
                info.channel->isPlaying(&isPlaying);
                if (isPlaying) count++;
            }
        }
        return count;
    }

    void FModAudio::m_PrintChannelStatus() const {
        //std::cout << "Active Channels: " << m_GetActiveChannelCount() << "/" << MAX_CHANNELS << "\n";
        for (size_t i = 0; i < m_channelPool.size(); ++i) {
            const auto& info = m_channelPool[i];
            if (info.isActive && info.channel) {
                bool isPlaying = false;
                info.channel->isPlaying(&isPlaying);
                //std::cout << "Channel " << i << ": " << (isPlaying ? "Playing" : "Stopped")
                   // << " EntityID: " << info.entityId << "\n";
            }
        }
    }



    void FModAudio::m_StopSound(const std::string& entityId) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            it->second->stop();

            for (auto& info : m_channelPool) {
                if (info.entityId == entityId) {
                    info.isActive = false;
                    info.channel = nullptr;
                    break;
                }
            }

            m_entityChannels.erase(it);
        }
    }

    void FModAudio::m_StopAllSounds(float fadeOutTime) {

        for (auto& pair : m_entityChannels) {
            m_FadeSound(pair.first, 0.0f, fadeOutTime);

            for (auto& info : m_channelPool) {
                if (info.channel == pair.second) {
                    info.isFadingOut = true;
                    info.fadeCompleteTime = std::chrono::steady_clock::now() +
                        std::chrono::milliseconds(static_cast<int>(fadeOutTime * 1000));
                    break;
                }
            }
        }
    }

    void FModAudio::m_PauseAllSounds() {
        for (auto& pair : m_entityChannels) {
            FMOD::Channel* channel = pair.second;
            if (channel) {
                channel->setPaused(true);
            }
        }
    }

    bool FModAudio::m_UnpauseAllSounds() {
        for (auto& channelPair : m_entityChannels) {
            FMOD::Channel* channel = channelPair.second;
            if (channel) {
                bool isPaused = false;
                channel->getPaused(&isPaused);
                if (isPaused) {
                    channel->setPaused(false);
                }
            }
        }
        return true;
    }

    bool FModAudio::m_SetVolume(const std::string& entityId, float volume) {
        volume = std::clamp(volume, 0.0f, 1.0f);

        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            FMOD_RESULT result = it->second->setVolume(volume);
            if (result != FMOD_OK) {
                return false;
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_SetPan(const std::string& entityId, float pan) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            //std::cout << pan << std::endl;
            FMOD_RESULT result = it->second->setPan(pan);
            if (result != FMOD_OK) {
                return false;
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_PauseSound(const std::string& entityId) {
        auto channelIt = m_entityChannels.find(entityId);
        if (channelIt != m_entityChannels.end()) {
            FMOD::Channel* channel = channelIt->second;
            bool isPaused = false;
            channel->getPaused(&isPaused);
            if (!isPaused) {
                channel->setPaused(true);
                return true;
            }
        }
        return false;
    }

    bool FModAudio::m_UnpauseSound(const std::string& entityId) {
        auto channelIt = m_entityChannels.find(entityId);
        if (channelIt != m_entityChannels.end()) {
            FMOD::Channel* channel = channelIt->second;
            bool isPaused = false;
            channel->getPaused(&isPaused);
            if (isPaused) {
                channel->setPaused(false);
                return true;
            }
        }
        return false;
    }


    bool FModAudio::m_IsPlaying(const std::string& entityId) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            bool isPlaying = false;
            FMOD_RESULT result = it->second->isPlaying(&isPlaying);
            if (result != FMOD_OK) {
                return false;
            }
            return isPlaying;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_SetLooping(const std::string& entityId, bool loop) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
            FMOD_RESULT result = it->second->setMode(mode);
            if (result != FMOD_OK) {
                return false;
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_FadeSound(const std::string& entityId, float targetVolume, float fadeDuration) {
        auto it = m_entityChannels.find(entityId);
        if (it == m_entityChannels.end()) {
            return false;
        }

        FMOD::Channel* channel = it->second;

        FMOD_RESULT result = channel->setVolumeRamp(true);
        if (result != FMOD_OK) {
            return false;
        }

        float currentVolume;
        result = channel->getVolume(&currentVolume);
        if (result != FMOD_OK) {
            return false;
        }

        for (auto& info : m_channelPool) {
            if (info.channel == channel) {
                info.isFading = true;
                info.fadeStartTime = std::chrono::steady_clock::now();
                info.fadeDuration = fadeDuration;
                info.fadeStartVolume = currentVolume;
                info.fadeTargetVolume = targetVolume;
                return true;
            }
        }

        return false;
    }

    FMOD::Channel* FModAudio::m_GetChannelForEntity(ecs::EntityID entityId) {
        std::string entityIdStr = std::to_string(entityId);
        auto it = m_entityChannels.find(entityIdStr);
        if (it != m_entityChannels.end()) {
            return it->second;
        }
        return nullptr;
    }

    void FModAudio::m_InitializeChannelPool() {
        m_channelPool.resize(MAX_CHANNELS);
        for (auto& info : m_channelPool) {
            info.channel = nullptr;
            info.isActive = false;
            info.priority = 0.0f;
        }
    }

    FMOD::Channel* FModAudio::m_GetAvailableChannel(const std::string& entityId) {
        for (auto& info : m_channelPool) {
            if (!info.isActive) {
                info.entityId = entityId;
                info.isActive = true;
                info.lastUsed = std::chrono::steady_clock::now();
                return info.channel;
            }
        }

        int index = m_FindLeastImportantChannel();
        if (index >= 0) {
            auto& info = m_channelPool[index];
            if (info.channel) {
                info.channel->stop();
            }
            info.entityId = entityId;
            info.lastUsed = std::chrono::steady_clock::now();
            return info.channel;
        }

        return nullptr;
    }

    void FModAudio::m_ReleaseUnusedChannels() {
        auto currentTime = std::chrono::steady_clock::now();

        for (auto& info : m_channelPool) {
            if (info.channel) {
                bool isPlaying = false;
                info.channel->isPlaying(&isPlaying);

                bool shouldRelease = !isPlaying;

                if (info.isFadingOut && currentTime >= info.fadeCompleteTime) {
                    shouldRelease = true;
                }

                if (shouldRelease) {
                    info.channel->stop();
                    auto it = m_entityChannels.find(info.entityId);
                    if (it != m_entityChannels.end()) {
                        m_entityChannels.erase(it);
                    }
                    info.isActive = false;
                    info.isFadingOut = false;
                    info.channel = nullptr;
                }
            }
        }
    }

    int FModAudio::m_FindLeastImportantChannel() const {
        int leastImportantIdx = -1;
        auto oldestTime = std::chrono::steady_clock::now();

        for (size_t i = 0; i < m_channelPool.size(); ++i) {
            const auto& info = m_channelPool[i];
            if (info.channel) {
                bool isPlaying = false;
                info.channel->isPlaying(&isPlaying);
                if (!isPlaying || info.lastUsed < oldestTime) {
                    leastImportantIdx = static_cast<int>(i);
                    oldestTime = info.lastUsed;
                }
            }
            else {
                return static_cast<int>(i);
            }
        }
        return leastImportantIdx;
    }

    // AudioManager Implementation (UPDATED 28/11/2024)
    AudioManager::AudioManager() {
        FMOD::System_Create(&m_system);
        m_system->init(64, FMOD_INIT_NORMAL, nullptr);
    }

    AudioManager::~AudioManager() {
        m_system->release();
        for (auto& pair : m_soundMap) {
            FModAudio* sound = pair.second.get();
            if (sound) {
                sound->m_Shutdown();
            }
        }
    }

    void AudioManager::m_LoadAudio(const std::string& name, const std::string& path) {
        auto sound = std::make_unique<FModAudio>(m_system);
        if (sound->m_CreateSound(path.c_str())) {
            m_soundMap[name] = std::move(sound);
        }
        else {
            //TODO Handle error (e.g., logging or notification)
            /*std::cerr << "Failed to load audio: " << path << std::endl;
            std::cerr << sound->m_Init() << std::endl;
            std::cerr << sound->m_CreateSound(path.c_str()) << std::endl;*/
        }
    }

    void AudioManager::m_PlayAudioForEntity(ecs::EntityID entityId, const std::string& name, float volume) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();
            std::string entityIdStr = std::to_string(entityId);

            auto* ecs = ecs::ECS::m_GetInstance();
            auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

            if (entityComponent) {
                ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);
                for (auto& audioFile : ac->m_AudioFiles) {
                    if (audioFile.m_Name == name) {
                        float adjustedVolume = volume;
                        if (audioFile.m_IsBGM) {
                            if (m_GlobalBGMVolume == 0) return;
                            adjustedVolume *= m_GlobalBGMVolume;
                        }
                        else if (audioFile.m_IsSFX) {
                            if (m_GlobalSFXVolume == 0) return;
                            adjustedVolume *= m_GlobalSFXVolume;
                        }

                        float pan = audioFile.m_Pan;

                        if (sound->m_PlaySound(entityIdStr)) {
                            sound->m_SetVolume(entityIdStr, adjustedVolume);
                            sound->m_SetPan(entityIdStr, pan);
                        }
                        return;
                    }
                }
            }
        }
        else {
            // TODO Handle error (e.g., logging or notification)
           // LOGGING_WARN("Sound not found: " + name);
        }
    }


    void AudioManager::m_StopAudioForEntity(ecs::EntityID entityId, const std::string& name) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();
            sound->m_StopSound(std::to_string(entityId));
        }
        else {
            // TODO Handle error (e.g., logging or notification)
            // std::cerr << "Sound not found: " << name << std::endl;
        }
    }

    void AudioManager::m_PauseAudioForEntity(ecs::EntityID entityId, const std::string& name) {
        auto& sound = m_soundMap[name];
        if (sound) {
            sound->m_PauseSound(std::to_string(entityId));
        }
    }

    void AudioManager::m_UnpauseAudioForEntity(ecs::EntityID entityId, const std::string& name) {
        auto& sound = m_soundMap[name];
        if (sound) {
            sound->m_UnpauseSound(std::to_string(entityId));
        }
    }

    void AudioManager::m_SetVolumeForEntity(ecs::EntityID entityId, const std::string& name, float volume) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();

            auto* ecs = ecs::ECS::m_GetInstance();
            auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

            if (entityComponent) {
                ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

                for (auto& audioFile : ac->m_AudioFiles) {
                    if (audioFile.m_Name == name) {
                        float adjustedVolume = volume;
                        if (audioFile.m_IsBGM) {
                            adjustedVolume *= m_GlobalBGMVolume;
                        }
                        else if (audioFile.m_IsSFX) {
                            adjustedVolume *= m_GlobalSFXVolume;
                        }

                        sound->m_SetVolume(std::to_string(entityId), adjustedVolume);
                        return;
                    }
                }
            }
        }
        else {
            LOGGING_WARN("Sound not found: " + name);
        }
    }


    void AudioManager::m_SetLoopingForEntity(ecs::EntityID entityId, const std::string& name, bool loop) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            auto& audio = it->second;
            FMOD::Channel* channel = audio->m_GetChannelForEntity(entityId);
            if (channel) {
                // Set the loop count: -1 for infinite loop, 0 for no loop, or a specific number for limited loops
                channel->setLoopCount(loop ? -1 : 0);
            }
        }
    }

    void AudioManager::m_SetIsBGMForEntity(ecs::EntityID entityId, const std::string& name, bool isBGM) {
        auto* ecs = ecs::ECS::m_GetInstance();
        auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

        if (entityComponent) {
            ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

            for (auto& audioFile : ac->m_AudioFiles) {
                if (audioFile.m_Name == name) {
                    audioFile.m_IsBGM = isBGM;
                    break;  // Stop after finding the correct file
                }
            }
        }
    }


    void AudioManager::m_SetIsSFXForEntity(ecs::EntityID entityId, const std::string& name, bool isSFX) {
        auto* ecs = ecs::ECS::m_GetInstance();
        auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

        if (entityComponent) {
            ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

            for (auto& audioFile : ac->m_AudioFiles) {
                if (audioFile.m_Name == name) {
                    audioFile.m_IsSFX = isSFX;
                    break;  
                }
            }
        }
    }


    bool AudioManager::m_CheckIsBGMForEntity(ecs::EntityID entityId, const std::string& name) {
        auto* ecs = ecs::ECS::m_GetInstance();
        auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

        if (entityComponent) {
            ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

            for (auto& audioFile : ac->m_AudioFiles) {
                if (audioFile.m_Name == name) {
                    return audioFile.m_IsBGM;
                }
            }
        }
        else
            LOGGING_WARN("Audio Component not found, unable to CheckIsBGM");
            return false;
    }

    bool AudioManager::m_CheckIsSFXForEntity(ecs::EntityID entityId, const std::string& name) {
        auto* ecs = ecs::ECS::m_GetInstance();
        auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

        if (entityComponent) {
            ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

            for (auto& audioFile : ac->m_AudioFiles) {
                if (audioFile.m_Name == name) {
                    return audioFile.m_IsSFX;
                }
            }
        }
        else
            LOGGING_WARN("Audio Component not found, unable to CheckIsSFX");
            return false;
    }

    void AudioManager::m_SetGlobalBGMVolume(float volume) {
        if (0.0f <= volume && volume <= 1.0f)
        {
            m_GlobalBGMVolume = volume;
        }
    }

    void AudioManager::m_SetGlobalSFXVolume(float volume) {
        if (0.0f <= volume && volume <= 1.0f)
        {
            m_GlobalSFXVolume = volume;
        }
    }

    void AudioManager::m_SetPlayOnStartForEntity(ecs::EntityID entityId, const std::string& audioName, bool playOnStart) {
        auto* ecs = ecs::ECS::m_GetInstance();
        auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

        if (entityComponent) {
            ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

            for (auto& audioFile : ac->m_AudioFiles) {
                if (audioFile.m_Name == audioName) {
                    audioFile.m_PlayOnStart = playOnStart;
                }
                else if (playOnStart) {
                    audioFile.m_PlayOnStart = false;
                }
            }
        }
    }

    bool AudioManager::m_IsPlayingForEntity(ecs::EntityID entityId, const std::string& name) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();
            return sound->m_IsPlaying(std::to_string(entityId));
        }
        else {
            // TODO Handle error (e.g., logging or notification)
            // std::cerr << "Sound not found: " << name << std::endl;
            return false;
        }
    }

    void AudioManager::m_SetPanForEntity(ecs::EntityID entityId, const std::string& audioName, float pan) {
        auto* ecs = ecs::ECS::m_GetInstance();
        auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId);

        if (entityComponent) {
            ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

            for (auto& audioFile : ac->m_AudioFiles) {
                if (audioFile.m_Name == audioName) {
                    audioFile.m_Pan = pan;
                    //std::cout << audioFile.m_Pan << std::endl;
                    audioFile.m_hasChanged = true;
                    break;
                }
            }
        }

        auto it = m_soundMap.find(audioName);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();

            if (!sound->m_SetPan(std::to_string(entityId), pan)) {
                //std::cerr << "Failed to set pan for entity " << entityId << " on sound " << audioName << std::endl;
            }
        }
        else {
            //std::cerr << "Sound not found: " << audioName << std::endl;
        }
    }




    void AudioManager::m_PauseAllSounds() {
        for (auto& soundPair : m_soundMap) {
            FModAudio* audio = soundPair.second.get();
            if (audio) {
                audio->m_PauseAllSounds();
            }
        }
    }

    void AudioManager::m_UnpauseAllSounds() {
        for (auto& soundPair : m_soundMap) {
            FModAudio* audio = soundPair.second.get();
            if (audio) {
                audio->m_UnpauseAllSounds();
            }
        }
    }


    void AudioManager::m_StopAllSounds() {
        for (auto& soundPair : m_soundMap) {
            FModAudio* audio = soundPair.second.get();
            if (audio) {
                float fadeOutTime = 0.3f;
                audio->m_StopAllSounds(fadeOutTime);
            }
        }
    }
}