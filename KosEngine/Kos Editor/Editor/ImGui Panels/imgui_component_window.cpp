/******************************************************************/
/*!
\file      imgui_component_window.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This file implements the ImGui component window for handling
           user interactions with ECS components in the application. It
           enables the addition, display, and modification of various
           entity components within the ECS framework.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "ECS/ECS.h"
#include "Asset Manager/AssetManager.h"
#include "Graphics/GraphicsPipe.h"
#include "ECS/Layers.h"
#include "Editor/TilemapCalculations.h"
#include "Events/EventsDragFloat.h"
#include "Events/EventsEventHandler.h"
#include "Pathfinding/AStarPathfinding.h"
#include "ScriptVariable.h"
#include "Editor/WindowFile.h"
#pragma warning(push)
#pragma warning(disable : 26495)  // Disable uninitialized variable warning
#include "Dependencies/rapidjson/document.h"
#include "Dependencies/rapidjson/writer.h"
#include "Dependencies/rapidjson/filereadstream.h"
#pragma warning(pop)




static const float slider_start_pos_x = 100.0f; //Padding for the slider
static const float slidersize = 50.f;

template <typename T>
struct DrawComponents {

    T m_Array;
    int count{};
    


    bool operator()(float& _args) {
        

        ImGui::AlignTextToFramePadding();
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        bool changed = false;
        ImGui::DragFloat(title.c_str(), &_args, 0.1f, -1000.0f, 1000.f, "%.2f");
        if (ImGui::IsItemActivated()) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }
        ImGui::PopItemWidth();
        count++;
        return changed;
    }

    bool operator()(physicspipe::EntityType& _args)
    {
        const char* shapeName = (_args == physicspipe::EntityType::CIRCLE) ? "CIRCLE" : "RECTANGLE";
        bool changed = false;
        if (ImGui::BeginCombo("Shape Types", shapeName))
        {
            if (ImGui::Selectable("RECTANGLE")) 
            {
                _args = physicspipe::EntityType::RECTANGLE;
                changed = true;
            }
            if (ImGui::Selectable("CIRCLE")) 
            {
                _args = physicspipe::EntityType::CIRCLE;
                changed = true;

            }
            ImGui::EndCombo();
        }
        count++;
        return changed;
    }
    bool operator()(graphicpipe::LightType& _args)
    {
        const char* shapeName = (_args == graphicpipe::LightType::GLOW) ? "GLOW" : "MULTIPLY";
        bool changed = false;
        if (ImGui::BeginCombo("Shape Types", shapeName))
        {
            if (ImGui::Selectable("MULTIPLY"))
            {
                _args = graphicpipe::LightType::MULTIPLY;
                changed = true;
            }
            if (ImGui::Selectable("GLOW"))
            {
                _args = graphicpipe::LightType::GLOW;
                changed = true;
            }
            ImGui::EndCombo();
        }
        count++;
        return changed;
    }

    bool operator()(int& _args) {
        
        ImGui::AlignTextToFramePadding();
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragInt(title.c_str(), &_args, 1.0f, -100, 100);
        ImGui::PopItemWidth();
        bool changed = false;
        if (ImGui::IsItemActivated()) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }
        count++;
        return changed;
    }

    bool operator()(vector2::Vec2& _args) {
        
        ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::PushItemWidth(slidersize);
        std::string title = "X##" + m_Array[count];
        bool changed = false;
        ImGui::DragFloat(title.c_str(), &_args.m_x, 0.02f, -50.f, 50.f, "%.2f");
        if(ImGui::IsItemActivated()){
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        title = "Y##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_y, 0.02f, -50.0f, 50.0f, "%.2f");
        if (ImGui::IsItemActivated() && !changed) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }
        ImGui::PopItemWidth();
        ImGui::PopItemWidth();



        count++;
        return changed;
    }

    bool operator()( vector3::Vec3& _args) {
        ImGui::PushItemWidth(slidersize);
        ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "X##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_x, 0.02f, -50.f, 50.f, "%.2f");
        bool changed = false;
        if (ImGui::IsItemActivated()) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        title = "Y##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_y, 0.02f, -50.0f, 50.0f, "%.2f");
        if (ImGui::IsItemActivated()) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        title = "Z##" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        ImGui::DragFloat(title.c_str(), &_args.m_z, 0.02f, -50.0f, 50.0f, "%.2f");
        if (ImGui::IsItemActivated()) {
            changed = true;
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            changed = true;
        }

        ImGui::PopItemWidth();
        ImGui::PopItemWidth();
        ImGui::PopItemWidth();
        count++;
        return changed;
    }


    bool operator()(bool& _args) {
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        std::string title = "##" + m_Array[count];
        ImGui::Checkbox(title.c_str(), &_args);
        bool changed = false;
        if (ImGui::IsItemEdited()) {
            changed = true;
        }
        count++;
        return changed;
    }

    bool operator()(std::string& _args) {
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        std::string title = "##" + m_Array[count];
        ImGui::InputText(title.c_str(), &_args);
        bool changed = false;
        if (ImGui::IsItemEdited()) {
            changed = true;
        }

        count++;
        return changed;
    }

    void operator()(layer::LAYERS& _args) {
        ImGui::AlignTextToFramePadding();
        ImGui::Text(m_Array[count].c_str());
        ImGui::SameLine(slider_start_pos_x);
        ImGui::SetNextItemWidth(100.0f);
        std::string title = "##slider_" + std::to_string(count) + "_" + m_Array[count];
        ImGui::PushItemWidth(slidersize);
        int toint = int(_args);
        if (ImGui::DragInt(title.c_str(), &toint, 1.0f, 0, 8)) {
            _args = (layer::LAYERS)toint;
        }
        ImGui::PopItemWidth();

        count++;
    }

    template <typename U>
    void operator()(std::vector<U>& _args) {
        if constexpr (std::is_class_v<U>) {
            int _count{};
            for (U& x : _args) {
                ImGui::PushID(_count++);
                x.ApplyFunction(DrawComponents<decltype(x.Names())>{x.Names()});
                ImGui::PopID();
            }
        }
        else {
            int _count{};
            for (U& x : _args) {
                ImGui::PushID(_count++);
                (*this)(x); // Handle non-class types
                count--;// minus so no subsciprt error
                ImGui::PopID();
            }
        }
        count++;
    }
    
    template <typename K>
    void operator()(K& _args) {
        if constexpr (std::is_class_v<K>) {
            _args.ApplyFunction(DrawComponents<decltype(_args.Names())>{_args.Names()});
            count++;
        }
        
    }

};

const char* ComponentNames[] =
{
    "Add Components", "Collider Component", "Sprite Component", "Enemy Component", "Rigid Body Component", "Text Component",
    "Animation Component", "Camera Component" , "Button Component" , "Script Component", "Tilemap Component", "Audio Component",
    "Grid Component", "RayCast Component", "PathfindingComponent", "Lighting Component", "Particle Component","Video Component",
    "UI Sprite Component"
};

void gui::ImGuiHandler::m_DrawComponentWindow()
{
    bool windowOpen = true;
    std::string Title = "Component Window";
    ecs::ECS* ecs = ecs::ECS::m_GetInstance();

    std::string windowTitle = Title;

    ImGui::Begin(windowTitle.c_str(), &windowOpen);

    //Add Component Window

    static int ComponentType = 0;



    if (ecs->m_ECS_EntityMap.size() > 0 && m_clickedEntityId >= 0) {

        ecs::EntityID entityID = m_clickedEntityId;
      


        if (ImGui::Combo("##ADDCOMPONENT", &ComponentType, ComponentNames, IM_ARRAYSIZE(ComponentNames), IM_ARRAYSIZE(ComponentNames))) {
            ecs::compSignature EntitySignature = ecs->m_ECS_EntityMap[entityID];
            if (ComponentType == 1) {
                ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, entityID);
                if (ecs->m_ECS_EntityMap[entityID].test(ecs::TYPESPRITECOMPONENT))
                {
                    graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                    ecs::ColliderComponent* colCom = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entityID));
                    ecs::SpriteComponent* spriteCom = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                    if (!spriteCom->m_imageFile.empty())
                    {
                        assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                        colCom->m_Size.m_x = static_cast<float>(static_cast<float>(assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_width) / static_cast<float>(pipe->m_unitWidth) / assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_stripCount);
                        colCom->m_Size.m_y = static_cast<float>(assets->m_imageManager.m_imageMap[spriteCom->m_imageFile].m_height) / static_cast<float>(pipe->m_unitHeight);
                    }
                }
               
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPECOLLIDERCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPECOLLIDERCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 2) {
                ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPESPRITECOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPESPRITECOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 3) {
                ecs->m_AddComponent(ecs::TYPEENEMYCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEENEMYCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEENEMYCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 4) {
                ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPERIGIDBODYCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 5) {
                ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPETEXTCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPETEXTCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 6) {
                ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEANIMATIONCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEANIMATIONCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 7) {
                ecs->m_AddComponent(ecs::TYPECAMERACOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPECAMERACOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPECAMERACOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 8) {
                ecs->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEBUTTONCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEBUTTONCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 9) {
                ecs->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPESCRIPTCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPESCRIPTCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 10) {
                ecs->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPETILEMAPCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPETILEMAPCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 11) {
                ecs->m_AddComponent(ecs::TYPEAUDIOCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEAUDIOCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEAUDIOCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 12) {
                
                ecs->m_AddComponent(ecs::TYPEGRIDCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEGRIDCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEGRIDCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 13) {
                ecs->m_AddComponent(ecs::TYPERAYCASTINGCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPERAYCASTINGCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPERAYCASTINGCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 14) {
                ecs->m_AddComponent(ecs::TYPEPATHFINDINGCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEPATHFINDINGCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEPATHFINDINGCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 15) {
                 ecs->m_AddComponent(ecs::TYPELIGHTINGCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPELIGHTINGCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPELIGHTINGCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 16) {
                ecs->m_AddComponent(ecs::TYPEPARTICLECOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEPARTICLECOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEPARTICLECOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 17) {
                ecs->m_AddComponent(ecs::TYPEVIDEOCOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEVIDEOCOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEVIDEOCOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }
            if (ComponentType == 18) {
                ecs->m_AddComponent(ecs::TYPEUISPRITECOMPONENT, entityID);
                ComponentType = 0;
                if (!EntitySignature.test(ecs::TYPEUISPRITECOMPONENT)) {
                    events::AddComponent action(entityID, ecs::TYPEUISPRITECOMPONENT);
                    ////DISPATCH_ACTION_EVENT(action);
                }
            }

           
        }

        auto CreateContext = [](ecs::ComponentType Type, ecs::EntityID ID) {
            if (ImGui::BeginPopupContextItem()) {
                if (Type != ecs::TYPETRANSFORMCOMPONENT && ImGui::MenuItem("Delete Component")) {
                    ecs::ECS::m_GetInstance()->m_RemoveComponent(Type, ID);
                    events::RemoveComponent removeTemp(ID,Type);
                    //DISPATCH_ACTION_EVENT(removeTemp);
                }
                if (ImGui::MenuItem("Reset Component")) {
                    if (Type == ecs::TYPETRANSFORMCOMPONENT) {
                        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
                        const auto& tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
                        tc->m_position = vector2::Vec2{0.f, 0.f};
                        tc->m_rotation = 0.f;
                        tc->m_scale = vector2::Vec2{1.f, 1.f};
                    }
                    else {
                        ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[Type]->m_ResetComponent(ID);
                    }
                    
                    
                }

                ImGui::EndPopup();
            }
        };

        //static std::map<int, std::pair<ecs::EntityID, bool>> layerMap; // Bool represents whether it's a sprite or a text
        //static bool layerChange{ true };
        //static size_t entitySize = ecs->m_ECS_EntityMap.size();
        //static std::string sceneChange = m_activeScene;

        //if (layerChange || entitySize != ecs->m_ECS_EntityMap.size() || sceneChange != m_activeScene)
        //{
        //    layerChange = false;
        //    entitySize = ecs->m_ECS_EntityMap.size();
        //    sceneChange = m_activeScene;
        //    layerMap.clear();

        //    for (const auto& scene : ecs->m_ECS_SceneMap)
        //    {
        //        if (scene.second.m_isActive)
        //        {
        //            for (const auto& id : scene.second.m_sceneIDs)
        //            {
        //                if (ecs->m_ECS_EntityMap[id].test(ecs::TYPESPRITECOMPONENT))
        //                {
        //                    ecs::SpriteComponent* sprite = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(id));
        //                    const int maxLayer = 999;
        //                    int layer = sprite->m_layer;
        //                    if (layer >= maxLayer)
        //                    {
        //                        layer = maxLayer;
        //                        while (layerMap.find(--layer) != layerMap.end());
        //                    }
        //                    else if (layerMap.find(sprite->m_layer) != layerMap.end())
        //                    {
        //                        while (layerMap.find(++layer) != layerMap.end())
        //                        {

        //                        }
        //                    }
        //                    sprite->m_layer = layer;
        //                    layerMap[layer] = { id , true };
        //                }
        //                if (ecs->m_ECS_EntityMap[id].test(ecs::TYPETEXTCOMPONENT))
        //                {
        //                    ecs::TextComponent* text = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(id));
        //                    const int maxLayer = 999;
        //                    int layer = text->m_fontLayer;
        //                    if (layer >= maxLayer)
        //                    {
        //                        layer = maxLayer;
        //                        while (layerMap.find(--layer) != layerMap.end());
        //                    }
        //                    else if (layerMap.find(text->m_fontLayer) != layerMap.end())
        //                    {
        //                        while (layerMap.find(++layer) != layerMap.end())
        //                        {

        //                        }
        //                    }
        //                    text->m_fontLayer = layer;
        //                    layerMap[layer] = { id , false };
        //                }
        //            }
        //        }


        //    }
        //}
        


        ecs::compSignature EntitySignature = ecs->m_ECS_EntityMap[entityID];
        assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
        ImGui::SeparatorText("Components");

        if (EntitySignature.test(ecs::TYPENAMECOMPONENT))
        {
            // Retrieve the TransformComponent
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]
                ->m_GetEntityComponent(entityID));
            static ecs::NameComponent oldValN = *nc;
            //Display Position
            ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
            ImGui::Text("Object Name: ");
            ImGui::SameLine(slider_start_pos_x);
            ImGui::SetNextItemWidth(100.0f);
            ImGui::InputText("##NAMETEXT##", &nc->m_entityName);
            if (ImGui::IsItemDeactivatedAfterEdit()) {
                events::ModifyName action(ecs::TYPENAMECOMPONENT, entityID, nc, oldValN);
                ////DISPATCH_ACTION_EVENT(action);
                oldValN = *nc;
            }
            //ImGui::SameLine();

            bool hidden = nc->m_hide;
            if (ImGui::Checkbox("Hide", &hidden)) {
                if (!nc->m_hide) {
                    ecs->m_layersStack.m_hideEntitywithChild(entityID);
                }
                else {
                    ecs->m_layersStack.m_unhideEntitywithChild(entityID);
                }

            }

            //layer selector
            const char* layers[] = { ecs->m_layersStack.m_layerMap[layer::DEFAULT].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER1].first.c_str(),ecs->m_layersStack.m_layerMap[layer::LAYER2].first.c_str(),
                                  ecs->m_layersStack.m_layerMap[layer::LAYER3].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER4].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER5].first.c_str(),
                                  ecs->m_layersStack.m_layerMap[layer::LAYER6].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER7].first.c_str(), ecs->m_layersStack.m_layerMap[layer::LAYER8].first.c_str() };


            std::vector<const char*> layers_vec;
            for (const auto& _layer : ecs->m_layersStack.m_layerMap) {
                layers_vec.push_back(_layer.second.first.c_str());
            }

            int layer_current = nc->m_Layer;
            if (ImGui::Combo("Layers", &layer_current, layers_vec.data(), static_cast<int>(layers_vec.size()))) {
                ecs->m_layersStack.m_SwapEntityLayer((layer::LAYERS)layer_current, nc->m_Layer, entityID);
                events::ModifyName action(ecs::TYPENAMECOMPONENT, entityID, nc, oldValN);
                ////DISPATCH_ACTION_EVENT(action);
                oldValN = *nc;
            }

            // Convert vector to array of char* for ImGui
            std::vector<const char*> tag_Names(m_tags.size());
            std::transform(m_tags.begin(), m_tags.end(), tag_Names.begin(), [](const std::string& tag) {
                return tag.c_str();
                });

            int item{};
            const auto& it = std::find(tag_Names.begin(), tag_Names.end(), nc->m_entityTag);
            if (it != tag_Names.end()) {
                item = static_cast<int>(std::distance(tag_Names.begin(), it));
            }
            else {
                tag_Names.push_back(nc->m_entityTag.c_str());
                const auto& it2 = std::find(tag_Names.begin(), tag_Names.end(), nc->m_entityTag);
                item = static_cast<int>(std::distance(tag_Names.begin(), it2));
            }
            
            if (ImGui::Combo("Tag", &item, tag_Names.data(), static_cast<int>(tag_Names.size()))) {
                nc->m_entityTag = m_tags[item];
                events::ModifyName action(ecs::TYPENAMECOMPONENT, entityID, nc, oldValN);
                ////DISPATCH_ACTION_EVENT(action);
                oldValN = *nc;
            }

           // std::cout << nc->m_entityTag << std::endl;
            //create overwrite button for prefab
            if (nc->m_isPrefab && !m_prefabSceneMode) {
                auto* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID));
                if (!tc->m_haveParent || !static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(tc->m_parentID))->m_isPrefab) {
                    if (ImGui::Button("Overwrite")) {

                        prefab::Prefab::m_OverWriteScenePrafab(entityID);

                    }
                    ImGui::SameLine();

                    if (ImGui::Checkbox("Sync", &nc->m_syncPrefab)) {

                        prefab::Prefab::m_UpdateAllPrefabEntity(nc->m_prefabName);
                    }
                }
                
                const char* buf = nc->m_prefabName.c_str();
                ImGui::InputText("##readonlytext", (char*)buf, strlen(buf), ImGuiInputTextFlags_ReadOnly);
            }



            static bool open;

            //TODO find better way to implement
            if (EntitySignature.test(ecs::TYPETRANSFORMCOMPONENT)) {
                
                open = ImGui::CollapsingHeader("Transform Component");

                CreateContext(ecs::TYPETRANSFORMCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPETRANSFORMCOMPONENT)) {
                    auto* rbc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID));
                    DrawComponents toDraw(rbc->Names());
                    static ecs::TransformComponent oldVal = *rbc;
                    if (toDraw(rbc->m_position)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::POS)) {
                            oldVal = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TRANSFORM, dragfloat::Member::POS)) {
                            events::TransformComponentChanged action(ecs::TYPETRANSFORMCOMPONENT, entityID, rbc, oldVal);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldVal = *rbc;
                        }
                        
                    }
                    if (toDraw(rbc->m_rotation)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ROT)) {
                            oldVal = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TRANSFORM, dragfloat::Member::ROT)) {
                            events::TransformComponentChanged action(ecs::TYPETRANSFORMCOMPONENT, entityID, rbc, oldVal);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldVal = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_scale)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::SCALE)) {
                            oldVal = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TRANSFORM, dragfloat::Member::SCALE)) {
                            events::TransformComponentChanged action(ecs::TYPETRANSFORMCOMPONENT, entityID, rbc, oldVal);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldVal = *rbc;
                        }
                    }
                    
                }


            }
            if (EntitySignature.test(ecs::TYPESPRITECOMPONENT)){

                open = ImGui::CollapsingHeader("Sprite Component");

                CreateContext(ecs::TYPESPRITECOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPESPRITECOMPONENT)) {

                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                    static ecs::SpriteComponent oldValS = *sc;
                   
                        
                    if (ImGui::BeginCombo("Images", sc->m_imageFile.c_str()))
                    {
                        for (const auto& image : Asset->m_imageManager.m_imageMap) {

                            if (ImGui::Selectable(image.first.c_str())) {
                                //static ecs::SpriteComponent oldValA = *sc;
                                sc->m_imageFile = image.first.c_str();
                                if (!ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT))
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    //graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                                    if (assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount > 1)
                                    {
                                       
                                        ecs::AnimationComponent* com = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID));
                                        com->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                        events::ModifySprite action2(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                                        //DISPATCH_ACTION_EVENT(action2);
                                        events::AddComponent action1(entityID, ecs::TYPEANIMATIONCOMPONENT);
                                        //DISPATCH_ACTION_EVENT(action1);
                                        oldValS = *sc;
                                       
                                    }
                                }
                                else 
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    ecs::AnimationComponent* com = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                                    com->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                    //static ecs::AnimationComponent oldValA2 = *com;
                                    //events::ModifyAnim action(ecs::TYPESPRITECOMPONENT, entityID, com, oldValA2);
                                    //////DISPATCH_ACTION_EVENT(action);
                                    events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                                    ////DISPATCH_ACTION_EVENT(action);
                                    oldValS = *sc;
                                }
                            }
                        }
                        ImGui::EndCombo();
                    }

                    if (ImGui::BeginDragDropTarget())
                    {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                            if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {
                                if (Asset->m_imageManager.m_imageMap.find(filename->filename().string()) == Asset->m_imageManager.m_imageMap.end()) {
                                    LOGGING_WARN("File not loaded, please reload content browser");
                                }
                                else {
                                    sc->m_imageFile = filename->filename().string();
                                }
                            }
                            else {
                                LOGGING_WARN("Wrong File Type");
                            }



                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImVec4 color = ImVec4(sc->m_color.m_x, sc->m_color.m_y, sc->m_color.m_z, sc->m_alpha);

                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3("##MyColor2", (float*)&color, ImGuiColorEditFlags_DisplayRGB))
                    {
                        sc->m_color.m_x = color.x;
                        sc->m_color.m_y = color.y;
                        sc->m_color.m_z = color.z;
                        //events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                        //////DISPATCH_ACTION_EVENT(action);
                    }
                    if (ImGui::IsItemActivated()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::SPRITECLR)) {
                            oldValS = *sc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::SPRITE, dragfloat::Member::SPRITECLR)) {
                            events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValS = *sc;
                        }
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::SPRITECLR)) {
                            oldValS = *sc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::SPRITE, dragfloat::Member::SPRITECLR)) {
                            events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValS = *sc;
                        }
                    }
                    //static ecs::SpriteComponent oldValS2 = *sc;
                    if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPESPRITECOMPONENT)) {
                        auto* rbc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                        //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                        DrawComponents toDraw(rbc->Names());
                        if (toDraw(rbc->m_imageFile)) {

                            events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValS = *rbc;
                        }
                        if (toDraw(rbc->m_isIlluminated)) {
                            events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValS = *rbc;
                        }
                        if (toDraw(rbc->m_alpha)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::SPRITEALPHA)) {
                                oldValS = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::SPRITE, dragfloat::Member::SPRITEALPHA)) {
                                events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, rbc, oldValS);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValS = *rbc;
                            }
                        }
                        if (toDraw(rbc->m_layer)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::SPRITELAYER)) {
                                oldValS = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::SPRITE, dragfloat::Member::SPRITELAYER)) {
                                events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, rbc, oldValS);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValS = *rbc;
                            }
                        }
                        
                    }

               
                    

                //    if (ImGui::TreeNode("Image Layers"))
                //    {
                //        //int count = 0;
                //        for (auto it2 = layerMap.begin(); it2 != layerMap.end(); ++it2) 
                //        {
                //            ecs::NameComponent* namec = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(it2->second.first));
                //            if (namec == nullptr) continue;
                //            std::string entityIDS = std::to_string(it2->second.first);
                //            if (it2->second.second)
                //            {
                //                entityIDS += " (Sprite)";
                //            }
                //            else
                //            {
                //                entityIDS += " (Font)";
                //            }

                //            std::string selectable = namec->m_entityName + "_ID" + entityIDS;
                //            if (it2->second.first == static_cast<unsigned>(m_clickedEntityId))
                //            {
                //                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.8f, 0.f, 1.0f));
                //                selectable += "(Active)";
                //                ImGui::Selectable(selectable.c_str());
                //                ImGui::PopStyleColor();
                //            }
                //            else
                //            {
                //                ImGui::Selectable(selectable.c_str());
                //            }
                //            
                //          /*  std::ostringstream display;
                //            display << selectable << std::setw(30 - selectable.length()) << std::right << "Layer" + std::to_string(spriteLayer);*/
                //            

                //            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                //            {
                //                decltype(it2) n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? std::prev(it2) : std::next(it2));
                //                if (n_next != layerMap.end())
                //                {
                //                    if (n_next->second.second && it2->second.second)
                //                    {
                //                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                //                        ecs::SpriteComponent* sprite2 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                //                        int layer = sprite1->m_layer;
                //                        sprite1->m_layer = sprite2->m_layer;
                //                        sprite2->m_layer = layer;
                //                    }
                //                    else if (!n_next->second.second && !it2->second.second)
                //                    {
                //                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                //                        ecs::TextComponent* text2 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                //                        int layer = text1->m_fontLayer;
                //                        text1->m_fontLayer = text2->m_fontLayer;
                //                        text2->m_fontLayer = layer;
                //                    }
                //                    else if (n_next->second.second && !it2->second.second)
                //                    {
                //                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                //                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                //                        int layer = text1->m_fontLayer;
                //                        text1->m_fontLayer = sprite1->m_layer;
                //                        sprite1->m_layer = layer;
                //                    }
                //                    else
                //                    {
                //                        ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                //                        ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                //                        int layer = text1->m_fontLayer;
                //                        text1->m_fontLayer = sprite1->m_layer;
                //                        sprite1->m_layer = layer;
                //                    }
                //                    layerChange = true;
                //                    ImGui::ResetMouseDragDelta();
                //                }
                //            }
                //           
                //        }
                //        ImGui::TreePop();
                //    }
                //  
                }


            }
            if (EntitySignature.test(ecs::TYPECOLLIDERCOMPONENT)) {

                open = ImGui::CollapsingHeader("Collider Component");

                CreateContext(ecs::TYPECOLLIDERCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPECOLLIDERCOMPONENT)) {
                    auto* rbc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entityID));
                    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    static ecs::ColliderComponent oldValC = *rbc;
                    DrawComponents toDraw(rbc->Names());
                    if (toDraw(rbc->m_collisionResponse)) {
                        events::ModifyCollider action(ecs::TYPECOLLIDERCOMPONENT, entityID, rbc, oldValC);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValC = *rbc;
                    }
                    if (toDraw(rbc->m_collisionCheck)) {
                        events::ModifyCollider action(ecs::TYPECOLLIDERCOMPONENT, entityID, rbc, oldValC);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValC = *rbc;
                    }
                    if (toDraw(rbc->m_drawDebug)) {
                        events::ModifyCollider action(ecs::TYPECOLLIDERCOMPONENT, entityID, rbc, oldValC);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValC = *rbc;
                    }
                    if (toDraw(rbc->m_type)) {
                        events::ModifyCollider action(ecs::TYPECOLLIDERCOMPONENT, entityID, rbc, oldValC);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValC = *rbc;
                    }

                    if (toDraw(rbc->m_OffSet)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::COLLOFFSET)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::COLL, dragfloat::Member::COLLOFFSET)) {
                            events::ModifyCollider action(ecs::TYPECOLLIDERCOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_radius)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::COLLRAD)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::COLL, dragfloat::Member::COLLRAD)) {
                            events::ModifyCollider action(ecs::TYPECOLLIDERCOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_Size)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::COLLSIZE)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::COLL, dragfloat::Member::COLLSIZE)) {
                            events::ModifyCollider action(ecs::TYPECOLLIDERCOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                }


            }
            if (EntitySignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {

                open = ImGui::CollapsingHeader("RigidBody Component");

                CreateContext(ecs::TYPERIGIDBODYCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPERIGIDBODYCOMPONENT)) {
                    auto* rbc = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entityID));
                    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    static ecs::RigidBodyComponent oldValR = *rbc;
                    DrawComponents toDraw(rbc->Names());

                    if (toDraw(rbc->m_Velocity)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDVELO)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDVELO)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_Acceleration)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDACCEL)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDACCEL)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }

                    if (toDraw(rbc->m_Rotation)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDROT)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDROT)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_AngularVelocity)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDANGVELO)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDANGVELO)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_AngularAcceleration)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDANGACCEL)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDANGACCEL)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_Mass)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDMASS)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDMASS)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_InverseMass)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDINVMASS)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDINVMASS)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_LinearDamping)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDLINDAMP)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDLINDAMP)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_AngularDamping)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDANGDAMP)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDANGDAMP)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_Force)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDFORCE)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDFORCE)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_Torque)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::RIGIDTORQUE)) {
                            oldValR = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::RIGID, dragfloat::Member::RIGIDTORQUE)) {
                            events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValR = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_IsKinematic)) {
                        events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValR = *rbc;
                    }
                    if (toDraw(rbc->m_IsStatic)) {
                        events::ModifyRigid action(ecs::TYPERIGIDBODYCOMPONENT, entityID, rbc, oldValR);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValR = *rbc;
                    }
                }


            }
            if (EntitySignature.test(ecs::TYPEENEMYCOMPONENT)) {

                open = ImGui::CollapsingHeader("Enemy Component");

                CreateContext(ecs::TYPEENEMYCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEENEMYCOMPONENT)) {
                    auto* rbc = static_cast<ecs::EnemyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEENEMYCOMPONENT]->m_GetEntityComponent(entityID));
                    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    static ecs::EnemyComponent oldValE = *rbc;
                    DrawComponents toDraw(rbc->Names());
                    if (toDraw(rbc->m_enemyTag)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ENEMYTAG)) {
                            oldValE = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::ENEMY, dragfloat::Member::ENEMYTAG)) {
                            events::ModifyEnemy action(ecs::TYPEENEMYCOMPONENT, entityID, rbc, oldValE);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValE = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_enemyTypeInt)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ENEMYTYPE)) {
                            oldValE = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::ENEMY, dragfloat::Member::ENEMYTYPE)) {
                            events::ModifyEnemy action(ecs::TYPEENEMYCOMPONENT, entityID, rbc, oldValE);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValE = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_enemyRoamBehaviourInt)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ENEMYBEHAVE)) {
                            oldValE = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::ENEMY, dragfloat::Member::ENEMYBEHAVE)) {
                            events::ModifyEnemy action(ecs::TYPEENEMYCOMPONENT, entityID, rbc, oldValE);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValE = *rbc;
                        }
                    }


                }


            }
            if (EntitySignature.test(ecs::TYPETEXTCOMPONENT)) {

                open = ImGui::CollapsingHeader("Text Component");

                CreateContext(ecs::TYPETEXTCOMPONENT, entityID);


                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPETEXTCOMPONENT)) {

                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* tc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityID));
                    static ecs::TextComponent oldValT = *tc;
                    ImVec4 color = ImVec4(tc->m_color.m_x, tc->m_color.m_y, tc->m_color.m_z, 255.0f / 255.0f);
                    DrawComponents toDraw(tc->Names());
                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Text: ");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::InputText("##TEXT##", &tc->m_text);
                    if (ImGui::IsItemActivated()) {
                        events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValT = *tc;
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValT = *tc;
                    }

                    // Checkbox for cursor centering
                    ImGui::Text("Center Text");
                    ImGui::SameLine();
                    ImGui::Checkbox("##Text Centered", &tc->m_isCentered);

         
                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Size");
                    ImGui::SameLine(slider_start_pos_x);
                    ImGui::SetNextItemWidth(100.0f);
                    ImGui::DragFloat("###TEXTXXX", &tc->m_fontSize, 0.05f, 1.f, 1000.0f, "%.2f");
                    if (ImGui::IsItemActivated()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TEXTSIZE)) {
                            oldValT = *tc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TEXT, dragfloat::Member::TEXTSIZE)) {
                            events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValT = *tc;
                        }
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TEXTSIZE)) {
                            oldValT = *tc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TEXT, dragfloat::Member::TEXTSIZE)) {
                            events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValT = *tc;
                        }
                    }
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3("##MyColor1", (float*)&color, ImGuiColorEditFlags_DisplayRGB)) {
                        tc->m_color.m_x = color.x;
                        tc->m_color.m_y = color.y;
                        tc->m_color.m_z = color.z;
                    }
                    if (ImGui::IsItemActivated()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TEXTCLR)) {
                            oldValT = *tc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TEXT, dragfloat::Member::TEXTCLR)) {
                            events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValT = *tc;
                        }
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TEXTCLR)) {
                            oldValT = *tc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TEXT, dragfloat::Member::TEXTCLR)) {
                            events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValT = *tc;
                        }
                    }
                    if (ImGui::BeginCombo("Fonts", tc->m_fileName.c_str()))
                    {
                        for (const auto& font : Asset->m_fontManager.m_fonts) {
                            if (font.first.empty())continue;
                            if (ImGui::Selectable(font.first.c_str())) {
                                tc->m_fileName = font.first.c_str();
                                events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValT = *tc;
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::BeginDragDropTarget())
                    {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                            if (filename->filename().extension().string() == ".ttf") {
                                if (Asset->m_fontManager.m_fonts.find(filename->filename().string()) == Asset->m_fontManager.m_fonts.end()) {
                                    LOGGING_WARN("File not loaded, please reload content browser");
                                }
                                else {
                                    tc->m_fileName = filename->filename().string();
                                }
                            }
                            else {
                                LOGGING_WARN("Wrong File Type");
                            }



                        }
                        ImGui::EndDragDropTarget();
                    }

                    //if (ImGui::TreeNode("Font Layers"))
                    //{
                    //    //int count = 0;
                    //    for (auto it2 = layerMap.begin(); it2 != layerMap.end(); ++it2)
                    //    {
                    //        ecs::NameComponent* namec = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(it2->second.first));
                    //        std::string entityIDS = std::to_string(it2->second.first);
                    //        if (it2->second.second)
                    //        {
                    //            entityIDS += " (Sprite)";
                    //        }
                    //        else
                    //        {
                    //            entityIDS += " (Font)";
                    //        }

                    //        std::string selectable = namec->m_entityName + "_ID" + entityIDS;
                    //        if (it2->second.first == static_cast<unsigned>(m_clickedEntityId))
                    //        {
                    //            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.3f, 0.8f, 1.0f));
                    //            selectable += "(Active)";
                    //            ImGui::Selectable(selectable.c_str());
                    //            ImGui::PopStyleColor();
                    //        }
                    //        else
                    //        {
                    //            ImGui::Selectable(selectable.c_str());
                    //        }

                    //        /*  std::ostringstream display;
                    //          display << selectable << std::setw(30 - selectable.length()) << std::right << "Layer" + std::to_string(spriteLayer);*/


                    //        if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                    //        {
                    //            decltype(it2) n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? std::prev(it2) : std::next(it2));
                    //            if (n_next != layerMap.end())
                    //            {
                    //                if (n_next->second.second && it2->second.second)
                    //                {
                    //                    ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                    //                    ecs::SpriteComponent* sprite2 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                    //                    int layer = sprite1->m_layer;
                    //                    sprite1->m_layer = sprite2->m_layer;
                    //                    sprite2->m_layer = layer;
                    //                }
                    //                else if (!n_next->second.second && !it2->second.second)
                    //                {
                    //                    ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                    //                    ecs::TextComponent* text2 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                    //                    int layer = text1->m_fontLayer;
                    //                    text1->m_fontLayer = text2->m_fontLayer;
                    //                    text2->m_fontLayer = layer;
                    //                }
                    //                else if (n_next->second.second && !it2->second.second)
                    //                {
                    //                    ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(n_next->second.first));
                    //                    ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(it2->second.first));
                    //                    int layer = text1->m_fontLayer;
                    //                    text1->m_fontLayer = sprite1->m_layer;
                    //                    sprite1->m_layer = layer;
                    //                }
                    //                else
                    //                {
                    //                    ecs::SpriteComponent* sprite1 = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(it2->second.first));
                    //                    ecs::TextComponent* text1 = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(n_next->second.first));
                    //                    int layer = text1->m_fontLayer;
                    //                    text1->m_fontLayer = sprite1->m_layer;
                    //                    sprite1->m_layer = layer;
                    //                }
                    //                layerChange = true;
                    //                ImGui::ResetMouseDragDelta();
                    //            }
                    //        }

                    //    }
                    //    ImGui::TreePop();
                    //}
                    //DrawComponents toDraw(tc->Names());
                    if (toDraw(tc->m_fontLayer)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TEXTLAYER)) {
                            oldValT = *tc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TEXT, dragfloat::Member::TEXTLAYER)) {
                            events::ModifyText action(ecs::TYPETEXTCOMPONENT, entityID, tc, oldValT);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValT = *tc;
                        }
                    }
                }

                //if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPETEXTCOMPONENT)) {
                //    auto* rbc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityID));
                //    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                //    static ecs::TextComponent oldValT2 = *rbc;
                //   
                //}

            }
            if (EntitySignature.test(ecs::TYPEANIMATIONCOMPONENT)) {

                open = ImGui::CollapsingHeader("Animation Component");

                CreateContext(ecs::TYPEANIMATIONCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT)) {
                    auto* rbc = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    static ecs::AnimationComponent oldValA = *rbc;
                    DrawComponents toDraw(rbc->Names());

                    if (toDraw(rbc->m_frameNumber)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ANIMFRAMENUM)) {
                            oldValA = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::ANIM, dragfloat::Member::ANIMFRAMENUM)) {
                            events::ModifyAnim action(ecs::TYPEANIMATIONCOMPONENT, entityID, rbc, oldValA);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValA = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_framesPerSecond)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ANIMFPS)) {
                            oldValA = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::ANIM, dragfloat::Member::ANIMFPS)) {
                            events::ModifyAnim action(ecs::TYPEANIMATIONCOMPONENT, entityID, rbc, oldValA);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValA = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_frameTimer)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ANIMFT)) {
                            oldValA = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::ANIM, dragfloat::Member::ANIMFT)) {
                            events::ModifyAnim action(ecs::TYPEANIMATIONCOMPONENT, entityID, rbc, oldValA);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValA = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_isAnimating)) {
                        events::ModifyAnim action(ecs::TYPEANIMATIONCOMPONENT, entityID, rbc, oldValA);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValA = *rbc;
                    }
                    if (toDraw(rbc->m_stripCount)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::ANIMSC)) {
                            oldValA = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::ANIM, dragfloat::Member::ANIMSC)) {
                            events::ModifyAnim action(ecs::TYPEANIMATIONCOMPONENT, entityID, rbc, oldValA);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValA = *rbc;
                        }
                    }
                }


            }
            if (EntitySignature.test(ecs::TYPECAMERACOMPONENT)) {

                open = ImGui::CollapsingHeader("Camera Component");

                CreateContext(ecs::TYPECAMERACOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPECAMERACOMPONENT)) {
                    auto* rbc = static_cast<ecs::CameraComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECAMERACOMPONENT]->m_GetEntityComponent(entityID));
                    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    static ecs::CameraComponent oldValC = *rbc;
                    DrawComponents toDraw(rbc->Names());
                    if (toDraw(rbc->m_left)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::CAMLEFT)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::CAMERA, dragfloat::Member::CAMLEFT)) {
                            events::ModifyCamera action(ecs::TYPECAMERACOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_right)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::CAMRIGHT)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::CAMERA, dragfloat::Member::CAMRIGHT)) {
                            events::ModifyCamera action(ecs::TYPECAMERACOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_bottom)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::CAMDOWN)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::CAMERA, dragfloat::Member::CAMDOWN)) {
                            events::ModifyCamera action(ecs::TYPECAMERACOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_top)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::CAMUP)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::CAMERA, dragfloat::Member::CAMUP)) {
                            events::ModifyCamera action(ecs::TYPECAMERACOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_aspectRatio)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::CAMAR)) {
                            oldValC = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::CAMERA, dragfloat::Member::CAMAR)) {
                            events::ModifyCamera action(ecs::TYPECAMERACOMPONENT, entityID, rbc, oldValC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValC = *rbc;
                        }
                    }
                }
                

            }
            if (EntitySignature.test(ecs::TYPESCRIPTCOMPONENT)) {

                open = ImGui::CollapsingHeader("Script Component");

                CreateContext(ecs::TYPESCRIPTCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPESCRIPTCOMPONENT)) {
                    auto* sc = static_cast<ecs::ScriptComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(entityID));
                    //static ecs::ScriptComponent oldValS = *sc;
                    //DrawComponents toDraw(sc->Names());
                    for (const auto& scriptname : sc->m_scripts)
                    {
                        //print out varaibles
                        scripteditor::ScriptEditor::DisplayScriptComponents(std::get<0>(scriptname), entityID);
                    }

                    
                    if (ImGui::BeginListBox("Scripts"))
                    {
                        int n{};
                        for (auto& scriptname : sc->m_scripts)
                        {

                            if (std::get<1>(scriptname)) {
                                ImGui::Selectable(std::get<0>(scriptname).c_str());
                            }
                            else {
                                ImGui::Selectable(std::string(std::get<0>(scriptname) + " (Disabled)").c_str());
                            }
                            
                            if (ImGui::BeginPopupContextItem()) {

                                if (std::get<1>(scriptname)) {
                                    if (ImGui::MenuItem("Disable Script")) {
                                        std::get<1>(scriptname) = false;

                                        ImGui::EndPopup();


                                        break;
                                    }
                                }
                                else {
                                    if (ImGui::MenuItem("Enable Script")) {
                                        std::get<1>(scriptname) = true;

                                        ImGui::EndPopup();


                                        break;
                                    }
                                }


                                if (ImGui::MenuItem("Delete Script")) {
                                    sc->m_scripts.erase(std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](const auto& x) {return std::get<0>(x) == std::get<0>(scriptname);}));

                                    ImGui::EndPopup();


                                    break;
                                }
                                ImGui::EndPopup();
                            }


                            if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
                            {
                                int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
                                if (n_next >= 0 && n_next < sc->m_scripts.size())
                                {

                                    std::swap(sc->m_scripts[n], sc->m_scripts[n_next]);
                                    ImGui::ResetMouseDragDelta();
                                }
                            }


                            n++;
                        }
                        ImGui::EndListBox();
                    }
                   
                    std::string preview = "Add Scripts";
                    if (ImGui::BeginCombo("####add scrip", preview.c_str()))
                    {
                        for (const auto& scriptname : assetManager->m_scriptManager.m_CSScripts) {



                            const bool is_selected{};
                            if (ImGui::Selectable(scriptname.first.c_str(), is_selected)) {
                                //TODO for now push back
                                if (std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](const auto& x) {return std::get<0>(x) == scriptname.first; }) == sc->m_scripts.end()) {
                                    sc->m_scripts.push_back(std::make_tuple(scriptname.first, true, std::unordered_map<std::string, std::string>{}));
                                }
                                else {
                                    LOGGING_WARN("Script is already inside Object");
                                }
                                

                            }

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    }


                }


            }

            if (EntitySignature.test(ecs::TYPEBUTTONCOMPONENT)) {

                open = ImGui::CollapsingHeader("Button Component");

                CreateContext(ecs::TYPEBUTTONCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEBUTTONCOMPONENT)) {
                    auto* rbc = static_cast<ecs::ButtonComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEBUTTONCOMPONENT]->m_GetEntityComponent(entityID));
                    //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                    static ecs::ButtonComponent oldValB = *rbc;
                    DrawComponents toDraw(rbc->Names());
                    toDraw.count++;
                    //if (toDraw(rbc->m_Position)) {
                    //    //if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::BUTTONPOS)) {
                    //    //    oldValB = *rbc;
                    //    //}
                    //    //if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::BUTTON, dragfloat::Member::BUTTONPOS)) {
                    //    //    events::ModifyButton action(ecs::TYPEBUTTONCOMPONENT, entityID, rbc, oldValB);
                    //    //    ////DISPATCH_ACTION_EVENT(action);
                    //    //    oldValB = *rbc;
                    //    //}
                    //}
                    if (toDraw(rbc->m_Scale)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::BUTTONSCALE)) {
                            oldValB = *rbc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::BUTTON, dragfloat::Member::BUTTONSCALE)) {
                            events::ModifyButton action(ecs::TYPEBUTTONCOMPONENT, entityID, rbc, oldValB);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValB = *rbc;
                        }
                    }
                    if (toDraw(rbc->m_IsClick)) {
                        events::ModifyButton action(ecs::TYPEBUTTONCOMPONENT, entityID, rbc, oldValB);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValB = *rbc;
                    }
                    if (toDraw(rbc->m_EnableHover)) {

                    }
                    if (toDraw(rbc->m_IsHover)) {

                    }

                }


            }

           

            if (EntitySignature.test(ecs::TYPETILEMAPCOMPONENT)) {

                open = ImGui::CollapsingHeader("Tilemap Component");

                CreateContext(ecs::TYPETILEMAPCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPETILEMAPCOMPONENT)) {

                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* tmc = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(entityID));
                    //auto* transform = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID));
                    static ecs::TilemapComponent oldValTM = *tmc;
                    DrawComponents toDraw(tmc->Names());


                    if (ImGui::BeginCombo("Tilemaps", tmc->m_tilemapFile.c_str()))
                    {
                        for (const auto& image : Asset->m_imageManager.m_imageMap) {

                            if (ImGui::Selectable(image.first.c_str())) {
                                tmc->m_tilemapFile = image.first.c_str();
                                events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, tmc, oldValTM);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValTM = *tmc;
                            }
                        }
                        ImGui::EndCombo();
                    }

                    if (ImGui::BeginDragDropTarget())
                    {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                            if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {
                                if (Asset->m_imageManager.m_imageMap.find(filename->filename().string()) == Asset->m_imageManager.m_imageMap.end()) {
                                    LOGGING_WARN("File not loaded, please reload content browser");
                                }
                                else {
                                    tmc->m_tilemapFile = filename->filename().string();
                                }
                            }
                            else {
                                LOGGING_WARN("Wrong File Type");
                            }
                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImVec4 color = ImVec4(tmc->m_color.m_x, tmc->m_color.m_y, tmc->m_color.m_z, tmc->m_alpha);

                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3("##MyColor3", (float*)&color, ImGuiColorEditFlags_DisplayRGB))
                    {
                        tmc->m_color.m_x = color.x;
                        tmc->m_color.m_y = color.y;
                        tmc->m_color.m_z = color.z;
                    }
                    if (ImGui::IsItemActivated()) {
                        events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, tmc, oldValTM);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValTM = *tmc;
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, tmc, oldValTM);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValTM = *tmc;
                    }


                    if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPETILEMAPCOMPONENT)) {
                        auto* rbc = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(entityID));
                        //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                        if (toDraw(tmc->m_tileIndex)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TILEMAPIDX)) {
                                oldValTM = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TILEMAP, dragfloat::Member::TILEMAPIDX)) {
                                events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, rbc, oldValTM);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValTM = *rbc;
                            }
                        }
                        if (toDraw(tmc->m_tileLayer)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TILEMAPLAYER)) {
                                oldValTM = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TILEMAP, dragfloat::Member::TILEMAPLAYER)) {
                                events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, rbc, oldValTM);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValTM = *rbc;
                            }
                        }
                        if (toDraw(tmc->m_rowLength)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TILEMAPROW)) {
                                oldValTM = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TILEMAP, dragfloat::Member::TILEMAPROW)) {
                                events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, rbc, oldValTM);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValTM = *rbc;
                            }
                        }
                        if (toDraw(tmc->m_columnLength)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TILEMAPCOL)) {
                                oldValTM = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TILEMAP, dragfloat::Member::TILEMAPCOL)) {
                                events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, rbc, oldValTM);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValTM = *rbc;
                            }
                        }
                        if (toDraw(tmc->m_pictureRowLength)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TILEMAPPICROW)) {
                                oldValTM = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TILEMAP, dragfloat::Member::TILEMAPPICROW)) {
                                events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, rbc, oldValTM);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValTM = *rbc;
                            }
                        }
                        if (toDraw(tmc->m_pictureColumnLength)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::TILEMAPPICCOL)) {
                                oldValTM = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::TILEMAP, dragfloat::Member::TILEMAPPICCOL)) {
                                events::ModifyTilemap action(ecs::TYPETILEMAPCOMPONENT, entityID, rbc, oldValTM);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValTM = *rbc;
                            }
                        }
                    }
                    Tilemap::resizeTiles(tmc, tmc->m_rowLength, tmc->m_columnLength);

                    if (ImGui::Button("Pick Tile"))
                     {
                         m_tilePickerMode = true;
                         m_collisionSetterMode = false;
                     }

                    /*if (ImGui::Button("Set Colliders"))
                    {
                        m_tilePickerMode = false;
                        m_collisionSetterMode = true;
                    }*/
                    //Tilemap::debugTileIndex(tmc);

                    //std::cout << EditorCamera::calculateWorldCoordinatesFromMouse(ImGui::GetMousePos().x, ImGui::GetMousePos().y).m_y << std::endl;

                }


            }

            if (EntitySignature.test(ecs::TYPEGRIDCOMPONENT)) {

                open = ImGui::CollapsingHeader("Grid Component");

                CreateContext(ecs::TYPEGRIDCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEGRIDCOMPONENT)) {

                    auto* grid = static_cast<ecs::GridComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(entityID));
                    //auto* transform = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityID)); //unused
                    
                    if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEGRIDCOMPONENT)) {
                        auto* rbc = static_cast<ecs::GridComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(entityID));
                        //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                        static ecs::GridComponent oldValG = *rbc;
                        DrawComponents toDraw(rbc->Names());
                        if (toDraw(rbc->m_Anchor)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::GRIDANCHOR)) {
                                oldValG = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::GRID, dragfloat::Member::GRIDANCHOR)) {
                                events::ModifyGrid action(ecs::TYPEGRIDCOMPONENT, entityID, rbc, oldValG);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValG = *rbc;
                            }
                        }
                        if (toDraw(rbc->m_GridRowLength)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::GRIDROW)) {
                                oldValG = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::GRID, dragfloat::Member::GRIDROW)) {
                                events::ModifyGrid action(ecs::TYPEGRIDCOMPONENT, entityID, rbc, oldValG);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValG = *rbc;
                            }
                        }
                        if (toDraw(rbc->m_GridColumnLength)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::GRIDCOL)) {
                                oldValG = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::GRID, dragfloat::Member::GRIDCOL)) {
                                events::ModifyGrid action(ecs::TYPEGRIDCOMPONENT, entityID, rbc, oldValG);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValG = *rbc;
                            }
                        }
                        if (toDraw(rbc->m_SetCollidable)) {
                            events::ModifyGrid action(ecs::TYPEGRIDCOMPONENT, entityID, rbc, oldValG);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValG = *rbc;
                         
                        }
                        if (toDraw(rbc->m_GridKey)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::GRIDKEY)) {
                                oldValG = *rbc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::GRID, dragfloat::Member::GRIDKEY)) {
                                events::ModifyGrid action(ecs::TYPEGRIDCOMPONENT, entityID, rbc, oldValG);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValG = *rbc;
                            }
                        }
                    }
                    Tilemap::resizeCollidableGrid(grid, grid->m_GridRowLength, grid->m_GridColumnLength);

                    if (ImGui::Button("Set Colliders"))
                    {
                        m_tilePickerMode = false;
                        m_collisionSetterMode = true;
                    }
                }
            }

            if (EntitySignature.test(ecs::TYPEAUDIOCOMPONENT)) {
                open = ImGui::CollapsingHeader("Audio Component");

                CreateContext(ecs::TYPEAUDIOCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEAUDIOCOMPONENT)) {
                    auto* ac = static_cast<ecs::AudioComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityID));
                    //assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
                    static ecs::AudioComponent oldValAU = *ac;
                    if (ac) {
                        int fileIndex = 0;
                        for (auto it2 = ac->m_AudioFiles.begin(); it2 != ac->m_AudioFiles.end();) {
                            ImGui::PushID(fileIndex);

                            bool removeFile = false;
                            

                            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                            std::string headerName = "Audio File " + std::to_string(fileIndex + 1) + ": " + it2->m_Name;
                            bool nodeOpen = ImGui::TreeNodeEx(headerName.c_str(), flags);

                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                ImGui::OpenPopup("AudioContextMenu");
                            }

                            if (ImGui::BeginPopup("AudioContextMenu")) {
                                if (ImGui::MenuItem("Remove")) {
                                    removeFile = true;
                                }
                                ImGui::EndPopup();
                            }

                            if (nodeOpen) {
                                char buffer[256];
                                strncpy(buffer, it2->m_FilePath.c_str(), sizeof(buffer));


                                if (ImGui::BeginCombo("Sounds", it2->m_Name.c_str())) {
                                    for (const auto& sound : assetManager->m_audioManager.getSoundMap()) {
                                        if (ImGui::Selectable(sound.first.c_str(), sound.first == it2->m_Name)) {
                                            if (sound.first != it2->m_Name) {
                                                auto& audioManager = assetManager->m_audioManager;

                                                if (audioManager.m_IsPlayingForEntity(entityID, it2->m_Name)) {
                                                    audioManager.m_StopAudioForEntity(entityID, it2->m_Name);
                                                }

                                                if (audioManager.getSoundMap().find(sound.first) == audioManager.getSoundMap().end()) {
                                                    LOGGING_WARN("Selected sound not found in the audio manager.");
                                                    continue;
                                                }

                                                it2->m_Name = sound.first;
                                            }
                                        }
                                    }
                                    ImGui::EndCombo();
                                }


                                if (ImGui::BeginDragDropTarget()) {
                                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file")) {
                                        IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                                        std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                                        if (filename->filename().extension().string() == ".ogg" || ".wav") {
                                            if (assetManager->m_audioManager.getSoundMap().find(filename->filename().string()) == assetManager->m_audioManager.getSoundMap().end()) {
                                                LOGGING_WARN("File not loaded, please reload content browser");
                                            }
                                            else {
                                                assetManager->m_LoadAudio(filename->filename().string());
                                            }
                                        }
                                        else {
                                            LOGGING_WARN("Wrong File Type");
                                        }
                                    }
                                    ImGui::EndDragDropTarget();
                                }

                                ImGui::SliderFloat("Volume", &it2->m_Volume, 0.0f, 1.0f);
                                assetManager->m_audioManager.m_SetVolumeForEntity(entityID, it2->m_Name, it2->m_Volume);

                                ImGui::SliderFloat("Pan", &it2->m_Pan, -1.0f, 1.0f);
                                assetManager->m_audioManager.m_SetPanForEntity(entityID, it2->m_Name, it2->m_Pan);

                                bool wasLooping = it2->m_Loop;

                                if (ImGui::Button("Play Sound")) {
                                    std::string key = it2->m_Name;
                                    auto& audioManager = assetManager->m_audioManager;
                                    if (!audioManager.m_IsPlayingForEntity(entityID, key)) {
                                        audioManager.m_PlayAudioForEntity(entityID, key, it2->m_Volume);
                                    }
                                    else {
                                        audioManager.m_StopAudioForEntity(entityID, it2->m_Name);   
                                        audioManager.m_PlayAudioForEntity(entityID, key, it2->m_Volume);
                                    }
                                }
    
                                if (ImGui::Checkbox("Play On Start", &it2->m_PlayOnStart)) {
                                    auto& audioManager = assetManager->m_audioManager;

                                    if (it2->m_PlayOnStart) {
                                        for (auto& audioFile : ac->m_AudioFiles) {
                                            if (&audioFile != &(*it2)) {
                                                audioFile.m_PlayOnStart = false;
                                            }
                                        }

                                        audioManager.m_StopAudioForEntity(entityID, it2->m_Name);

                                    }
                                    else {
                                        audioManager.m_StopAudioForEntity(entityID, it2->m_Name);
                                    }

                                    audioManager.m_SetPlayOnStartForEntity(entityID, it2->m_Name, it2->m_PlayOnStart);
                                }

                                if (ImGui::Checkbox("Loop", &it2->m_Loop)) {
                                    auto& audioManager = assetManager->m_audioManager;

                                    audioManager.m_SetLoopingForEntity(entityID, it2->m_Name, it2->m_Loop);

                                    if (it2->m_Loop != wasLooping && audioManager.m_IsPlayingForEntity(entityID, it2->m_Name)) {
                                        audioManager.m_StopAudioForEntity(entityID, it2->m_Name);
                                        audioManager.m_PlayAudioForEntity(entityID, it2->m_Name, it2->m_Volume);
                                    }
                                }

                                if (ImGui::Checkbox("isBGM", &it2->m_IsBGM)) {
                                    auto& audioManager = assetManager->m_audioManager;

                                    audioManager.m_SetIsBGMForEntity(entityID, it2->m_Name, it2->m_IsBGM);
                                    //std::cout << it2->m_IsBGM << std::endl;
                                }

                                if (ImGui::Checkbox("isSFX", &it2->m_IsSFX)) {
                                    auto& audioManager = assetManager->m_audioManager;

                                    audioManager.m_SetIsSFXForEntity(entityID, it2->m_Name, it2->m_IsSFX);
                                    //std::cout << it2->m_IsSFX << std::endl;

                                }


                                bool isPaused = false;
                                auto& audioManager = assetManager->m_audioManager;
                                if (audioManager.getSoundMap().find(it2->m_Name) != audioManager.getSoundMap().end()) {
                                    auto& sound = audioManager.getSoundMap()[it2->m_Name];

                                    if (sound->m_GetChannelForEntity(entityID)) {
                                        sound->m_GetChannelForEntity(entityID)->getPaused(&isPaused);
                                    }

                                    if (ImGui::Checkbox("Pause Sound", &isPaused)) {
                                        if (isPaused) {
                                            audioManager.m_PauseAudioForEntity(entityID, it2->m_Name);
                                        }
                                        else {
                                            audioManager.m_UnpauseAudioForEntity(entityID, it2->m_Name);
                                        }
                                    }
                                }

                                if (ImGui::Button("Stop Sound")) {
                                    std::string key = it2->m_Name;
                                    auto& audioManager2 = assetManager->m_audioManager;
                                    audioManager2.m_StopAudioForEntity(entityID, key);
                                }



                                ImGui::TreePop();
                            }

                            ImGui::PopID();

                            if (removeFile) {
                                std::string key = it2->m_Name;
                                auto& audioManager = assetManager->m_audioManager;
                                audioManager.m_StopAudioForEntity(entityID, key);
                                it2 = ac->m_AudioFiles.erase(it2);
                                events::RemoveAudio action(ecs::TYPEAUDIOCOMPONENT, entityID, ac, oldValAU);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValAU = *ac;
                            }
                            else {
                                ++it2;
                                ++fileIndex;
                            }
                        }

                        ImGui::Separator();

                        static char newAudioName[256] = "";
                        if (ImGui::Button("Add Audio File")) {
                            ac->m_AudioFiles.emplace_back();
                            events::AddAudio action(ecs::TYPEAUDIOCOMPONENT, entityID, ac, oldValAU);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValAU = *ac;
                        }

                        if (ImGui::Button("Stop All Sounds"))
                        {
                            auto& audioManager = assetManager->m_audioManager;
                            audioManager.m_StopAllSounds();
                        }
                    }
                }
            }
             if (EntitySignature.test(ecs::TYPELIGHTINGCOMPONENT)) {

                open = ImGui::CollapsingHeader("Lighting Component");

                CreateContext(ecs::TYPELIGHTINGCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPELIGHTINGCOMPONENT)) {
                    auto* lc = static_cast<ecs::LightingComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPELIGHTINGCOMPONENT]->m_GetEntityComponent(entityID));
                    //lc->ApplyFunction(DrawComponents(lc->Names()));
                    static ecs::LightingComponent oldValL = *lc;
                    DrawComponents toDraw(lc->Names());
                    if (toDraw(lc->m_innerOuterRadius)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTRAD)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTRAD)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    if (toDraw(lc->m_intensity)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTINTENSE)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTINTENSE)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    if (toDraw(lc->m_light_OffSet)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTOFFSET)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTOFFSET)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    if (toDraw(lc->m_light_scale)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTSCALE)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTSCALE)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    if (toDraw(lc->m_light_rotation)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTROT)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTROT)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    if (toDraw(lc->m_lightType)) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTTYPE)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTTYPE)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    

                    ImVec4 color = ImVec4(lc->m_colour.m_x, lc->m_colour.m_y, lc->m_colour.m_z, 1.f);

                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider

                    //ImGui::SameLine();
                    if (ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
                    {
                        lc->m_colour.m_x = color.x;
                        lc->m_colour.m_y = color.y;
                        lc->m_colour.m_z = color.z;
                    }
                    if (ImGui::IsItemActivated()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTCLR)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTCLR)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::LIGHTCLR)) {
                            oldValL = *lc;
                        }
                        if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::LIGHT, dragfloat::Member::LIGHTCLR)) {
                            events::ModifyLight action(ecs::TYPELIGHTINGCOMPONENT, entityID, lc, oldValL);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValL = *lc;
                        }
                    }
                    ImGui::SameLine();
                    ImGui::Text("Color");

                }

            }

             if (EntitySignature.test(ecs::TYPEPARTICLECOMPONENT)) {

                 open = ImGui::CollapsingHeader("Particle Component");

                 CreateContext(ecs::TYPEPARTICLECOMPONENT, entityID);

                 if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEPARTICLECOMPONENT)) {
                     auto* rbc = static_cast<ecs::ParticleComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entityID));
                     //rbc->ApplyFunction(DrawComponents(rbc->Names()));
                     static ecs::ParticleComponent oldValP = *rbc;
                     DrawComponents toDraw(rbc->Names());
                     if (toDraw(rbc->m_willSpawn)) {
                        events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValP = *rbc;
                     }
                     if (toDraw(rbc->m_noOfParticles)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLENUM)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLENUM)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_lifeSpan)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLENUM)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLENUM)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_particleSize)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLENUM)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLENUM)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_velocity)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLENUM)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLENUM)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_acceleration)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLEACCEL)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLEACCEL)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_coneRotation)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLECONEROT)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLECONEROT)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_coneAngle)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLENUM)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLENUM)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_randomFactor)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLERAND)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLERAND)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_stripCount)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLESTRIP)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLESTRIP)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_frameNumber)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLEFRAMENUM)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLEFRAMENUM)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_layer)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLELAYER)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLELAYER)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_friction)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLEFRICTION)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLEFRICTION)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (toDraw(rbc->m_fps)) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLEFPS)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLEFPS)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }

                     if (toDraw(rbc->m_loopAnimation)) {
                         events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                         ////DISPATCH_ACTION_EVENT(action);
                         oldValP = *rbc;
                     }

                     ImVec4 color = ImVec4(rbc->m_color.m_x, rbc->m_color.m_y, rbc->m_color.m_z, 1.f);

                     ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider

                     ImGui::SameLine();
                     if (ImGui::ColorEdit4("MyColor##4", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
                     {
                         rbc->m_color.m_x = color.x;
                         rbc->m_color.m_y = color.y;
                         rbc->m_color.m_z = color.z;
                     }
                     if (ImGui::IsItemActivated()) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLECLR)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLECLR)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     if (ImGui::IsItemDeactivatedAfterEdit()) {
                         if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PARTICLECLR)) {
                             oldValP = *rbc;
                         }
                         if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PARTICLE, dragfloat::Member::PARTICLECLR)) {
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                         }
                     }
                     ImGui::SameLine();
                     ImGui::Text("Color");
                     ImGui::SetNextItemWidth(100.0f);
                     if (ImGui::BeginCombo("Images", rbc->m_imageFile.c_str()))
                     {
                         assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                         for (const auto& image : Asset->m_imageManager.m_imageMap) {

                             if (ImGui::Selectable(image.first.c_str())) 
                             {
                                 rbc->m_imageFile = image.first.c_str();
                                 events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                                 ////DISPATCH_ACTION_EVENT(action);
                                 oldValP = *rbc;
                             }
                         }
                         ImGui::EndCombo();
                     }
       
                     static bool resetImage = false; 
                     if (ImGui::Checkbox("Reset Image", &resetImage))
                     {
                         if (resetImage)
                         {
                             rbc->m_imageFile.clear();
                             events::ModifyParticle action(ecs::TYPEPARTICLECOMPONENT, entityID, rbc, oldValP);
                             ////DISPATCH_ACTION_EVENT(action);
                             oldValP = *rbc;
                             resetImage = false;
                         }
                     }

                 }


            }
            if (EntitySignature.test(ecs::TYPERAYCASTINGCOMPONENT)) {

                open = ImGui::CollapsingHeader("Raycast Component");

                CreateContext(ecs::TYPERAYCASTINGCOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPERAYCASTINGCOMPONENT)) {
                    auto* rcc = static_cast<ecs::RaycastComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERAYCASTINGCOMPONENT]->m_GetEntityComponent(entityID));
                    static ecs::RaycastComponent oldValRC = *rcc;
                    if (ImGui::Button("+ Add Ray")) {
                        rcc->m_raycast.push_back(ecs::RaycastComponent::Raycast{});
                        events::AddRay action(ecs::TYPERAYCASTINGCOMPONENT, entityID, rcc, oldValRC);
                        ////DISPATCH_ACTION_EVENT(action);
                        oldValRC = *rcc;
                    }
                    ImGui::Separator();


                    int _count{};
                    for (auto it2 = rcc->m_raycast.begin(); it2 != rcc->m_raycast.end(); it2++) {
                        ImGui::PushID(_count);
                        it2->ApplyFunction(DrawComponents(it2->Names()));

                        if (ImGui::Button("+ Layer")) {
                            it2->m_Layers.push_back((layer::LAYERS)0);
                        }

                        ImGui::SameLine();
                        if (it2->m_Layers.size() > 0 && ImGui::Button("- Layer")) {
                            it2->m_Layers.pop_back();
                            ImGui::PopID();
                            break;

                        }
                        ImGui::SameLine();
                        if (ImGui::Button(" Delete Ray")) {
                            rcc->m_raycast.erase(it2);
                            events::RemoveRay action(ecs::TYPERAYCASTINGCOMPONENT, entityID, rcc, oldValRC);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValRC = *rcc;
                            ImGui::PopID();
                            break;
                        }

                        ImGui::PopID();
                        _count++;

                        ImGui::Separator();

                    }
                }
            }

            if (EntitySignature.test(ecs::TYPEPATHFINDINGCOMPONENT)) {
                bool openPC = ImGui::CollapsingHeader("Pathfinding Component");

                CreateContext(ecs::TYPEPATHFINDINGCOMPONENT, entityID);

                if (openPC && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEPATHFINDINGCOMPONENT)) {
                    auto* pfc = static_cast<ecs::PathfindingComponent*>(
                        ecs->m_ECS_CombinedComponentPool[ecs::TYPEPATHFINDINGCOMPONENT]->m_GetEntityComponent(entityID)
                        );

                    if (pfc) {


                        //pfc->ApplyFunction(DrawComponents(pfc->Names()));
                        static ecs::PathfindingComponent oldValPF = *pfc;
                        DrawComponents toDraw(pfc->Names());
                        if (toDraw(pfc->m_StartPos)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PATHFINDINGSTART)) {
                                oldValPF = *pfc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PATHFINDING, dragfloat::Member::PATHFINDINGSTART)) {
                                events::ModifyPathfinding action(ecs::TYPEPATHFINDINGCOMPONENT, entityID, pfc, oldValPF);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValPF = *pfc;
                            }
                        }
                        if (toDraw(pfc->m_TargetPos)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PATHFINDINGEND)) {
                                oldValPF = *pfc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PATHFINDING, dragfloat::Member::PATHFINDINGEND)) {
                                events::ModifyPathfinding action(ecs::TYPEPATHFINDINGCOMPONENT, entityID, pfc, oldValPF);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValPF = *pfc;
                            }
                        }
                        if (toDraw(pfc->m_GridKey)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::PATHFINDINGKEY)) {
                                oldValPF = *pfc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::PATHFINDING, dragfloat::Member::PATHFINDINGKEY)) {
                                events::ModifyPathfinding action(ecs::TYPEPATHFINDINGCOMPONENT, entityID, pfc, oldValPF);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValPF = *pfc;
                            }
                        }

                    }
                }
            }


            if (EntitySignature.test(ecs::TYPEVIDEOCOMPONENT)) {
                bool openPC = ImGui::CollapsingHeader("Video Component");

                CreateContext(ecs::TYPEVIDEOCOMPONENT, entityID);

                if (openPC && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEVIDEOCOMPONENT)) {
                    auto* vc = static_cast<ecs::VideoComponent*>(
                        ecs->m_ECS_CombinedComponentPool[ecs::TYPEVIDEOCOMPONENT]->m_GetEntityComponent(entityID)
                        );
                    static ecs::VideoComponent oldValV = *vc;
                    DrawComponents toDraw(vc->Names());

                    if (vc) {

                        if (vc->play == false) {
                            if (ImGui::Button("Play")) {
                                vc->play = true;
                                events::ModifyVideo action(ecs::TYPEVIDEOCOMPONENT, entityID, vc, oldValV);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValV = *vc;
                            }
                        }
                        else {
                            if (ImGui::Button("Stop")) {
                                vc->play = false;
                                events::ModifyVideo action(ecs::TYPEVIDEOCOMPONENT, entityID, vc, oldValV);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValV = *vc;
                            }
                        }

                        //vc->ApplyFunction(DrawComponents(vc->Names()));

                        std::string preview = vc->filename;
                        toDraw.count++;
                        ImGui::Text("Filename");
                        ImGui::SameLine();
                        if (ImGui::BeginCombo("####add video", preview.c_str()))
                        {
                            for (const auto& videoname : assetManager->m_videoManager.m_videopath) {



                                const bool is_selected{};
                                if (ImGui::Selectable(videoname.first.c_str(), is_selected)) {
                                    //TODO for now push back
                                    vc->filename = videoname.first;
                                    events::ModifyVideo action(ecs::TYPEVIDEOCOMPONENT, entityID, vc, oldValV);
                                    ////DISPATCH_ACTION_EVENT(action);
                                    oldValV = *vc;


                                }

                                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }

                            ImGui::EndCombo();
                        }


                        //if (toDraw(vc->filename)) {
                        //    events::ModifyVideo action(ecs::TYPEVIDEOCOMPONENT, entityID, vc, oldValV);
                        //    ////DISPATCH_ACTION_EVENT(action);
                        //    oldValV = *vc;
                        //}
                        if (toDraw(vc->pause)) {
                            events::ModifyVideo action(ecs::TYPEVIDEOCOMPONENT, entityID, vc, oldValV);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValV = *vc;
                        }
                        if (toDraw(vc->loop)) {
                            events::ModifyVideo action(ecs::TYPEVIDEOCOMPONENT, entityID, vc, oldValV);
                            ////DISPATCH_ACTION_EVENT(action);
                            oldValV = *vc;
                        }
                        if (toDraw(vc->layer)) {
                            if ((dragfloat::DragFloatCheck::m_GetInstance()->m_GetPrevMem() != dragfloat::Member::VIDEOLAYER)) {
                                oldValV = *vc;
                            }
                            if (dragfloat::DragFloatCheck::m_GetInstance()->m_Click(dragfloat::Comp::VIDEO, dragfloat::Member::VIDEOLAYER)) {
                                events::ModifyVideo action(ecs::TYPEVIDEOCOMPONENT, entityID, vc, oldValV);
                                ////DISPATCH_ACTION_EVENT(action);
                                oldValV = *vc;
                            }
                        }

                    }
                }
            }
            
            if (EntitySignature.test(ecs::TYPEUISPRITECOMPONENT)) {
                open = ImGui::CollapsingHeader("UI Sprite Component");

                CreateContext(ecs::TYPEUISPRITECOMPONENT, entityID);

                if (open && ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEUISPRITECOMPONENT)) {

                    assetmanager::AssetManager* Asset = assetmanager::AssetManager::m_funcGetInstance();
                    auto* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEUISPRITECOMPONENT]->m_GetEntityComponent(entityID));
                    static ecs::SpriteComponent oldValS = *sc;


                    if (ImGui::BeginCombo("Images##UI1234", sc->m_imageFile.c_str()))
                    {
                        for (const auto& image : Asset->m_imageManager.m_imageMap) {

                            if (ImGui::Selectable(image.first.c_str())) {
                                //static ecs::SpriteComponent oldValA = *sc;
                                sc->m_imageFile = image.first.c_str();
                                if (!ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT))
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    //graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                                    if (assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount > 1)
                                    {

                                        ecs::AnimationComponent* com = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID));
                                        com->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                        events::ModifySprite action2(ecs::TYPEUISPRITECOMPONENT, entityID, sc, oldValS);
                                        //DISPATCH_ACTION_EVENT(action2);
                                        events::AddComponent action1(entityID, ecs::TYPEANIMATIONCOMPONENT);
                                        //DISPATCH_ACTION_EVENT(action1);
                                        oldValS = *sc;

                                    }
                                }
                                else
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    ecs::AnimationComponent* com = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                                    com->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                    //static ecs::AnimationComponent oldValA2 = *com;
                                    //events::ModifyAnim action(ecs::TYPESPRITECOMPONENT, entityID, com, oldValA2);
                                    //////DISPATCH_ACTION_EVENT(action);
                                    events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                                    ////DISPATCH_ACTION_EVENT(action);
                                    oldValS = *sc;
                                }
                            }
                        }
                        ImGui::EndCombo();
                    }

                    if (ImGui::BeginDragDropTarget())
                    {

                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                        {
                            IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                            std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);
                            if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {
                                if (Asset->m_imageManager.m_imageMap.find(filename->filename().string()) == Asset->m_imageManager.m_imageMap.end()) {
                                    LOGGING_WARN("File not loaded, please reload content browser");
                                }
                                else {
                                    sc->m_imageFile = filename->filename().string();
                                }
                            }
                            else {
                                LOGGING_WARN("Wrong File Type");
                            }



                        }
                        ImGui::EndDragDropTarget();
                    }

                    ImVec4 color = ImVec4(sc->m_color.m_x, sc->m_color.m_y, sc->m_color.m_z, sc->m_alpha);

                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Color");
                    ImGui::SameLine();
                    if (ImGui::ColorEdit3("##1234MyColor2", (float*)&color, ImGuiColorEditFlags_DisplayRGB))
                    {
                        sc->m_color.m_x = color.x;
                        sc->m_color.m_y = color.y;
                        sc->m_color.m_z = color.z;
                        //events::ModifySprite action(ecs::TYPESPRITECOMPONENT, entityID, sc, oldValS);
                        //////DISPATCH_ACTION_EVENT(action);
                    }
                  
                    ImGui::AlignTextToFramePadding();  // Aligns text to the same baseline as the slider
                    ImGui::Text("Layer");
                    ImGui::SameLine();
                    if (ImGui::InputInt("Layer##9127391823", &sc->m_layer))
                    {
                    }
                }
            }

            //draw invinsible box
            if (ImGui::GetContentRegionAvail().x > 0 && ImGui::GetContentRegionAvail().y > 0){
                
                ImGui::InvisibleButton("##Invinsible", ImVec2{ ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y });

                if (ImGui::BeginDragDropTarget())
                {

                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("file"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(std::filesystem::path));
                        std::filesystem::path* filename = static_cast<std::filesystem::path*>(payload->Data);

                        if (filename->filename().extension().string() == ".png" || filename->filename().extension().string() == ".jpg") {

                            if (!EntitySignature.test(ecs::TYPESPRITECOMPONENT)) {// does not have sprite component, create one
                                ecs::SpriteComponent* com = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, entityID));
                                com->m_imageFile = filename->filename().string();
                                if (!ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT))
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    //graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                                    if (assets->m_imageManager.m_imageMap[com->m_imageFile].m_stripCount > 1)
                                    {
                                        ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID));
                                        ani->m_stripCount = assets->m_imageManager.m_imageMap[com->m_imageFile].m_stripCount;
                                    }
                                }
                                else
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                                    ani->m_stripCount = assets->m_imageManager.m_imageMap[com->m_imageFile].m_stripCount;
                                }
                            }
                            else {
                                auto* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityID));
                                sc->m_imageFile = filename->filename().string();
                                if (!ecs->m_ECS_EntityMap[entityID].test(ecs::TYPEANIMATIONCOMPONENT))
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    //graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
                                    if (assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount > 1)
                                    {
                                        ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, entityID));
                                        ani->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                    }
                                }
                                else
                                {
                                    assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
                                    ecs::AnimationComponent* ani = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityID));
                                    ani->m_stripCount = assets->m_imageManager.m_imageMap[sc->m_imageFile].m_stripCount;
                                }
                            }
                        }

                        if (filename->filename().extension().string() == ".ttf") {

                            if (!EntitySignature.test(ecs::TYPETEXTCOMPONENT)) {// does not have sprite component, create one
                                ecs::TextComponent* com = static_cast<ecs::TextComponent*>(ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, entityID));
                                com->m_fileName = filename->filename().string();
                            }
                            else {
                                auto* sc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityID));
                                sc->m_fileName = filename->filename().string();
                            }
                        }

                        if (filename->filename().extension().string() == ".mpg" || filename->filename().extension().string() == ".mpeg") {

                            if (!EntitySignature.test(ecs::TYPEVIDEOCOMPONENT)) {// does not have sprite component, create one
                                ecs::VideoComponent* vid = static_cast<ecs::VideoComponent*>(ecs->m_AddComponent(ecs::TYPEVIDEOCOMPONENT, entityID));
                                vid->filename = filename->filename().string();
                                vid->play = true;
                            }
                            else {
                                auto* vc = static_cast<ecs::VideoComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEVIDEOCOMPONENT]->m_GetEntityComponent(entityID));
                                vc->filename = filename->filename().string();
                                vc->play = true;
                            }
                        }

                    }
                    ImGui::EndDragDropTarget();
                }
            }
        }

     }
    ImGui::End();
}   
