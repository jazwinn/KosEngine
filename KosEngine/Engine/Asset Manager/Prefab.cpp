/******************************************************************/
/*!
\file      Prefab.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file contains the definations for the prefab class.
           It reads a json file and stores all its data. When the prefab
           is called in the game. It creates an entiy and copy
           the prefab data to the new entity



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "Prefab.h"
#include "../De&Serialization/json_handler.h"
#include "../Debugging/Logging.h"
#include "AssetManager.h"
#include "../ECS/Hierachy.h"
#include "../ECS/ECS.h"
#include "SceneManager.h"
#include "../De&Serialization/json_handler.h"

#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/filewritestream.h"
#include "../Dependencies/rapidjson/prettywriter.h"


namespace prefab {


    void AssignPrefabToNameComponent(ecs::EntityID parentid, std::string scenename) {
        const auto& vecChild = ecs::Hierachy::m_GetChild(parentid);
        if (!vecChild.has_value()) return;
        for (auto& childid : vecChild.value()) {
            ecs::ECS* ecs = ecs::ECS::m_GetInstance();
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(childid));
            nc->m_isPrefab = true;
            nc->m_prefabName = scenename;

            if (ecs::Hierachy::m_GetChild(childid).has_value()) {
                AssignPrefabToNameComponent(childid, scenename);
            }
        }
    }

    int Prefab::m_CreatePrefab(std::string prefabscene, std::string insertscene)
    {
        if (prefabscene == insertscene) {
            LOGGING_ERROR("Cannot load onto itself");
            return -1;
        }



        //check if prefabscene exist
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        if (insertscene.empty()) {
            for (auto& scene : ecs->m_ECS_SceneMap) {

                if (scene.second.m_isActive && (!scene.second.m_isPrefab)) {
                    insertscene = scene.first;
                    break;
                }

            }


            
        }

        if (ecs->m_ECS_SceneMap.find(prefabscene) == ecs->m_ECS_SceneMap.end()) {
            LOGGING_ERROR("Prefab not loaded into scene");
            return -1;
        }
       
        std::vector<ecs::EntityID> vecid;

        for (const auto& prefabentity : ecs->m_ECS_SceneMap.find(prefabscene)->second.m_sceneIDs) {
            //duplicate only parents
            if (ecs::Hierachy::m_GetParent(prefabentity).has_value())continue;
            vecid.push_back(ecs->m_DuplicateEntity(prefabentity, insertscene));
        }

        for (auto& id : vecid) {
            //assign parent as prefab
            ecs::ECS* Tempecs = ecs::ECS::m_GetInstance();
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(Tempecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
            nc->m_isPrefab = true;
            nc->m_prefabName = prefabscene;

            const auto& vecChild = ecs::Hierachy::m_GetChild(id);
            if (!vecChild.has_value()) continue;
            AssignPrefabToNameComponent(id, prefabscene);
        
        }

        return vecid[0];
    }

    void Prefab::m_SaveEntitytoPrefab(ecs::EntityID id)
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
        std::string m_jsonFilePath{ "../Assets/Prefabs/" }; //TODO allow drag and drop onto content browser

        std::string filename;

        short count{};
        do {
            if (count > 0) {
                filename = nc->m_entityName + "_" + std::to_string(count) + ".prefab";
            }
            else {
                filename = nc->m_entityName + ".prefab";
            }
            count++;
        } while (ecs->m_ECS_SceneMap.find(filename) != ecs->m_ECS_SceneMap.end());

        std::string path = m_jsonFilePath + filename;
        scenes::SceneManager::m_GetInstance()->m_CreateNewScene(path);


        /*******************************SERIALIZATION START******************************************/

        Serialization::Serialize::m_JsonFileValidation(path);

        // Create JSON object to hold the updated values
        rapidjson::Document doc;
        doc.SetArray();  // Initialize as an empty array

        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        std::unordered_set<ecs::EntityID> savedEntities;  //track saved entities

        //Start saving the entities
        Serialization::Serialize::m_SaveEntity(id, doc, allocator, savedEntities);

        // Write the JSON back to file
        rapidjson::StringBuffer writeBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(writeBuffer);
        doc.Accept(writer);

        std::ofstream outputFile(path);
        if (outputFile) {
            outputFile << writeBuffer.GetString();
            outputFile.close();
        }

        LOGGING_INFO("Save Prefab Successful");

        /*******************************SERIALIZATION END******************************************/


        // load prefab
        scenes::SceneManager::m_GetInstance()->m_LoadScene(path);


    }

    //void Prefab::m_AssignEntitytoPrefab(std::string prefab, ecs::EntityID id)
    //{
    //    ecs::ECS* ecs = ecs::ECS::m_GetInstance();
    //    if (ecs->m_ECS_SceneMap.find(prefab) == ecs->m_ECS_SceneMap.end()) {
    //        LOGGING_WARN("Prefab not loaded");
    //        return;
    //    }
    //    // only store upmost parent id

    //    ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
    //    if (tc->m_haveParent) {
    //        return;
    //    }

    //}

    void Prefab::m_UpdateAllPrefabEntity(std::string prefab)
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        //return if prefab dont exist
        if (ecs->m_ECS_SceneMap.find(prefab) == ecs->m_ECS_SceneMap.end()) {
            LOGGING_ERROR("Prefab do not exist");
        }

        ecs::EntityID scenePrefabID = ecs->m_ECS_SceneMap.find(prefab)->second.m_prefabID;


        //retrieve prefabs children id
        const std::vector<ecs::EntityID>& childrenid = ecs->m_ECS_SceneMap.find(prefab)->second.m_sceneIDs;

        //the most scuff way //MAYBE prefab component?
        for (auto& id : ecs->m_ECS_EntityMap) {
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id.first));
            ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id.first));
            if (nc->m_isPrefab && (nc->m_prefabName == prefab)) {
                //if sync is turn off, skip update
                if (nc->m_syncPrefab == false) continue;


                //skip all prefabs children
                if (std::find(childrenid.begin(), childrenid.end(), id.first) != childrenid.end()) continue;
                if (tc->m_haveParent) {
                    auto* parentnc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(tc->m_parentID));
                    
                    if (parentnc->m_isPrefab && parentnc->m_prefabName == prefab)continue;
                }

                m_UpdatePrefab(scenePrefabID, id.first);

            }


        }

    }

    void Prefab::m_UpdatePrefab(ecs::EntityID sceneprefabID, ecs::EntityID entityid, bool isPrefabChild)
    {
        if (sceneprefabID == entityid) return;

        //update all of entity with prefab
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        ecs::compSignature scenePrefabSignature = ecs->m_ECS_EntityMap.find(sceneprefabID)->second;
        ecs::compSignature entitySignature = ecs->m_ECS_EntityMap.find(entityid)->second;

        //check if current entity have parent and store it
        auto Parent = ecs::Hierachy::m_GetParent(entityid);



        for (size_t n{}; n < ecs::TOTALTYPECOMPONENT; n++) {

            //skip transform component and name componetn
            // if entity is parent and component is transform component, skip
            if ((isPrefabChild == false) && (ecs::ComponentType)n == ecs::TYPETRANSFORMCOMPONENT)continue;

            //if component is namecomponent, only share layer and entity tag 
            if ((ecs::ComponentType)n == ecs::TYPENAMECOMPONENT) {// sync only layers and tagname
                ecs::NameComponent* prefabNc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(sceneprefabID));
                ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entityid));

                nc->m_Layer = prefabNc->m_Layer;
                nc->m_entityTag = prefabNc->m_entityTag;

                continue;
            }

            if (scenePrefabSignature.test((ecs::ComponentType)n)) {

                //save prefab detail
                if ((ecs::ComponentType)n == ecs::TYPETRANSFORMCOMPONENT) {
                    //only duplicate, TRS
                    ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityid));
                    ecs::TransformComponent* prefabtc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(sceneprefabID));

                    tc->m_position = prefabtc->m_position;
                    tc->m_rotation = prefabtc->m_rotation;
                    tc->m_scale = prefabtc->m_scale;

                    continue;
                }

                //test if enityid has that component, if not add it
                if (!entitySignature.test((ecs::ComponentType)n)) {
                    ecs->m_AddComponent((ecs::ComponentType)n, entityid);
                }
                ecs::Component* comp = static_cast<ecs::Component*>(ecs->m_ECS_CombinedComponentPool[(ecs::ComponentType)n]->m_DuplicateComponent(sceneprefabID, entityid));
                comp->m_scene = scenes::SceneManager::GetSceneByEntityID(entityid).value();


            }
            else {
                if (entitySignature.test((ecs::ComponentType)n)) {
                    ecs->m_RemoveComponent((ecs::ComponentType)n, entityid);
                }
            }
        }


        //checks if duplicates entity has parent and assign it
        //if (Parent.has_value()) {
        //    ecs::Hierachy::m_SetParent(Parent.value(), entityid);
        //}

        //checks if entity has child call recursion
        const auto& scenePrefabChild = ecs::Hierachy::m_GetChild(sceneprefabID);
        if (scenePrefabChild.has_value()) {
            //clear child id of vector for new entity

            std::vector<ecs::EntityID> childID = scenePrefabChild.value();
            
            ecs::EntityID entityChild;

            //the most scuff way to do sth
            //if size of prefab is less than entity (means a prefab entity was deleted)
            //delete the entire of the childs entity and rebuilt
            {
                const auto& entitych = ecs::Hierachy::m_GetChild(entityid);
                if (entitych.has_value() && (childID.size() < entitych.value().size())) {
                    //delete all the children
                    for (const auto& id : entitych.value()) {
                        ecs->m_DeleteEntity(id);
                    }

                }
            }


            const auto& entitych = ecs::Hierachy::m_GetChild(entityid);
            size_t count{};
            for (const auto& prefabchild : childID) {

                //check if entityid, have child, if no create child
                if (!entitych.has_value()  || entitych.value().size() < (count +1)) {
                    //create entity and assign to entityid
                    
                    entityChild = ecs->m_CreateEntity(scenes::SceneManager::GetSceneByEntityID(entityid).value());
                    ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entityChild));
                    ecs::NameComponent* prefabnc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(prefabchild));
                    nc->m_entityName = prefabnc->m_entityName;

                    nc->m_isPrefab = true;
                    nc->m_prefabName = scenes::SceneManager::GetSceneByEntityID(prefabchild).value();

                    ecs::Hierachy::m_SetParent(entityid, entityChild);
                }
                else {
                    entityChild = entitych.value()[count];
                }

                //ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityChild));

                m_UpdatePrefab(prefabchild, entityChild, true);

                count++;
            }
        }
        else {

            const auto& entitych = ecs::Hierachy::m_GetChild(entityid);
            if (entitych.has_value() && (entitych.value().size() > 0)) {
                //delete all the children
                for (const auto& id : entitych.value()) {
                    ecs->m_DeleteEntity(id);
                }



            }

        }




    }

    void CheckEntityisPrefab(ecs::EntityID id, std::string prefabscene) {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
        if (!nc->m_isPrefab) {
            nc->m_isPrefab = true;
            nc->m_prefabName = prefabscene;
        }

        const auto& child = ecs::Hierachy::m_GetChild(id);
        if (child.has_value()) {

            for (auto& childID : child.value()) {
                CheckEntityisPrefab(childID, prefabscene);
            }

        }
    }

    void Prefab::m_OverWriteScenePrafab(ecs::EntityID id)
    {
        //delete the all the prefab entity
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));

        const std::string& scene = nc->m_prefabName;

        if (ecs->m_ECS_SceneMap.find(scene) == ecs->m_ECS_SceneMap.end()) {
            LOGGING_ERROR("Prefab Scenen not loaded");
        }

        ecs->m_DeleteEntity(ecs->m_ECS_SceneMap.find(scene)->second.m_prefabID);

        //TODO if check if any new child entity, than assign it to be a prefab create a function
        CheckEntityisPrefab(id, scene);


        //duplicate param entity into prefab

        ecs->m_ECS_SceneMap.find(scene)->second.m_prefabID = ecs->m_DuplicateEntity(id, scene);


        //save scene
        scenes::SceneManager::m_GetInstance()->m_SaveScene(scene);

        //call to update all prefab

        m_UpdateAllPrefabEntity(scene);

    }

}

