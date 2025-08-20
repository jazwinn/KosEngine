/******************************************************************/
/*!
\file      Audio.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 30, 2024
\brief     This file contains the declaration of the FModAudio class,
           which acts as a simple wrapper around the FMOD audio system
           for handling audio playback and control.

This header file provides the declaration of the FModAudio class, which
utilizes the FMOD sound system to enable various audio functionalities.
These include initializing the FMOD system, creating and playing audio
files, controlling audio parameters such as volume, panning, and looping,
as well as handling sound playback with pause, stop, and fade features.
The class ensures proper resource management by cleaning up FMOD objects
and releasing resources when no longer in use.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "fmod.hpp"
//#include "fmod_errors.h"
#include <thread>
#include "../Config/pch.h"

#include "../ECS/ECS.h"
namespace fmodaudio {

    /******************************************************************/
    /*!
    \class   FModAudio
    \brief   A class that provides an interface to the FMOD audio system.
             It manages audio initialization, sound creation, playback,
             and control over various sound parameters such as volume,
             pan, and looping.
    */
    /******************************************************************/
    class FModAudio {
    public:

        /******************************************************************/
        /*!
        \fn      FModAudio()
        \brief   Constructor that initializes default values for the FModAudio class.
        */
        /******************************************************************/
        FModAudio(FMOD::System* system);

        /******************************************************************/
        /*!
        \fn      ~FModAudio()
        \brief   Destructor that releases FMOD resources.
        */
        /******************************************************************/
        ~FModAudio();

        /******************************************************************/
        /*!
        \fn      bool m_Init()
        \brief   Initializes the FMOD system to manage sound playback.
        \return  True if the system is successfully initialized, false otherwise.
        */
        /******************************************************************/
        bool m_Init();

        /******************************************************************/
        /*!
        \fn      void m_Shutdown()
        \brief   Shuts down the FMOD system and releases all allocated resources.
        */
        /******************************************************************/
        void m_Shutdown();

        void Update(float deltaTime);

        /******************************************************************/
        /*!
        \fn      bool m_CreateSound(const char* soundFile)
        \brief   Creates a sound object from the given sound file.
        \param   soundFile - Path to the audio file to load.
        \return  True if the sound is successfully created, false otherwise.
        */
        /******************************************************************/
        bool m_CreateSound(const char* soundFile);

        /******************************************************************/
        /*!
        \fn      bool m_PlaySound(const std::string& entityId)
        \brief   Plays a sound associated with the specified entity ID.
        \param   entityId - The ID of the entity that should play the sound.
        \return  True if the sound is successfully played, false otherwise.
        */
        /******************************************************************/
        bool m_PlaySound(const std::string& entityId);

        /******************************************************************/
        /*!
        \fn      void m_StopSound(const std::string& entityId)
        \brief   Stops the sound associated with the specified entity ID.
        \param   entityId - The ID of the entity for which to stop the sound.
        */
        /******************************************************************/
        void m_StopSound(const std::string& entityId);

        /******************************************************************/
        /*!
        \fn      void m_StopAllSounds()
        \brief   Stops all currently playing sounds.
        */
        /******************************************************************/
        void m_StopAllSounds(float fadeOutTime);

        /******************************************************************/
        /*!
        \fn      void m_PauseAllSounds()
        \brief   Pauses all currently playing sounds.
        */
        /******************************************************************/
        void m_PauseAllSounds();

        /******************************************************************/
        /*!
        \fn      bool m_UnpauseAllSounds()
        \brief   Unpauses all paused sounds.
        \return  True if sounds are successfully unpaused, false otherwise.
        */
        /******************************************************************/
        bool m_UnpauseAllSounds();

        /******************************************************************/
        /*!
        \fn      bool m_PauseSound(const std::string& entityId)
        \brief   Pauses the sound associated with the specified entity ID.
        \param   entityId - The ID of the entity for which to pause the sound.
        \return  True if the sound is successfully paused, false otherwise.
        */
        /******************************************************************/
        bool m_PauseSound(const std::string& entityId);

        /******************************************************************/
        /*!
        \fn      bool m_UnpauseSound(const std::string& entityId)
        \brief   Unpauses the sound associated with the specified entity ID.
        \param   entityId - The ID of the entity for which to unpause the sound.
        \return  True if the sound is successfully unpaused, false otherwise.
        */
        /******************************************************************/
        bool m_UnpauseSound(const std::string& entityId);

        /******************************************************************/
        /*!
        \fn      bool m_FadeSound(const std::string& entityId, float targetVolume, float fadeDuration)
        \brief   Gradually changes the volume of a sound to the target volume.
        \param   entityId - The ID of the entity for which to fade the sound.
        \param   targetVolume - The desired volume level.
        \param   fadeDuration - Duration over which the fade should occur.
        \return  True if the fade is successfully applied, false otherwise.
        */
        /******************************************************************/
        bool m_FadeSound(const std::string& entityId, float targetVolume, float fadeDuration);

        /******************************************************************/
        /*!
        \fn      bool m_SetLooping(const std::string& entityId, bool loop)
        \brief   Sets whether a sound should loop continuously.
        \param   entityId - The ID of the entity for which to set looping.
        \param   loop - True to enable looping, false to disable.
        \return  True if looping is set successfully, false otherwise.
        */
        /******************************************************************/
        bool m_SetLooping(const std::string& entityId, bool loop);

        /******************************************************************/
        /*!
        \fn      bool m_SetVolume(const std::string& entityId, float volume)
        \brief   Sets the volume for a specific sound.
        \param   entityId - The ID of the entity for which to set the volume.
        \param   volume - Desired volume level (0.0 to 1.0).
        \return  True if volume is set successfully, false otherwise.
        */
        /******************************************************************/
        bool m_SetVolume(const std::string& entityId, float volume);

        /******************************************************************/
        /*!
        \fn      bool m_SetPan(const std::string& entityId, float pan)
        \brief   Sets the pan (stereo balance) for a specific sound.
        \param   entityId - The ID of the entity for which to set the pan.
        \param   pan - Stereo balance value (-1.0 for left, 1.0 for right).
        \return  True if pan is set successfully, false otherwise.
        */
        /******************************************************************/
        bool m_SetPan(const std::string& entityId, float pan);  

        /******************************************************************/
        /*!
        \fn      bool m_IsPlaying(const std::string& entityId)
        \brief   Checks if the sound associated with the given entity ID is currently playing.
        \param   entityId - The ID of the entity for which to check the sound.
        \return  True if the sound is playing, false otherwise.
        */
        /******************************************************************/
        bool m_IsPlaying(const std::string& entityId);

        /******************************************************************/
        /*!
        \fn      FMOD::Channel* m_GetChannelForEntity(ecs::EntityID entityId)
        \brief   Retrieves the FMOD channel associated with the given entity ID.
        \param   entityId - The ID of the entity.
        \return  Pointer to the FMOD channel associated with the entity.
        */
        /******************************************************************/
        FMOD::Channel* m_GetChannelForEntity(ecs::EntityID entityId);

    private:
        /******************************************************************/
        /*!
        \var     FMOD::System* m_system
        \brief   FMOD system object to manage sound playback.
        */
        /******************************************************************/
        FMOD::System* m_system;

        /******************************************************************/
        /*!
        \var     FMOD::Sound* m_sound
        \brief   FMOD sound object to represent the loaded audio.
        */
        /******************************************************************/
        FMOD::Sound* m_sound;

        /******************************************************************/
        /*!
        \var     std::unordered_map<std::string, FMOD::Channel*> m_entityChannels
        \brief   Map of entity ID to its playing FMOD channel.
        */
        /******************************************************************/
        std::unordered_map<std::string, FMOD::Channel*> m_entityChannels;

        struct ChannelInfo {
            FMOD::Channel* channel;
            std::string entityId;
            bool isActive;
            float priority;
            std::chrono::steady_clock::time_point lastUsed;

            bool isFading = false;
            bool isFadingOut = false; // Specifically for stopping sounds
            std::chrono::steady_clock::time_point fadeStartTime;
            std::chrono::steady_clock::time_point fadeCompleteTime;
            float fadeDuration = 0.0f;
            float fadeStartVolume = 1.0f;
            float fadeTargetVolume = 1.0f;

            ChannelInfo() : channel(nullptr), isActive(false), priority(0.0f) {}
        };

        static const int MAX_CHANNELS = 64;
        std::vector<ChannelInfo> m_channelPool;

        void m_InitializeChannelPool();
        FMOD::Channel* m_GetAvailableChannel(const std::string& entityId);
        void m_ReleaseUnusedChannels();
        int m_FindLeastImportantChannel() const;

        //For Debugging
        int m_GetActiveChannelCount() const;
        void m_PrintChannelStatus() const;  // For debugging
    };



    class AudioManager {
    public:

        /******************************************************************/
        /*!
        \fn      AudioManager()
        \brief   Constructor that initializes the audio manager.
        */
        /******************************************************************/
        AudioManager();

        /******************************************************************/
        /*!
        \fn      ~AudioManager()
        \brief   Destructor that cleans up audio resources.
        */
        /******************************************************************/
        ~AudioManager();

        /******************************************************************/
        /*!
        \fn      void m_LoadAudio(const std::string& name, const std::string& path)
        \brief   Loads an audio file into the sound map.
        \param   name - Identifier for the audio asset.
        \param   path - Path to the audio file.
        */
        /******************************************************************/
        void m_LoadAudio(const std::string& name, const std::string& path);

        /******************************************************************/
        /*!
        \fn      void m_PlayAudioForEntity(ecs::EntityID entityId, const std::string& name, float volume)
        \brief   Plays audio for the specified entity with the given volume.
        \param   entityId - ID of the entity to play audio for.
        \param   name - Name of the audio asset.
        \param   volume - Volume level to play the audio at.
        */
        /******************************************************************/
        void m_PlayAudioForEntity(ecs::EntityID entityId, const std::string& name, float volume);
        
        /******************************************************************/
        /*!
        \fn      void m_StopAudioForEntity(ecs::EntityID entityId, const std::string& name)
        \brief   Stops the audio associated with the given entity.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        */
        /******************************************************************/
        void m_StopAudioForEntity(ecs::EntityID entityId, const std::string& name);

        /******************************************************************/
        /*!
        \fn      void m_PauseAudioForEntity(ecs::EntityID entityId, const std::string& name)
        \brief   Pauses the audio associated with the given entity.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        */
        /******************************************************************/
        void m_PauseAudioForEntity(ecs::EntityID entityId, const std::string& name);

        /******************************************************************/
        /*!
        \fn      void m_UnpauseAudioForEntity(ecs::EntityID entityId, const std::string& name)
        \brief   Unpauses the audio associated with the given entity.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        */
        /******************************************************************/
        void m_UnpauseAudioForEntity(ecs::EntityID entityId, const std::string& name);

        /******************************************************************/
        /*!
        \fn      void m_SetVolumeForEntity(ecs::EntityID entityId, const std::string& name, float volume)
        \brief   Sets the volume for the given entity's audio.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \param   volume - Desired volume level (0.0 to 1.0).
        */
        /******************************************************************/
        void m_SetVolumeForEntity(ecs::EntityID entityId, const std::string& name, float volume);

        /******************************************************************/
        /*!
        \fn      void m_SetLoopingForEntity(ecs::EntityID entityId, const std::string& name, bool loop)
        \brief   Sets looping for the audio associated with the given entity.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \param   loop - True to enable looping, false to disable.
        */
        /******************************************************************/
        void m_SetLoopingForEntity(ecs::EntityID entityId, const std::string& name, bool loop);

        /******************************************************************/
        /*!
        \fn      void m_SetIsBGMForEntity(ecs::EntityID entityId, const std::string& name, bool isBGM)
        \brief   Sets whether the audio is categorized as background music (BGM) for a specific entity.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \param   isBGM - True if the audio is BGM, false otherwise.
        */
        /******************************************************************/
        void m_SetIsBGMForEntity(ecs::EntityID entityId, const std::string& name, bool isBGM);

        /******************************************************************/
        /*!
        \fn      void m_SetIsSFXForEntity(ecs::EntityID entityId, const std::string& name, bool isSFX)
        \brief   Sets whether the audio is categorized as sound effects (SFX) for a specific entity.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \param   isSFX - True if the audio is SFX, false otherwise.
        */
        /******************************************************************/
        void m_SetIsSFXForEntity(ecs::EntityID entityId, const std::string& name, bool isSFX);

        /******************************************************************/
        /*!
        \fn      void m_SetGlobalBGMVolume(float volume)
        \brief   Sets the global volume level for all background music (BGM).
        \param   volume - Desired global BGM volume (0.0 to 1.0).
        */
        /******************************************************************/
        void m_SetGlobalBGMVolume(float volume);

        /******************************************************************/
        /*!
        \fn      void m_SetGlobalSFXVolume(float volume)
        \brief   Sets the global volume level for all sound effects (SFX).
        \param   volume - Desired global SFX volume (0.0 to 1.0).
        */
        /******************************************************************/
        void m_SetGlobalSFXVolume(float volume);

        /******************************************************************/
        /*!
        \fn      bool m_CheckIsBGMForEntity(ecs::EntityID entityId, const std::string& name)
        \brief   Checks if the specified audio for the entity is marked as background music (BGM).
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \return  True if the audio is BGM, false otherwise.
        */
        /******************************************************************/
        bool m_CheckIsBGMForEntity(ecs::EntityID entityId, const std::string& name);

        /******************************************************************/
        /*!
        \fn      bool m_CheckIsSFXForEntity(ecs::EntityID entityId, const std::string& name)
        \brief   Checks if the specified audio for the entity is marked as sound effects (SFX).
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \return  True if the audio is SFX, false otherwise.
        */
        /******************************************************************/
        bool m_CheckIsSFXForEntity(ecs::EntityID entityId, const std::string& name);

        /******************************************************************/
        /*!
        \fn      void m_SetPlayOnStartForEntity(ecs::EntityID entityId, const std::string& audioName, bool playOnStart)
        \brief   Sets whether audio should play on start for the given entity.
        \param   entityId - ID of the entity.
        \param   audioName - Name of the audio asset.
        \param   playOnStart - True to play on start, false otherwise.
        */
        /******************************************************************/
        void m_SetPlayOnStartForEntity(ecs::EntityID entityId, const std::string& audioName, bool playOnStart);

        /******************************************************************/
        /*!
        \fn      bool m_IsPlayingForEntity(ecs::EntityID entityId, const std::string& name)
        \brief   Checks if the specified entity's audio is playing.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \return  True if the audio is playing, false otherwise.
        */
        /******************************************************************/
        bool m_IsPlayingForEntity(ecs::EntityID entityId, const std::string& name);

        /******************************************************************/
        /*!
        \fn      void m_SetPanForEntity(ecs::EntityID entityId, const std::string& name, float pan)
        \brief   Sets the pan for the specified entity's audio.
        \param   entityId - ID of the entity.
        \param   name - Name of the audio asset.
        \param   pan - Stereo balance (-1.0 for left, 1.0 for right).
        */
        /******************************************************************/
        void m_SetPanForEntity(unsigned int entityId, const std::string& name, float pan);
        
        /******************************************************************/
        /*!
        \fn      void m_PauseAllSounds()
        \brief   Pauses all audio components managed by the audio manager.
        */
        /******************************************************************/
        void m_PauseAllSounds();

        /******************************************************************/
        /*!
        \fn      void m_UnpauseAllSounds()
        \brief   Unpauses all audio components managed by the audio manager.
        */
        /******************************************************************/
        void m_UnpauseAllSounds();

        /******************************************************************/
        /*!
        \fn      void m_StopAllSounds()
        \brief   Stops all audio components managed by the audio manager.
        */
        /******************************************************************/
        void m_StopAllSounds();

        /******************************************************************/
        /*!
        \fn      std::unordered_map<std::string, std::unique_ptr<fmodaudio::FModAudio>>& getSoundMap()
        \brief   Retrieves the map of sound names to their respective FModAudio objects.
        \return  Reference to the unordered map containing sound names and FModAudio objects.
        */
        /******************************************************************/
        std::unordered_map<std::string, std::unique_ptr<FModAudio>>& getSoundMap() {
            return m_soundMap;
        }

        FMOD::System* GetSystem() {
            return m_system; 
        }

    public:
        float m_GlobalBGMVolume{ 0.5f };
        float m_GlobalSFXVolume{ 0.5f };

    private:

        /******************************************************************/
        /*!
        \var     std::unordered_map<std::string, std::unique_ptr<FModAudio>> m_soundMap
        \brief   Map of sound names to their respective FModAudio objects.
        */
        /******************************************************************/
        std::unordered_map<std::string, std::unique_ptr<FModAudio>> m_soundMap;

        FMOD::System* m_system;  // Single global FMOD system
    };

}
#endif // AUDIO_MANAGER_H
