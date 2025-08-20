/*!
\file      imgui_test_panel.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the imgui panel that has buttons that will be used for testing purposes

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
#include "Helper/Helper.h"
#include "De&Serialization/json_handler.h"
#include "Debugging/Logging.h"
#include "Asset Manager/AssetManager.h"
#include "Events/EventsBaseMessage.h"
#include "Events/MessagingMessageSystem.h"
#include "Events/EventsListeners.h"
#include "Events/EventsEventHandler.h"
#include "Inputs/Input.h"



void gui::ImGuiHandler::m_DrawTestWindow() {
	
	//bool clicked = false;
	static int maxTime = 60;
	static int currTime = 0;

	ecs::ECS *ecs = ecs::ECS::m_GetInstance();
	//assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
	logging::Logger log;

	messaging::MessageSystem MsgSys;
	MsgSys.m_AddListener(messaging::MessageType::AUDIOPLAY, messaging::SoundPlayed);
	bool open = true;
	ImGui::Begin("Test Window", &open);
	ImGui::SeparatorText(" AUDIO ");
	if (ImGui::Button("BGM") || Input::InputSystem::m_isKeyTriggered(keys::F1)) {
		events::ButtonPressEvent temp(1);
		DISPATCH_BUTTON_EVENT(temp);
		//Honestly unsure how to test unregistering cuz idk when it would be used
		//REMOVE_BUTTON_LISTENER(m_buttonID);
	}
	ImGui::SameLine();
	if (ImGui::Button("Sound1") || Input::InputSystem::m_isKeyTriggered(keys::F2)) {
		events::ButtonPressEvent temp(2);
		DISPATCH_BUTTON_EVENT(temp);
	}
	ImGui::SameLine();
	if (ImGui::Button("Sound2") || Input::InputSystem::m_isKeyTriggered(keys::F3)) {
		events::ButtonPressEvent temp(3);
		DISPATCH_BUTTON_EVENT(temp);
	}
	ImGui::SameLine();
	if (ImGui::Button("Sound3") || Input::InputSystem::m_isKeyTriggered(keys::F4)) {
		events::ButtonPressEvent temp(4);
		DISPATCH_BUTTON_EVENT(temp);
	}
	if (ImGui::Button("Stop Sound") || Input::InputSystem::m_isKeyTriggered(keys::F5)) {
		events::ButtonPressEvent temp(5);
		DISPATCH_BUTTON_EVENT(temp);
	}


	/*
		1 background + 3 fx sounds
	*/
	ImGui::NewLine();
	ImGui::SeparatorText(" LOGGING ");
	if (ImGui::Button("Crash")) {
		LOGGING_ERROR("LOL");
	}
	ImGui::SameLine();
	if (ImGui::Button("Test Log")) {
		log.m_GetInstance().m_TestingLog();
	}
	ImGui::NewLine();
	ImGui::SeparatorText(" COLLISION ");
	static bool collision_Flag = false;
	static bool delete_Flag = true;
	static ecs::EntityID id_1;
	static ecs::EntityID id_2;
	if (ImGui::Button("Collision Test")) {
		if (!collision_Flag) {
			//	//create player 
		id_1 = ecs->m_CreateEntity(m_activeScene);
		ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id_1));
		ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, id_1));
		ecs::RigidBodyComponent* rc = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, id_1));
		ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_AddComponent(ecs::TYPENAMECOMPONENT, id_1));
		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id_1))->m_entityName = "ID_1";
		ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id_1));

		tc->m_scale = { 1.f,1.f };
		tc->m_position.m_x = static_cast<float>(1.0f);
		tc->m_position.m_y = static_cast<float>(0);
		cc->m_Size = { 0.5f,0.5f };

		id_2 = ecs->m_CreateEntity(m_activeScene);
		sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id_2));
		cc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, id_2));
		rc = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, id_2));
		nc = static_cast<ecs::NameComponent*>(ecs->m_AddComponent(ecs::TYPENAMECOMPONENT, id_2));
		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id_2))->m_entityName = "ID_2";


		tc->m_scale = { 1.f,1.f };
		tc->m_position.m_x = static_cast<float>(-1.0f);
		tc->m_position.m_y = static_cast<float>(0);
		cc->m_Size = { 0.5f,0.5f };

		collision_Flag = true;
		delete_Flag = false;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		if (!delete_Flag) {
			ecs->m_DeleteEntity(id_1);
			ecs->m_DeleteEntity(id_2);
			collision_Flag = false;
			delete_Flag = true;
		}
	}
	ImGui::NewLine();
	if (collision_Flag) {
		ImGui::SeparatorText("ID 1");
		ecs::TransformComponent* tc = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id_1);
		ecs::TransformComponent* tc2 = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id_2);
		if (ImGui::Button("Rotate Left 1")) {
			tc->m_rotation += 1.f;
		}
		ImGui::SameLine();
		if (ImGui::Button("  Up 1 ")) {
			tc->m_position.m_y += 0.1f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Rotate Right 1")) {
			tc->m_rotation -= 1.f;
		}
		ImGui::NewLine();
		if (ImGui::Button("  Left 1 ")) {
			tc->m_position.m_x -= 0.1f;
		}
		ImGui::SameLine();
		if (ImGui::Button("  Down 1 ")) {
			tc->m_position.m_y -= 0.1f;
		}
		ImGui::SameLine();
		if (ImGui::Button("  Right 1 ")) {
			tc->m_position.m_x += 0.1f;
		}
		ImGui::NewLine();
		ImGui::SeparatorText("ID 2");
		if (ImGui::Button("Rotate Left 2")) {
			tc2->m_rotation += 1.f;
		}
		ImGui::SameLine();
		if (ImGui::Button("  Up 2 ")) {
			tc2->m_position.m_y += 0.1f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Rotate Right 2")) {
			tc2->m_rotation -= 1.f;
		}
		ImGui::NewLine();
		if (ImGui::Button("  Left 2 ")) {
			tc2->m_position.m_x -= 0.1f;
		}
		ImGui::SameLine();
		if (ImGui::Button("  Down 2 ")) {
			tc2->m_position.m_y -= 0.1f;
		}
		ImGui::SameLine();
		if (ImGui::Button("  Right 2 ")) {
			tc2->m_position.m_x += 0.1f;
		}
	}

	ImGui::NewLine();
	static bool font_Flag = false;
	static bool font_delete_Flag = true;
	static ecs::EntityID fontid_1;
	static ecs::EntityID fontid_2;
	ImGui::SeparatorText("FONT");
	if (ImGui::Button("Double Font Test") && !font_Flag)
	{

		fontid_1 = ecs->m_CreateEntity(m_activeScene);
		ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, fontid_1);
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_1))->m_fileName = std::string("lazy_dog.ttf");
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_1))->m_text = std::string("Hello");
		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(fontid_1))->m_entityName = std::string("Test Font 1");
		static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(fontid_1))->m_position = { -1.f,1.f };
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_1))->m_fontSize = { 10.f };


		fontid_2 = ecs->m_CreateEntity(m_activeScene);
		ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, fontid_2);
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_2))->m_fileName = std::string("RubikGlitch-Regular.ttf");
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_2))->m_text = std::string("World");
		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(fontid_2))->m_entityName = std::string("Test Font 2");
		static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(fontid_2))->m_position = { -1.f,-1.f };
		static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(fontid_2))->m_fontSize = { 10.f };
		font_Flag = true;
		font_delete_Flag = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete Font")) 
	{
		if (!font_delete_Flag) 
		{
			if (ecs->m_ECS_EntityMap.find(fontid_1) != ecs->m_ECS_EntityMap.end())
			{
				ecs::ECS::m_GetInstance()->m_DeleteEntity(fontid_1);
			}
			if (ecs->m_ECS_EntityMap.find(fontid_2) != ecs->m_ECS_EntityMap.end())
			{
				ecs::ECS::m_GetInstance()->m_DeleteEntity(fontid_2);
			}
			font_Flag = false;
			font_delete_Flag = true;
		}
	}
	ImGui::NewLine();
	ImGui::SeparatorText("2500 Entity");
	static std::vector<ecs::EntityID> listofEntity{};
	static bool entity_FlagCreate = false;
	static bool entity_FlagDelete = true;
	static bool entity_ClickOnce = false;
	if (ImGui::Button("2500 Entity")){
		if (!entity_ClickOnce) {
			if (!listofEntity.size()) {
				int lowerBoundy = -2;
				int upperBoundy = 2;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> height(static_cast<float>(lowerBoundy), static_cast<float>(upperBoundy));
				std::uniform_real_distribution<float> height2(-2.5, 2.5);
				for (size_t i = 0; i < 2500; ++i) {
					ecs::EntityID id = ecs->m_CreateEntity(m_activeScene);
					ecs::TransformComponent* tc = (ecs::TransformComponent*)ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id);
					ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, id));
					sc->m_imageFile = "broombaTest2.png";

					tc->m_scale = { 1.f,1.f };
					tc->m_position.m_y = static_cast<float>(height(gen));
					tc->m_position.m_x = static_cast<float>(height2(gen));
					listofEntity.push_back(id);
				}
			}
			else {
				LOGGING_WARN("SPAWNING MORE THAN SET MAX ENTITY WILL CAUSE ASSERTION");
			}
			entity_ClickOnce = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete 2500")) {
		if (listofEntity.size()) {
			for (size_t i = 0; i < 2500; ++i) {
				ecs->m_DeleteEntity(listofEntity[i]);
			}
			listofEntity.clear();
		}
		else {
			LOGGING_WARN("NOTHING TO DELETE");
		}
	}

	ImGui::End();
}
