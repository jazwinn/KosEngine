/********************************************************************/
/*!
\file      SceneManager.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file declares the definitions for the `SceneManager` class,
		   which handles the creation, loading, saving, clearing, and management
		   of scenes within the ECS framework.
		   - m_CreateNewScene: Creates a new JSON file for a scene.
		   - m_LoadScene: Loads entities from a JSON file into the ECS system.
		   - m_ReloadScene: Reloads all active scenes.
		   - m_ClearAllScene: Clears all non-prefab scenes.
		   - m_ClearScene: Removes all entities from a specified scene.
		   - m_SaveScene: Saves the current state of a specified scene to a JSON file.
		   - m_SaveAllActiveScenes: Saves all active scenes.
		   - m_SwapScenes: Moves an entity from one scene to another.
		   - GetSceneByEntityID: Finds the scene that contains a specified entity.

This file supports scene management by providing functions for creating, saving,
loading, and clearing scenes within a game, allowing dynamic control of game states.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include "../Config/pch.h"
#include "../De&Serialization/json_handler.h"

namespace scenes {

	class SceneManager {
	public:
		static SceneManager* m_GetInstance() {
			if (!m_InstancePtr) {
				m_InstancePtr.reset(new SceneManager{});
			}
			return m_InstancePtr.get();
		}

		/******************************************************************/
		/*!
			\fn        bool m_CreateNewScene(std::filesystem::path scenepath)
			\brief     Creates a new scene JSON file at the specified path.
			\param[in] scenepath The file path where the new scene JSON will be created.
			\return    True if the scene file is successfully created; otherwise, false.
			\details   Checks for an existing file at the given path, creates a new JSON
					   file for the scene if none exists, and initializes it for use.
		*/
		/******************************************************************/
		bool m_CreateNewScene(std::filesystem::path scenepath);

		/******************************************************************/
		/*!
			\fn        void m_LoadScene(std::filesystem::path scenepath)
			\brief     Loads a scene from the specified JSON file path.
			\param[in] scenepath The file path of the scene JSON to load.
			\details   Loads the scene's entities and components into the ECS system, and
					   registers the scene path and details in the scene map.
		*/
		/******************************************************************/
		void m_LoadScene(std::filesystem::path scenepath);

		/******************************************************************/
		/*!
			\fn        void m_ReloadScene()
			\brief     Reloads all currently active scenes in the ECS system.
			\details   Clears all current scene data, then reloads each scene from its
					   previously stored path, restoring the scene states.
		*/
		/******************************************************************/
		void m_ReloadScene();

		/******************************************************************/
		/*!
			\fn        void m_ClearAllScene()
			\brief     Clears all active scenes except those marked as prefabs.
			\details   Removes entities and components associated with each non-prefab scene,
					   leaving prefabs untouched in the ECS scene map.
		*/
		/******************************************************************/
		void m_ClearAllScene();

		/******************************************************************/
		/*!
			\fn        void m_ClearScene(std::string scene)
			\brief     Clears all entities from a specified scene.
			\param[in] scene The name of the scene to clear.
			\details   Deletes each entity within the specified scene, then removes the scene
					   from the active scene list in the ECS system.
		*/
		/******************************************************************/
		void m_ClearScene(std::string scene);

		/******************************************************************/
		/*!
			\fn        void m_SaveScene(std::string scene)
			\brief     Saves the current state of a specified scene to its JSON file.
			\param[in] scene The name of the scene to save.
			\details   Serializes the scene’s entities and components to its associated
					   JSON file, preserving the current state of the scene.
		*/
		/******************************************************************/
		void m_SaveScene(std::string scene);

		/******************************************************************/
		/*!
			\fn        void m_SaveAllActiveScenes()
			\brief     Saves all active scenes in the ECS system to their respective JSON files.
			\details   Iterates over all active scenes and calls m_SaveScene for each,
					   ensuring that all scenes are up-to-date.
		*/
		/******************************************************************/
		void m_SaveAllActiveScenes(bool includeprefab = false);

		/******************************************************************/
		/*!
			\fn        void m_SwapScenes(std::string oldscene, std::string newscene, ecs::EntityID id)
			\brief     Moves a specified entity from one scene to another.
			\param[in] oldscene The name of the scene from which to remove the entity.
			\param[in] newscene The name of the scene to which the entity will be added.
			\param[in] id       The ID of the entity to move.
			\details   Transfers the entity’s data and components from one scene to another,
					   updating its scene association in the ECS system.
		*/
		/******************************************************************/
		void m_SwapScenes(std::string oldscene, std::string newscene , ecs::EntityID id);

		/******************************************************************/
		/*!
		\fn      void m_AssignEntityNewSceneName(const std::string& scene, ecs::EntityID id)
		\brief   Assigns a new scene name to a specific entity.
		\param   scene - Name of the new scene to assign.
		\param   id - Entity ID to assign to the new scene.
		*/
		/******************************************************************/
		void m_AssignEntityNewSceneName(const std::string& scene, ecs::EntityID id);

		/******************************************************************/
		/*!
			\fn        static std::optional<std::string> GetSceneByEntityID(ecs::EntityID entityID)
			\brief     Retrieves the name of the scene containing a specified entity.
			\param[in] entityID The ID of the entity to search for.
			\return    An optional containing the name of the scene, or nullopt if no match is found.
			\details   Searches through each loaded scene to find the one containing the given entity ID.
		*/
		/******************************************************************/
		static std::optional<std::string> GetSceneByEntityID(ecs::EntityID entityID);




		/******************************************************************/
		/*!
		\var     std::unordered_map<std::string, std::filesystem::path> m_unloadScenePath
		\brief   Stores the paths of scenes that have been unloaded, mapped by scene name.
		*/
		/******************************************************************/
		std::unordered_map<std::string, std::filesystem::path> m_unloadScenePath;

		/******************************************************************/
		/*!
		\var     std::unordered_map<std::string, std::filesystem::path> m_loadScenePath
		\brief   Stores the paths of scenes that have been loaded, mapped by scene name.
		*/
		/******************************************************************/
		std::unordered_map<std::string, std::filesystem::path> m_loadScenePath;

		/******************************************************************/
		/*!
		\var     std::vector<std::filesystem::path> m_recentFiles
		\brief   Stores the paths of recently used scene files.
		*/
		/******************************************************************/
		std::vector<std::filesystem::path> m_recentFiles;

		bool isSceneChanged{};
		//std::string m_AddScene(std::string filepath);

	private:

		/******************************************************************/
		/*!
		\var     static std::unique_ptr<SceneManager> m_InstancePtr
		\brief   Unique pointer to the singleton instance of SceneManager.
		*/
		/******************************************************************/
		static std::unique_ptr<SceneManager> m_InstancePtr;

	};
}
#endif SCENE_H
