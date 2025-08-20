/********************************************************************/
/*!
\file      ScriptVariable.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This header file defines the `ScriptEditor` class, which provides
           functions for displaying and modifying script components in an ECS entity.
           - DisplayScriptComponents: Displays and allows editing of public fields
             in a script component attached to an entity.

This file enables the dynamic editing of script component fields within the ECS,
allowing for real-time updates of properties such as integers, floats, and booleans.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "ScriptVariable.h"



#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/class.h>
#include <mono/metadata/metadata.h>

#include "Asset Manager/AssetManager.h"
#include "De&Serialization/json_handler.h"

#include "imgui.h"
#include "imgui_stdlib.h"

namespace scripteditor {

	void ScriptEditor::DisplayScriptComponents(std::string script, ecs::EntityID entityID)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

        static bool is_instanced = false;

		auto* sc = static_cast<ecs::ScriptComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(entityID));

        if (sc->m_scriptInstances.find(script) != sc->m_scriptInstances.end()) {
            is_instanced = true;
        }
        else {
            is_instanced = false;
        }
        //if (assetmanager->m_scriptManager.m_methodMap.find(script) == assetmanager->m_scriptManager.m_methodMap.end()) return;

		MonoImage* image = assetmanager->m_scriptManager.m_loadedDLLMap.find(assetmanager->m_scriptManager.m_outputdll)->second.m_image;
        if (image == NULL) return;

		MonoClass* scriptclass = mono_class_from_name(image, "", script.c_str());

        if (scriptclass == NULL) return;

        void* iter = nullptr;
        MonoClassField* field;
        while ((field = mono_class_get_fields(scriptclass, &iter)) != nullptr) {

            if(mono_field_get_flags(field) & 0x0010)return; // 0x0010 means static

            // Check if the field is public
            if ((mono_field_get_flags(field) & 0x0006) == 0x0006) { //0x0006 representing public
                const char* fieldName = mono_field_get_name(field);
                MonoType* fieldType = mono_field_get_type(field);
                int fieldTypeCode = mono_type_get_type(fieldType);

                MonoClassField* fields = mono_class_get_field_from_name(scriptclass, fieldName);

                //std::cout << "Found public field: " << fieldName << " (Type Code: " << fieldTypeCode << ")\n";

                // Change the value based on type (example for int and float)
                if (fieldTypeCode == MONO_TYPE_I4) { // Type code for int
                    int integer{0};
                    if (is_instanced) {
                        mono_field_get_value(sc->m_scriptInstances.find(script)->second.first, fields, &integer);
                    }
                    else {
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                        if (it != sc->m_scripts.end()) {
                            auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                            if (variableit != std::get<2>(*it).end()) {// if field exist
                                auto decodedData = Serialization::Serialize::DecodeBase64(variableit->second);
                                if (decodedData) {
                                    integer = *(static_cast<int*>(decodedData.get()));
                                }
                            }
                            else { // create field name in map
                                std::get<2>(*it)[fieldName] = std::string{}; //set default to 0
                            }
                        }      

                    }

                    ImGui::AlignTextToFramePadding();
                    if (ImGui::DragInt(fieldName, &integer, 1)) {
                        if (is_instanced) {
                            mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, &integer);
                        }
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });
                        if (it != sc->m_scripts.end()) {

                            std::get<2>(*it)[fieldName] = Serialization::Serialize::m_EncodeBase64(reinterpret_cast<void*>(&integer), sizeof(void*));

                        }
                    }
                }
                else if (fieldTypeCode == MONO_TYPE_R4) { // Type code for float
                    float _float{ 0 };
                    if (is_instanced) {
                        mono_field_get_value(sc->m_scriptInstances.find(script)->second.first, fields, &_float);
                    }
                    else {
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                        if (it != sc->m_scripts.end()) {
                            auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                            if (variableit != std::get<2>(*it).end()) {// if field exist
                                auto decodedData = Serialization::Serialize::DecodeBase64(variableit->second);
                                if (decodedData) {
                                    _float = *(static_cast<float*>(decodedData.get()));
                                }
                            }
                            else { // create field name in map
                                std::get<2>(*it)[fieldName] = std::string{}; //set default to 0
                            }
                        }

                    }

                    ImGui::AlignTextToFramePadding();
                    if (ImGui::DragFloat(fieldName, &_float, 1)) {
                        if (is_instanced) {
                            mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, &_float);
                        }
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });
                        if (it != sc->m_scripts.end()) {

                            std::get<2>(*it)[fieldName] = Serialization::Serialize::m_EncodeBase64(reinterpret_cast<void*>(&_float), sizeof(void*));

                        }
                    }

                }
                else if (fieldTypeCode == MONO_TYPE_BOOLEAN) { // Type code for float
                    bool _bool{false};
                    if (is_instanced) {
                        mono_field_get_value(sc->m_scriptInstances.find(script)->second.first, fields, &_bool);
                    }
                    else {
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                        if (it != sc->m_scripts.end()) {
                            auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                            if (variableit != std::get<2>(*it).end()) {// if field exist
                                auto decodedData = Serialization::Serialize::DecodeBase64(variableit->second);
                                if (decodedData) {
                                    _bool = *(static_cast<bool*>(decodedData.get()));
                                }
                            }
                            else { // create field name in map
                                std::get<2>(*it)[fieldName] = std::string{}; //set default to 0
                            }
                        }

                    }

                    ImGui::AlignTextToFramePadding();
                    if (ImGui::Checkbox(fieldName, &_bool)) {
                        if (is_instanced) {
                            mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, &_bool);
                        }
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });
                        if (it != sc->m_scripts.end()) {

                            std::get<2>(*it)[fieldName] = Serialization::Serialize::m_EncodeBase64(reinterpret_cast<void*>(&_bool), sizeof(void*));

                        }
                    }


                }
                else if (fieldTypeCode == MONO_TYPE_STRING) { // Type code for string

                    std::string string{};
                    MonoString* monoStr = nullptr;


                    if (is_instanced) {
                        mono_field_get_value(sc->m_scriptInstances.find(script)->second.first, fields, &monoStr);

                        if (monoStr != NULL) {
                            const char* cStr = mono_string_to_utf8(monoStr);
                            string = cStr;
                            mono_free((void*)cStr); // Free memory allocated by mono_string_to_utf8
                        }


                    }
                    else {
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                        if (it != sc->m_scripts.end()) {
                            auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                            if (variableit != std::get<2>(*it).end()) {// if field exist
                                string = variableit->second;
                            }
                            else { // create field name in map
                                std::get<2>(*it)[fieldName] = std::string{}; //set default to 0
                            }
                        }

                    }

                    ImGui::AlignTextToFramePadding();
                    if (ImGui::InputText(fieldName, &string)) {
                        if (is_instanced) {
                            MonoString* newMonoStr = mono_string_new(mono_domain_get(), string.c_str());
                            mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, newMonoStr);
                        }
                        const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });
                        if (it != sc->m_scripts.end()) {

                            std::get<2>(*it)[fieldName] = string;

                        }
                    }

                
                }
            }
        }

	}




}


