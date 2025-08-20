/******************************************************************/
/*!
\file      AssetManager.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 27, 2024
\brief     This file contains the declaration of the AssetManager class,
		   which handles loading, storing, and managing various game assets.

The AssetManager class provides functions for loading and managing
images, audio files, fonts, prefabs, and entity data. It serves as a
singleton, ensuring only one instance manages the resources for the
entire game. Assets are stored in containers for later retrieval and use.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef ASSETMGR_H
#define ASSETMGR_H


#include <../GLEW/include/GL/glew.h>
#include "Audio.h"
#include "Prefab.h"
#include "Image.h"
#include "Text.h"
#include "Video.h"
#include "../C#Mono/mono_handler.h"
#include "../C#Mono/InternalCall.h"


namespace assetmanager {

    /******************************************************************/
    /*!
    \class   AssetManager
    \brief   A singleton class that manages the loading, storage, and
             retrieval of game assets, including images, audio, fonts,
             and prefabs.
    */
    /******************************************************************/
    class AssetManager {
    private:
        /******************************************************************/
        /*!
        \var     static std::unique_ptr<AssetManager> m_instancePtr
        \brief   Unique pointer to the singleton instance of AssetManager.
        */
        /******************************************************************/
        static std::unique_ptr<AssetManager> m_instancePtr;

    public:
        /******************************************************************/
        /*!
        \fn      static AssetManager* m_funcGetInstance()
        \brief   Retrieves the singleton instance of AssetManager. If the
                 instance does not exist, it creates one.
        \return  Pointer to the singleton instance of AssetManager.
        */
        /******************************************************************/
        static AssetManager* m_funcGetInstance();

        /******************************************************************/
        /*!
        \fn      ~AssetManager()
        \brief   Destructor that cleans up any dynamically allocated assets.
        */
        /******************************************************************/
        ~AssetManager();

        /******************************************************************/
        /*!
        \fn      void m_funcLoadAssets()
        \brief   Loads all necessary assets (images, audio, fonts, prefabs)
                 required by the game.
        */
        /******************************************************************/
        void m_funcLoadAssets(std::string );

        /******************************************************************/
        /*!
        \fn      void m_LoadAsset(std::filesystem::path filepath)
        \brief   Loads an asset from the specified file path.
        \param   filepath - Path to the asset file to load.
        */
        /******************************************************************/
        void m_LoadAsset(std::filesystem::path);

        /******************************************************************/
        /*!
        \fn      void m_funcLoadImage(const char* file)
        \brief   Loads an image file and stores it in the image container.
        \param   file - Path to the image file to load.
        */
        /******************************************************************/
        void m_funcLoadImage(std::string file);

        /******************************************************************/
        /*!
        \fn      void m_LoadAudio(std::string file)
        \brief   Loads an audio file and stores it in the audio container.
        \param   file - Path to the audio file to load.
        */
        /******************************************************************/
        void m_LoadAudio(const std::string& file);

        /******************************************************************/
        /*!
        \fn      void m_LoadFont(std::string file)
        \brief   Loads a font file and stores character data in the font map.
        \param   file - Path to the font file to load.
        */
        /******************************************************************/
        void m_LoadFont(std::string file);

        /******************************************************************/
        /*!
        \fn      void m_LoadPrefab(std::string file)
        \brief   Loads a prefab file and stores it in the prefab map.
        \param   file - Path to the prefab file to load.
        */
        /******************************************************************/
        void m_LoadPrefab(std::string file);

        /******************************************************************/
        /*!
        \fn      void m_RenameAsset(std::filesystem::path old, std::filesystem::path _new)
        \brief   Renames an existing asset file from the old path to the new path.
        \param   old - Path to the existing asset file.
        \param   _new - New path for the asset file.
        */
        /******************************************************************/
        void m_RenameAsset(std::filesystem::path old, std::filesystem::path _new);

        /******************************************************************/
        /*!
        \fn      void m_LoadScript(std::filesystem::path filepath)
        \brief   Loads a script file and adds it to the script manager.
        \param   filepath - Path to the script file to load.
        */
        /******************************************************************/

        void m_LoadScript(std::filesystem::path filepath);
        /******************************************************************/
        /*!
        \var     fmodaudio::AudioManager m_audioManager
        \brief   Manages the loading and playback of audio assets.
        */
        /******************************************************************/
        fmodaudio::AudioManager m_audioManager;

        /******************************************************************/
        /*!
        \var     image::ImageManager m_imageManager
        \brief   Manages the loading and storage of image assets.
        */
        /******************************************************************/
        image::ImageManager m_imageManager;

        /******************************************************************/
        /*!
        \var     script::ScriptHandler m_scriptManager
        \brief   Handles the loading and management of scripts.
        */
        /******************************************************************/
        script::ScriptHandler m_scriptManager;

        video::VideoManager m_videoManager;

        /******************************************************************/
        /*!
        \var     std::unordered_map<std::string, prefab::Prefab> m_prefabs
        \brief   Map storing prefab assets by string identifier.
        */
        /******************************************************************/
        std::unordered_map<std::string, prefab::Prefab> m_prefabs;

        /******************************************************************/
        /*!
        \var     Font m_characters
        \brief   Map storing font character data.
        */
        /******************************************************************/

        text::FontManager m_fontManager;
        
    };

}

#endif // ASSETMGR_H