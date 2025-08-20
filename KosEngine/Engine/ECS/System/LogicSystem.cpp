/******************************************************************/
/*!
\file      LogicSystem.h
\author    Ng Jaz Winn
\par       jazwinn.ng@digipen.edu
\date      Nov 30, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../ECS.h"

#include "LogicSystem.h"
#include "../Asset Manager/AssetManager.h"



namespace ecs {

	void LogicSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();
		if (std::find_if(m_vecScriptComponentPtr.begin(), m_vecScriptComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecScriptComponentPtr.end()) {
			m_vecScriptComponentPtr.push_back((ScriptComponent*)ecs->m_ECS_CombinedComponentPool[TYPESCRIPTCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void LogicSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecScriptComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}


		//free script handler 
		for (int x : m_vecScriptComponentPtr[IndexID]->m_scriptHandler) {
			mono_gchandle_free(x);
		}
		

		for (auto& scriptname : m_vecScriptComponentPtr[IndexID]->m_scripts) {

			auto script = m_vecScriptComponentPtr[IndexID]->m_scriptInstances.find(std::get<0>(scriptname));

			if (!(script == m_vecScriptComponentPtr[IndexID]->m_scriptInstances.end())) {
				script->second.first = nullptr;
			}
			
			
		}


		//index to the last element
		size_t IndexLast = m_vecScriptComponentPtr.size() - 1;
		std::swap(m_vecScriptComponentPtr[IndexID], m_vecScriptComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);
		//popback the vector;
		m_vecScriptComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void LogicSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPESCRIPTCOMPONENT);

	}

	void LogicSystem::m_StartLogic()
	{
		ECS* ecs = ECS::m_GetInstance();

		//reset all script instance
		std::shared_ptr<ComponentPool<ScriptComponent>> pool = std::dynamic_pointer_cast<ComponentPool<ScriptComponent>>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]);
		std::for_each(pool->m_Pool.begin(), pool->m_Pool.end(), [](auto& x) {x.m_scriptInstances.clear(); });
		
		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecScriptComponentPtr.size(); n++) {
			ScriptComponent* scriptComp = m_vecScriptComponentPtr[n];

			//skip prefab
			if (ecs->m_ECS_SceneMap.find(scriptComp->m_scene)->second.m_isPrefab)continue;

			CreateandStartScriptInstance(scriptComp);

		} 
	}


	void LogicSystem::CreateandStartScriptInstance(ecs::ScriptComponent* scriptComp) {
		// create instance for each script

		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		for (const auto& _script : scriptComp->m_scripts) {

			//if (assetManager->m_scriptManager.m_ScriptMap.find(_script) == assetManager->m_scriptManager.m_ScriptMap.end()) {
			//	LOGGING_ERROR("SCRIPT NOT FOUND ! PLEASE RELAUNCH APPLIATION");
			//	continue;
			//}

			// retieve isntance for each object
			//
			//  << _script << std::endl;
			auto instance = assetManager->m_scriptManager.m_CreateObjectInstance("LogicScript", std::get<0>(_script));
			scriptComp->m_scriptInstances[std::get<0>(_script)] = std::make_pair(instance, false);
			scriptComp->m_scriptHandler.push_back(mono_gchandle_new(instance, true));

			

			//assign instance with script varaibles
			assetManager->m_scriptManager.m_assignVaraiblestoScript(scriptComp, std::get<0>(_script));
		}

		// invoke start function
		for (auto& instance : scriptComp->m_scriptInstances) {

			void* params[1];
			params[0] = &scriptComp->m_Entity; // Pass the entity ID

			assetManager->m_scriptManager.m_InvokeMethod(instance.first, "Awake", instance.second.first, params);
			

		}
	}

	void LogicSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();
		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		//if (m_vecMovementComponentPtr.size() != m_vecTransformComponentPtr.size()) {
		//	//std::cout << "Error: Vectors container size does not Match" << std::endl;
		//	LOGGING_ERROR("Error: Vectors container size does not Match");
		//	return;
		//}

		


		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecScriptComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			ScriptComponent* scriptComp = m_vecScriptComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((scriptComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;


			//check if scriptcomponent have instance
			for (auto& scriptstring : scriptComp->m_scripts) {

				if (std::find_if(scriptComp->m_scriptInstances.begin(), scriptComp->m_scriptInstances.end(), [&](auto& x){return x.first == std::get<0>(scriptstring);}) == scriptComp->m_scriptInstances.end()) {
					CreateandStartScriptInstance(scriptComp);
					LOGGING_INFO("Script Instance Created");
					break;
				}
			}


			for (auto& scriptname : scriptComp->m_scripts) {

				auto script = scriptComp->m_scriptInstances.find(std::get<0>(scriptname));
				try {
					// run the scripts update fuction
					const auto& scriptIsEnabled = std::find_if(scriptComp->m_scripts.begin(), scriptComp->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script->first;});
					if (scriptIsEnabled == scriptComp->m_scripts.end()) continue;

					if (std::get<1>(*scriptIsEnabled)){

						if (script->second.second == false) {
							assetManager->m_scriptManager.m_InvokeMethod(script->first, "Start", script->second.first, nullptr);
							script->second.second = true;
						}


						else if (script->second.first) {

							// Call the Update function
							assetManager->m_scriptManager.m_InvokeMethod(script->first, "Update", script->second.first, nullptr);
							
						} 

					}
					
				}
				catch (...) {
					return;

				}


			}
		}

		for (int n{}; n < m_vecScriptComponentPtr.size(); n++) { 
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			ScriptComponent* scriptComp = m_vecScriptComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			//skip component not of the scene
			if ((scriptComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;


			for (auto& scriptname : scriptComp->m_scripts) {

				auto script = scriptComp->m_scriptInstances.find(std::get<0>(scriptname));
				if (script == scriptComp->m_scriptInstances.end()) continue;
				try {
					// run the scripts update fuction
					const auto& scriptIsEnabled = std::find_if(scriptComp->m_scripts.begin(), scriptComp->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script->first; });
					if (scriptIsEnabled == scriptComp->m_scripts.end()) continue;

					if (std::get<1>(*scriptIsEnabled)) {
						assetManager->m_scriptManager.m_InvokeMethod(script->first, "LateUpdate", script->second.first, nullptr);

					}

				}
				catch (...) {
					return;
				}


			}
		}

	}



}

