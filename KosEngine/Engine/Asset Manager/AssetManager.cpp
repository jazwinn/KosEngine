/******************************************************************/
/*!
\file      AssetManager.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 27, 2024
\brief     This file contains the definition of the AssetManager class,
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

#include "AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Helper/Helper.h"
#include "SceneManager.h"
#include "stb_image.h"
#include "../ECS/ECS.h"




namespace assetmanager {

    std::unique_ptr<AssetManager> AssetManager::m_instancePtr = nullptr;

    void AssetManager::m_funcLoadAssets(std::string Directory )
    {
        for (auto& directoryPath : std::filesystem::directory_iterator(Directory)) {
            std::string filepath = directoryPath.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');

            if (directoryPath.is_directory()) {
                m_funcLoadAssets(filepath);
            }
            else {
                m_LoadAsset(directoryPath.path());
            }
        }


    }

    void AssetManager::m_LoadAsset(std::filesystem::path directoryPath)
    {
        std::string filepath = directoryPath.string();
        std::replace(filepath.begin(), filepath.end(), '\\', '/');

        if (directoryPath.filename().extension().string() == ".png" || directoryPath.filename().extension().string() == ".jpg") {
            LOGGING_INFO("Filepath: {}", filepath.c_str());
            m_funcLoadImage(filepath);

        }
        else if (directoryPath.filename().extension().string() == ".wav") {

            m_LoadAudio(filepath.c_str());
        }
        else if (directoryPath.filename().extension().string() == ".ttf") {
            //UNCOMMENT ME TO TEST IT OUT
            m_LoadFont(filepath);
        }
        else if (directoryPath.filename().extension().string() == ".cs") {
            m_LoadScript(directoryPath);
        }
        if (directoryPath.filename().extension().string() == ".prefab") {

            scenes::SceneManager::m_GetInstance()->m_LoadScene(directoryPath);
        }
        if (directoryPath.filename().extension().string() == ".json") {

            scenes::SceneManager::m_GetInstance()->m_unloadScenePath.insert(std::pair{ directoryPath.filename().string(), directoryPath});

        }
        if (directoryPath.filename().extension().string() == ".mpg" || directoryPath.filename().extension().string() == ".mpeg") {

            m_videoManager.m_videopath[directoryPath.filename().string()] = directoryPath;

        }
        if (directoryPath.filename().extension().string() == ".mp3") {

            LOGGING_POPUP("File Type: " + directoryPath.filename().extension().string() + " not supported");

        }

    }

    AssetManager* AssetManager::m_funcGetInstance()
    {
        if (!m_instancePtr)
        {
            m_instancePtr.reset(new AssetManager{});
        }
        return m_instancePtr.get();
    }

    AssetManager::~AssetManager()
    {
        
    }

    void AssetManager::m_LoadPrefab(std::string file) {
       // prefab::Prefab::m_DeSerializePrefab(file);

    }

   

    void AssetManager::m_LoadScript(std::filesystem::path filepath)
    {

        std::string filename = filepath.filename().stem().string();
        
        if (std::find_if(m_scriptManager.m_CSScripts.begin(), m_scriptManager.m_CSScripts.end(), [&filename](const std::pair<std::string, std::filesystem::path>& scriptpair) {return scriptpair.first == filename ;}) == m_scriptManager.m_CSScripts.end()) {
            //store if filename is not inside
            m_scriptManager.m_CSScripts.push_back(std::pair{filename, filepath});
        }

    }

    void AssetManager::m_LoadAudio(const std::string& file) {
        const std::set<std::string> supportedFormats = { ".wav", ".ogg" };

        if (!std::filesystem::exists(file)) {
           // std::cerr << "Error: Audio file not found: " << file << "\n";
            return;
        }

        std::string extension = file.substr(file.find_last_of('.'));
        if (supportedFormats.find(extension) == supportedFormats.end()) {
            //std::cerr << "Error: Unsupported audio format: " << extension << "\n";
            LOGGING_ERROR("Unsupported audio format: " + extension);
            return;
        }

        std::string fileName = file;
        size_t lastSlashPos = fileName.find_last_of("/\\");
        size_t lastDotPos = fileName.find_last_of('.');

        if (lastSlashPos == std::string::npos) lastSlashPos = -1;
        if (lastDotPos == std::string::npos || lastDotPos < lastSlashPos) lastDotPos = fileName.length();

        fileName = file.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);

        //check if audio is already loaded
        const auto& audiomap = m_audioManager.getSoundMap();
        if (audiomap.find(fileName) != audiomap.end()) {
            LOGGING_WARN("Sound Already Loaded");
            return;
        }


        m_audioManager.m_LoadAudio(fileName, file);
    }


   void AssetManager::m_LoadFont(std::string file)
   {
       std::filesystem::path filepath = file;
       std::string filename = filepath.filename().string();
       if (m_fontManager.m_fonts.find(filename) == m_fontManager.m_fonts.end()) {
           text::FontManager::LoadFont(file);
       }
       else {
           LOGGING_WARN("Font Already Loaded");
       }
   }
 

    void AssetManager::m_funcLoadImage(std::string file)
    {
        std::filesystem::path path = file;
        if (m_imageManager.m_imageMap.find(path.filename().string()) == m_imageManager.m_imageMap.end()) {
            m_imageManager.m_LoadImage(file.c_str());
        }
        else {
            LOGGING_WARN("Image Already Loaded");
        }
        
    }

    void AssetManager::m_RenameAsset(std::filesystem::path oldfilepath, std::filesystem::path newfilepath)
    {
        if (oldfilepath == newfilepath) {
            return;
        }


        if (oldfilepath.filename().extension().string() != newfilepath.filename().extension().string()) {
            LOGGING_WARN("Renaming assets of different type");
            return;
        }

        // find the file extension (Update for every new file type
        if (oldfilepath.extension().string() == ".png" || oldfilepath.extension().string() == ".jpg") {
            //check if old file name is present
            if (m_imageManager.m_imageMap.find(oldfilepath.filename().string()) != m_imageManager.m_imageMap.end()) {
                //check if new file name is non existent
                if (m_imageManager.m_imageMap.find(newfilepath.filename().string()) == m_imageManager.m_imageMap.end()) {

                    //create new map of updated name
                    m_imageManager.m_imageMap[newfilepath.filename().string()] = m_imageManager.m_imageMap.find(oldfilepath.filename().string())->second;
                    //delete old name map
                    m_imageManager.m_imageMap.erase(oldfilepath.filename().string());
                }
                else {
                    return;
                }
            }
            else {
                return;
            }
        }
        if (oldfilepath.extension().string() == ".prefab" || oldfilepath.extension().string() == ".json") {
            ecs::ECS* ecs = ecs::ECS::m_GetInstance();
            scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
            const auto& prefab = ecs->m_ECS_SceneMap.find(oldfilepath.filename().string());
            if (prefab != ecs->m_ECS_SceneMap.end()) {
                if (ecs->m_ECS_SceneMap.find(newfilepath.filename().string()) == ecs->m_ECS_SceneMap.end()) {
                    ecs->m_ECS_SceneMap[newfilepath.filename().string()] = ecs->m_ECS_SceneMap.find(oldfilepath.filename().string())->second;
                    ecs->m_ECS_SceneMap.erase(oldfilepath.filename().string());

                    if (scenemanager->m_loadScenePath.find(oldfilepath.filename().string()) != scenemanager->m_loadScenePath.end()) {
                        scenemanager->m_loadScenePath[newfilepath.filename().string()] = newfilepath;
                        scenemanager->m_loadScenePath.erase(oldfilepath.filename().string());
                    }

                    //update entityid component with new scene name
                    for (const auto id : ecs->m_ECS_SceneMap.find(newfilepath.filename().string())->second.m_sceneIDs) {
                        scenemanager->m_AssignEntityNewSceneName(newfilepath.filename().string(), id);
                    }


                }
                else {
                    return;
                }
            }
            else {
                return;
            }
        }
        if (oldfilepath.extension().string() == ".wav") {
            auto& soundmap = m_audioManager.getSoundMap();
            if (soundmap.find(oldfilepath.filename().stem().string()) != soundmap.end()) {
                if (soundmap.find(newfilepath.filename().stem().string()) == soundmap.end()) {

                    soundmap[newfilepath.filename().stem().string()] = std::move(soundmap.find(oldfilepath.filename().stem().string())->second);
                    soundmap.erase(oldfilepath.filename().stem().string());
                }
                else {
                    return;
                }
            }
            else {
                return;
            }
        }
        if (oldfilepath.extension().string() == ".ttf") {
            //check if old file name is present
            auto& textmap = m_fontManager.m_fonts;
            if (textmap.find(oldfilepath.filename().string()) != textmap.end()) {
                //check if new file name is non existent
                if (textmap.find(newfilepath.filename().string()) == textmap.end()) {

                    //create new map of updated name
                    textmap[newfilepath.filename().string()] = std::move(textmap.find(oldfilepath.filename().string())->second);
                    //delete old name map
                    textmap.erase(oldfilepath.filename().string());
                }
                else {
                    return;
                }
            }
            else {
                return;
            }
        }
        if (oldfilepath.extension().string() == ".cs") {
            LOGGING_POPUP("No Renaming of scripts, class name must be the same as filename");
            return;
            //check if old file name is present
            //const auto& it = std::find_if(m_scriptManager.m_CSScripts.begin(), m_scriptManager.m_CSScripts.end(), [oldfilepath](const auto& script) {return script.first == oldfilepath.filename(); });
            //if (it != m_scriptManager.m_CSScripts.end()) {
            //    m_scriptManager.m_CSScripts.erase(it);
            //    m_scriptManager.m_CSScripts.push_back(std::pair{ newfilepath.filename().string(), newfilepath });
            //}
        }

        std::filesystem::rename(oldfilepath.c_str(), newfilepath.c_str());
    }
}