#pragma once
/******************************************************************/
/*!
\file      AudioComponent.h
\author    Clarence Boey, c.boey, 2301547
\par       c.boey@digipen.edu
\date      Nov 19, 2024
\brief     This file contains the audio data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef AUDIOCOM_H
#define AUDIOCOM_H

#include "Component.h"
#include <string>

namespace ecs {

    struct AudioFile {
        std::string m_Name;
        std::string m_FilePath;
        float m_Volume{ 1.0f };
        bool m_Loop{ false };
        bool m_PlayOnStart{ false };
        bool m_HasPlayed{ false };
        float m_Pan{ 0.0f };
        bool m_IsBGM{ false };
        bool m_IsSFX{ true };
        bool m_hasChanged = false;


        float m_LastVolume{ -1.0f };
        bool m_LastLoopState{ false };

        REFLECTABLE(AudioFile, m_FilePath, m_Volume, m_Loop, m_PlayOnStart, m_HasPlayed, m_Pan, m_IsBGM, m_IsSFX)
    };

    class AudioComponent : public Component {
    public:
        std::vector<AudioFile> m_AudioFiles;
        REFLECTABLE(AudioComponent, m_AudioFiles)
    };
}
#endif AUDIOCOM_H