/******************************************************************/
/*!
\file      json_handler.cpp
\author    Chiu Jun Jie, junjie.c , 2301524 ,
\co-author Jaz Winn Ng, jazwinn.ng, 2301502
\par       junjie.c@digipen.edu
\date      Nov 29, 2024
\brief     This file handles JSON-related operations for component serialization and deserialization in the ECS framework.
			- m_LoadConfig: Loads configuration settings such as window dimensions and FPS cap from a config file.
			- m_JsonFileValidation: Validates if the JSON file exists and creates a new one if it doesn't.
			- m_LoadComponentsJson: Loads component data from a JSON file into the ECS.
			- m_SaveComponentsJson: Saves component data from the ECS to a JSON file.
			- m_SaveEntity: Serializes individual entity data to a JSON structure.
			- m_LoadEntity: Deserializes individual entity data from a JSON structure.
			- m_LoadEntity: Deserializes individual entity data from a JSON structure.
			- m_SavePhysicsLayerMatrix: Saves the current physics layer matrix to a JSON file.
			- m_LoadPhysicsLayerMatrix: Loads the physics layer matrix from a JSON file.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
/********************************************************************/

//REFLECTION

//SAVING

//template <typename T>//T refers to component
//void m_saveComponentreflect(T* component, rapidjson::Value& entityData, rapidjson::Document::AllocatorType& allocator);

//LOADING

//template <typename T>//T refers to component
//void m_LoadComponentreflect(T* component, rapidjson::Value& entityData);


/********************************************************************/
#include "../Config/pch.h"
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/filewritestream.h"
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/istreamwrapper.h"
#include "../Dependencies/rapidjson/prettywriter.h"
#include "../Dependencies/rapidjson/filereadstream.h"

#include "../ECS/ECS.h"
#include "../Helper/Helper.h"
#include "../Debugging/Logging.h"
#include "../Asset Manager/Prefab.h"
#include "../Graphics/GraphicsPipe.h"
#include "json_handler.h"

#include <cstdio>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../ECS/Hierachy.h"
#include <unordered_set>

namespace Serialization {

	std::string Serialize::configFilePath;

	std::string layerFilePath = "../Configs/LayerConfig.txt";

	void Serialize::m_LoadConfig(std::string filepath) {


		std::ifstream file;
		configFilePath = filepath + "/Config.txt";
		file.open(configFilePath);

		if (!file.is_open()) {
			LOGGING_ERROR("Error opening config file");
			file.close();
			return;
		}
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		//graphicpipe::GraphicsPipe* graphics = graphicpipe::GraphicsPipe::m_funcGetInstance();

		std::string line;
		
		//char* str[256];

		while (std::getline(file, line)) { // Read line by line
			std::stringstream str2{ line };
			std::string temp;
			str2 >> temp;
			if (temp == "WindowHeight:") str2 >> help->m_windowHeight;
			if (temp == "WindowWidth:") str2 >> help->m_windowWidth;
			if (temp == "FpsCap:") str2 >> help->m_fpsCap;
			if (temp == "StartScene:")std::getline(str2, help->m_startScene);
			// Remove leading whitespace
			help->m_startScene.erase(0, help->m_startScene.find_first_not_of(' '));
		}



		if (help->m_windowHeight <= 0 || help->m_windowWidth <= 0 || !help->m_fpsCap) {
			LOGGING_ERROR("Error Reading Config file (Width or Height <= 0)");
		}
		file.close();
		m_LoadPhysicsLayerMatrix();

	}

	std::vector<std::string> Serialize::m_LoadFilePath(std::string filepath) {


		std::ifstream file;
		std::string filePathText = filepath + "/filepath.txt";
		file.open(filePathText);
		std::vector < std::string> ret;
		if (!file.is_open()) {
			LOGGING_ERROR("Error opening config file");
			file.close();
			return ret;
		}
		std::string line;

		while (std::getline(file, line)) { // Read line by line
			std::stringstream str2{ line };
			std::string temp;
			str2 >> temp;
			ret.push_back(temp);
		}
		file.close();
		return ret;
	}

	bool Serialize::m_UpdateConfigStartScene( const std::string& newStartScene)
	{
		// Open the file for reading
		std::ifstream inFile(configFilePath);
		if (!inFile.is_open()) {
			std::cerr << "Error: Could not open file for reading." << std::endl;
			return false;
		}

		// Read all lines from the file
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(inFile, line)) {
			lines.push_back(line);
		}
		inFile.close();

		// Find and modify the StartScene line
		bool startSceneFound = false;
		for (auto& currentLine : lines) {
			if (currentLine.find("StartScene:") == 0) { // Check if the line starts with "StartScene:"
				currentLine = "StartScene: " + newStartScene;
				startSceneFound = true;
				break; // Exit the loop once the line is found and modified
			}
		}

		if (!startSceneFound) {
			std::cerr << "Error: StartScene line not found in the file." << std::endl;
			return false;
		}

		// Open the file for writing
		std::ofstream outFile(configFilePath);
		if (!outFile.is_open()) {
			std::cerr << "Error: Could not open file for writing." << std::endl;
			return false;
		}

		// Write the modified lines back to the file
		for (const auto& currentLine : lines) {
			outFile << currentLine << std::endl;
		}
		outFile.close();

		std::cout << "StartScene updated successfully!" << std::endl;
		return true;


	}

	void Serialize::m_JsonFileValidation(const std::string& filePath) {

		std::ifstream checkFile(filePath);

		if (!checkFile) {
			std::ofstream createFile(filePath);
			createFile << "[]";  // Initialize empty JSON array
			createFile.close();
		}
		checkFile.close();
	}

	void Serialize::m_LoadComponentsJson(const std::filesystem::path& jsonFilePath)
	{


		// Open the JSON file for reading
		std::ifstream inputFile(jsonFilePath.string());

		if (!inputFile) {
			LOGGING_ERROR("Failed to open JSON file for reading: {}", jsonFilePath.string().c_str());
			return;
		}

		std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
		inputFile.close();

		// Parse the JSON content
		rapidjson::Document doc;
		doc.Parse(fileContent.c_str());

		std::string scenename = jsonFilePath.filename().string();
		

		// Load Global Setting
		//reset global settings
		Helper::Helpers* helper = Helper::Helpers::GetInstance();
		helper->m_colour = {0.f,0.f,0.f};
		helper->m_currMousePicture = "default";
		helper->m_isMouseCentered = false;


		bool foundGlobalSettings = false;
		for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
			const rapidjson::Value& entry = doc[i];
			if (entry.HasMember("GlobalSettings")) {
				foundGlobalSettings = true;
				const rapidjson::Value& globalSettings = entry["GlobalSettings"];
				if (globalSettings.HasMember("globalIllumination")) {
					graphicpipe::GraphicsPipe::m_funcGetInstance()->m_globalLightIntensity = globalSettings["globalIllumination"].GetFloat();
				}
				if (globalSettings.HasMember("backgroundColor")) {
					const rapidjson::Value& bgColor = globalSettings["backgroundColor"];
					
					helper->m_colour.m_x = bgColor["r"].GetFloat();
					helper->m_colour.m_y = bgColor["g"].GetFloat();
					helper->m_colour.m_z = bgColor["b"].GetFloat();
				}
			}
			// Load Cursor Settings
			if (entry.HasMember("CursorSettings")) {
				const rapidjson::Value& cursorSettings = entry["CursorSettings"];

				if (cursorSettings.HasMember("cursorImage")) {
					helper->m_currMousePicture = cursorSettings["cursorImage"].GetString();
				}
				if (cursorSettings.HasMember("isCursorCentered")) {
					helper->m_isMouseCentered = cursorSettings["isCursorCentered"].GetBool();
				}
			}
		}

		/*******************INSERT INTO FUNCTION*****************************/

		// Iterate through each component entry in the JSON array
		for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
			const rapidjson::Value& entityData = doc[i];

			if (entityData.HasMember("GlobalSettings") || entityData.HasMember("CursorSettings")) {
				continue;
			}


			m_LoadEntity(entityData, std::nullopt, scenename);
		}


		LOGGING_INFO("Load Json Successful");
	}

	void Serialize::m_SaveComponentsJson(const std::filesystem::path& scene)
	{
		auto* ecs = ecs::ECS::m_GetInstance();
		std::string jsonFilePath = scene.string();
		m_JsonFileValidation(jsonFilePath);

		// Create JSON object to hold the updated values
		rapidjson::Document doc;
		doc.SetArray();  // Initialize as an empty array

		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		std::unordered_set<ecs::EntityID> savedEntities;  //track saved entities

		rapidjson::Value globalSettings(rapidjson::kObjectType);
		globalSettings.AddMember("globalIllumination",
			graphicpipe::GraphicsPipe::m_funcGetInstance()->m_globalLightIntensity, allocator);

		rapidjson::Value backgroundColor(rapidjson::kObjectType);
		Helper::Helpers* helper = Helper::Helpers::GetInstance();
		backgroundColor.AddMember("r", helper->m_colour.m_x, allocator);
		backgroundColor.AddMember("g", helper->m_colour.m_y, allocator);
		backgroundColor.AddMember("b", helper->m_colour.m_z, allocator);
		globalSettings.AddMember("backgroundColor", backgroundColor, allocator);

		rapidjson::Value settingsWrapper(rapidjson::kObjectType);
		settingsWrapper.AddMember("GlobalSettings", globalSettings, allocator);
		doc.PushBack(settingsWrapper, allocator);

		// Save Cursor Settings
		rapidjson::Value cursorSettings(rapidjson::kObjectType);
		cursorSettings.AddMember("cursorImage", rapidjson::Value(helper->m_currMousePicture.c_str(), allocator), allocator);
		cursorSettings.AddMember("isCursorCentered", helper->m_isMouseCentered, allocator);

		rapidjson::Value cursorWrapper(rapidjson::kObjectType);
		cursorWrapper.AddMember("CursorSettings", cursorSettings, allocator);
		doc.PushBack(cursorWrapper, allocator);

		//Start saving the entities
		std::vector<ecs::EntityID> entities = ecs->m_ECS_SceneMap.find(scene.filename().string())->second.m_sceneIDs;
		for (const auto& entityId : entities) {
			if (!ecs::Hierachy::m_GetParent(entityId).has_value()) {
				rapidjson::Value entityData(rapidjson::kObjectType);
				m_SaveEntity(entityId, doc, allocator, savedEntities);
			}
		}

		// Write the JSON back to file
		rapidjson::StringBuffer writeBuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(writeBuffer);
		doc.Accept(writer);

		std::ofstream outputFile(jsonFilePath);
		if (outputFile) {
			outputFile << writeBuffer.GetString();
			outputFile.close();
		}

		LOGGING_INFO("Save Json Successful");
	}

	void Serialize::m_SaveEntity(ecs::EntityID entityId, rapidjson::Value& parentArray, rapidjson::Document::AllocatorType& allocator, std::unordered_set<ecs::EntityID>& savedEntities) {
		auto* ecs = ecs::ECS::m_GetInstance();
		auto& signature = ecs->m_ECS_EntityMap.find(entityId)->second;

		if (savedEntities.find(entityId) != savedEntities.end()) {
			return;
		}

		rapidjson::Value entityData(rapidjson::kObjectType);
		bool hasComponents = false;

		// Find and save name for this entity
		if (signature.test(ecs::ComponentType::TYPENAMECOMPONENT)) {
			ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entityId));

			if (nc) {
				rapidjson::Value name(rapidjson::kObjectType);
				rapidjson::Value nameValue, prefabValue, tagValue;
				nameValue.SetString(nc->m_entityName.c_str(), allocator);
				name.AddMember("namestr", nameValue, allocator);
				name.AddMember("layer", (int)nc->m_Layer, allocator);
				name.AddMember("isprefab", nc->m_isPrefab, allocator);
				tagValue.SetString(nc->m_entityTag.c_str(), allocator);
				name.AddMember("tagstr", tagValue, allocator);
				if (nc->m_isPrefab) {
					prefabValue.SetString(nc->m_prefabName.c_str(), allocator);
					name.AddMember("prefabname", prefabValue, allocator);
					name.AddMember("issync", nc->m_syncPrefab, allocator);
				}
				entityData.AddMember("name", name, allocator);
				hasComponents = true;
				

			}


		}

		// Check if the entity has TransformComponent and save it
		if (signature.test(ecs::ComponentType::TYPETRANSFORMCOMPONENT)) {
			ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityId));
			if (tc) {
				rapidjson::Value transform(rapidjson::kObjectType);
				transform.AddMember("position", rapidjson::Value().SetObject()
					.AddMember("x", tc->m_position.m_x, allocator)
					.AddMember("y", tc->m_position.m_y, allocator), allocator);
				transform.AddMember("rotation", tc->m_rotation, allocator);
				transform.AddMember("scale", rapidjson::Value().SetObject()
					.AddMember("x", tc->m_scale.m_x, allocator)
					.AddMember("y", tc->m_scale.m_y, allocator), allocator);
				entityData.AddMember("transform", transform, allocator);
				hasComponents = true;

			}
		}

		// Check if the entity has ColliderComponent and save it
		if (signature.test(ecs::ComponentType::TYPECOLLIDERCOMPONENT)) {
			ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entityId));
			if (cc) {
				rapidjson::Value collider(rapidjson::kObjectType);
				collider.AddMember("checkcollision", cc->m_collisionCheck, allocator);
				collider.AddMember("checkresponse", cc->m_collisionResponse, allocator);
				collider.AddMember("size", rapidjson::Value().SetObject()
					.AddMember("x", cc->m_Size.m_x, allocator)
					.AddMember("y", cc->m_Size.m_y, allocator), allocator);
				collider.AddMember("offset", rapidjson::Value().SetObject()
					.AddMember("x", cc->m_OffSet.m_x, allocator)
					.AddMember("y", cc->m_OffSet.m_y, allocator), allocator);
				collider.AddMember("drawDebug", cc->m_drawDebug, allocator);
				
				collider.AddMember("radius", cc->m_radius, allocator);
				collider.AddMember("shapetype", (int)cc->m_type, allocator);
				entityData.AddMember("collider", collider, allocator);
				hasComponents = true;  // Mark as having a component


			}
		}

		// Check if the entity has PlayerComponent and save it
		if (signature.test(ecs::ComponentType::TYPEENEMYCOMPONENT)) {
			ecs::EnemyComponent* ec = static_cast<ecs::EnemyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEENEMYCOMPONENT]->m_GetEntityComponent(entityId));
			if (ec) {
				rapidjson::Value player(rapidjson::kObjectType);
				player.AddMember("enemytag", ec->m_enemyTag, allocator);
				player.AddMember("enemytype", ec->m_enemyTypeInt, allocator);
				player.AddMember("enemybehaviour", ec->m_enemyRoamBehaviourInt, allocator);
				entityData.AddMember("enemy", player, allocator);
				hasComponents = true;  // Mark as having a component
			}
		}

		// Check if the entity has RigidBodyComponent and save it
		if (signature.test(ecs::ComponentType::TYPERIGIDBODYCOMPONENT)) {
			ecs::RigidBodyComponent* rb = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entityId));
			if (rb) {
				rapidjson::Value rigidbody(rapidjson::kObjectType);

				rigidbody.AddMember("linearVelocity", rapidjson::Value().SetObject()
					.AddMember("x", rb->m_Velocity.m_x, allocator)
					.AddMember("y", rb->m_Velocity.m_y, allocator), allocator);
				rigidbody.AddMember("linearAcceleration", rapidjson::Value().SetObject()
					.AddMember("x", rb->m_Acceleration.m_x, allocator)
					.AddMember("y", rb->m_Acceleration.m_y, allocator), allocator);

				rigidbody.AddMember("rotation", rb->m_Rotation, allocator);

				rigidbody.AddMember("angularVelocity", rb->m_AngularVelocity, allocator);
				rigidbody.AddMember("angularAcceleration", rb->m_AngularAcceleration, allocator);

				rigidbody.AddMember("mass", rb->m_Mass, allocator);
				rigidbody.AddMember("inverseMass", rb->m_InverseMass, allocator);

				rigidbody.AddMember("linearDamping", rb->m_LinearDamping, allocator);
				rigidbody.AddMember("angularDamping", rb->m_AngularDamping, allocator);

				rigidbody.AddMember("force", rapidjson::Value().SetObject()
					.AddMember("x", rb->m_Force.m_x, allocator)
					.AddMember("y", rb->m_Force.m_y, allocator), allocator);

				rigidbody.AddMember("torque", rb->m_Torque, allocator);
				rigidbody.AddMember("kinematic", rb->m_IsKinematic, allocator);
				rigidbody.AddMember("static", rb->m_IsStatic, allocator);

				entityData.AddMember("rigidbody", rigidbody, allocator);
				hasComponents = true;  // Mark as having a component
			}
		}

		// Check if the entity has SpriteComponent and save it
		if (signature.test(ecs::ComponentType::TYPESPRITECOMPONENT)) {
			ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityId));
			if (sc) {
				rapidjson::Value sprite(rapidjson::kObjectType);
				rapidjson::Value textValue;
				textValue.SetString(sc->m_imageFile.c_str(), allocator);

				sprite.AddMember("imagefile", textValue, allocator);
				sprite.AddMember("layer", sc->m_layer, allocator);

				rapidjson::Value colorValue(rapidjson::kObjectType);
				colorValue.AddMember("r", sc->m_color.m_x, allocator);
				colorValue.AddMember("g", sc->m_color.m_y, allocator);
				colorValue.AddMember("b", sc->m_color.m_z, allocator);
				sprite.AddMember("color", colorValue, allocator);
				sprite.AddMember("alpha", sc->m_alpha, allocator);
				sprite.AddMember("isLit", sc->m_isIlluminated, allocator);

				entityData.AddMember("sprite", sprite, allocator);
				hasComponents = true;  // Mark as having a component
			}
		}

		// Check if the entity has TextComponent and save it
		if (signature.test(ecs::ComponentType::TYPETEXTCOMPONENT)) {
			ecs::TextComponent* tc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityId));
			if (tc) {
				rapidjson::Value text(rapidjson::kObjectType);
				rapidjson::Value textValue;
				textValue.SetString(tc->m_text.c_str(), allocator);
				text.AddMember("text", textValue, allocator);

				text.AddMember("text", textValue, allocator);

				if (!tc->m_fileName.empty()) {
					rapidjson::Value fileNameValue;
					fileNameValue.SetString(tc->m_fileName.c_str(), allocator);
					text.AddMember("filename", fileNameValue, allocator);
				}

				text.AddMember("fontsize", tc->m_fontSize, allocator);
				text.AddMember("color", rapidjson::Value().SetObject()
					.AddMember("r", tc->m_color.m_x, allocator)
					.AddMember("g", tc->m_color.m_y, allocator)
					.AddMember("b", tc->m_color.m_z, allocator), allocator);

				text.AddMember("layer", tc->m_fontLayer, allocator);
				text.AddMember("isCentered", tc->m_isCentered,allocator);

				entityData.AddMember("text", text, allocator);
				hasComponents = true;  // Mark as having a component
			}
		}

		// Check if the entity has AnimationComponent and save it
		if (signature.test(ecs::ComponentType::TYPEANIMATIONCOMPONENT)) {
			ecs::AnimationComponent* ac = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityId));
			if (ac) {
				rapidjson::Value animation(rapidjson::kObjectType);
				animation.AddMember("frameNumber", ac->m_frameNumber, allocator);
				animation.AddMember("framesPerSecond", ac->m_framesPerSecond, allocator);
				animation.AddMember("frameTimer", ac->m_frameTimer, allocator);
				animation.AddMember("isAnimating", ac->m_isAnimating, allocator);
				animation.AddMember("stripCount", ac->m_stripCount, allocator);
				entityData.AddMember("animation", animation, allocator);
				
				hasComponents = true;  // Mark as having a component
			}
		}

		// Check if the entity has CameraComponent and save it
		if (signature.test(ecs::ComponentType::TYPECAMERACOMPONENT))
		{
			ecs::CameraComponent* cc = static_cast<ecs::CameraComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPECAMERACOMPONENT]->m_GetEntityComponent(entityId));
			if (cc)
			{
				rapidjson::Value camera(rapidjson::kObjectType);

				camera.AddMember("planes", rapidjson::Value().SetObject()
					.AddMember("left", cc->m_left, allocator)
					.AddMember("right", cc->m_right, allocator)
					.AddMember("top", cc->m_top, allocator)
					.AddMember("bottom", cc->m_bottom, allocator), allocator);

				camera.AddMember("aspectRatio", cc->m_aspectRatio, allocator);

				entityData.AddMember("camera", camera, allocator);
				hasComponents = true;
			}
		}

		// Check if the entity has ButtonComponent and save it
		if (signature.test(ecs::ComponentType::TYPEBUTTONCOMPONENT))
		{
			ecs::ButtonComponent* bc = static_cast<ecs::ButtonComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEBUTTONCOMPONENT]->m_GetEntityComponent(entityId));
			if (bc)
			{
				rapidjson::Value button(rapidjson::kObjectType);

				button.AddMember("force", rapidjson::Value().SetObject()
					.AddMember("x", bc->m_Position.m_x, allocator)
					.AddMember("y", bc->m_Position.m_y, allocator), allocator);

				button.AddMember("scale", rapidjson::Value().SetObject()
					.AddMember("x", bc->m_Scale.m_x, allocator)
					.AddMember("y", bc->m_Scale.m_y, allocator), allocator);

				button.AddMember("enablehover", bc->m_EnableHover, allocator);

				entityData.AddMember("button", button, allocator);
				hasComponents = true;
			}
		}

		// Check if the entity has ScriptComponent and save it
		if (signature.test(ecs::ComponentType::TYPESCRIPTCOMPONENT))
		{
			ecs::ScriptComponent* sc = static_cast<ecs::ScriptComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(entityId));
			if (sc)
			{
				rapidjson::Value script(rapidjson::kObjectType);

				// Create an array to store script objects
				rapidjson::Value scriptArray(rapidjson::kArrayType);

				for (const auto& scriptName : sc->m_scripts) {
					// Create an object for each script
					rapidjson::Value scriptObject(rapidjson::kObjectType);

					// Add the script name (string) to the object
					rapidjson::Value scriptValue;
					scriptValue.SetString(std::get<0>(scriptName).c_str(), allocator);
					scriptObject.AddMember("name", scriptValue, allocator);

					// Add the boolean value to the object
					scriptObject.AddMember("enabled", std::get<1>(scriptName), allocator);

					rapidjson::Value variableMapValue(rapidjson::kArrayType);
					for (const auto& x : std::get<2>(scriptName)) {
						rapidjson::Value VariableString, base64string;

						VariableString.SetString(x.first.c_str(), allocator);
						base64string.SetString(x.second.c_str(), allocator);

						rapidjson::Value publicVariable(rapidjson::kObjectType); // Create "publicvariable" object
						publicVariable.AddMember("variable", VariableString, allocator);
						publicVariable.AddMember("base64", base64string, allocator); // Fixed the name


						variableMapValue.PushBack(publicVariable, allocator); // Push to array
					}

					scriptObject.AddMember("map", variableMapValue, allocator);

					// Add the object to the array
					scriptArray.PushBack(scriptObject, allocator);
				}

				// Add the script array to the "script" object
				script.AddMember("scripts", scriptArray, allocator);
				entityData.AddMember("script", script, allocator);
			}
		}

		// Check if the entity has ButtonComponent and save it
		if (signature.test(ecs::ComponentType::TYPETILEMAPCOMPONENT))
		{
			ecs::TilemapComponent* tilec = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(entityId));
			if (tilec)
			{
				rapidjson::Value tile(rapidjson::kObjectType);
				rapidjson::Value textValue;
				textValue.SetString(tilec->m_tilemapFile.c_str(), allocator);

				tile.AddMember("imagefile", textValue, allocator);
				tile.AddMember("layer", tilec->m_tileLayer, allocator);

				rapidjson::Value colorValue(rapidjson::kObjectType);
				colorValue.AddMember("r", tilec->m_color.m_x, allocator);
				colorValue.AddMember("g", tilec->m_color.m_y, allocator);
				colorValue.AddMember("b", tilec->m_color.m_z, allocator);

				tile.AddMember("color", colorValue, allocator);
				tile.AddMember("alpha", tilec->m_alpha, allocator);

				tile.AddMember("rowlength", tilec->m_rowLength, allocator);
				tile.AddMember("columnlength", tilec->m_columnLength, allocator);
				tile.AddMember("pictureRowLength", tilec->m_pictureRowLength, allocator);
				tile.AddMember("pictureColumnLength", tilec->m_pictureColumnLength, allocator);

				 //Create the main array for `tilePictureIndex`
				rapidjson::Value tileArray(rapidjson::kArrayType);

				 //Loop over each row in the 2D vector
				for (const std::vector<int>& row : tilec->m_tilePictureIndex) {
					// Create an array for the current row
					rapidjson::Value tileRow(rapidjson::kArrayType);

					// Add each integer in the row to the `tileRow` array
					for (int tile2 : row) {
						tileRow.PushBack(tile2, allocator);  // Add integer to the row array
					}

					// Add the `tileRow` array to the main `tileArray`
					tileArray.PushBack(tileRow, allocator);
				}
				tile.AddMember("tilePictureIndex", tileArray, allocator);
				entityData.AddMember("tilemap", tile, allocator);
				hasComponents = true;  // Mark as having a component
			}
		}

		// Check if the entity has GridComponent and save it
		if (signature.test(ecs::ComponentType::TYPEGRIDCOMPONENT))
		{
			ecs::GridComponent* gridc = static_cast<ecs::GridComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEGRIDCOMPONENT]->m_GetEntityComponent(entityId));
			if (gridc)
			{
				rapidjson::Value grid(rapidjson::kObjectType);

				grid.AddMember("gridRowLength", gridc->m_GridRowLength, allocator);
				grid.AddMember("gridColumnLength", gridc->m_GridColumnLength, allocator);
				grid.AddMember("setCollidable", gridc->m_SetCollidable, allocator);

				rapidjson::Value wallArray(rapidjson::kArrayType);

				for (const std::vector<int>& row : gridc->m_IsWall)
				{
					rapidjson::Value wallRow(rapidjson::kArrayType);
					for (int wall : row)
					{
						wallRow.PushBack(wall, allocator);  // Add integer to the row array
					}
					wallArray.PushBack(wallRow, allocator);
				}
				
				grid.AddMember("gridKey", gridc->m_GridKey, allocator);

				grid.AddMember("isWall", wallArray, allocator);
				entityData.AddMember("grid", grid, allocator);

				

				hasComponents = true;  // Mark as having a component
			}
		}

		if (signature.test(ecs::ComponentType::TYPEPARTICLECOMPONENT))
		{
			ecs::ParticleComponent* particle = static_cast<ecs::ParticleComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entityId));
			if (particle)
			{
				rapidjson::Value par(rapidjson::kObjectType);

				par.AddMember("willSpawn", particle->m_willSpawn, allocator);
				par.AddMember("noOfParticles", particle->m_noOfParticles, allocator);
				par.AddMember("lifeSpan", particle->m_lifeSpan, allocator);
				par.AddMember("velocityX", particle->m_velocity.m_x, allocator);
				par.AddMember("velocityY", particle->m_velocity.m_y, allocator);
				par.AddMember("sizeX", particle->m_particleSize.m_x, allocator);
				par.AddMember("sizeY", particle->m_particleSize.m_y, allocator);
				par.AddMember("accelerationX", particle->m_acceleration.m_x, allocator);
				par.AddMember("accelerationY", particle->m_acceleration.m_y, allocator);
				par.AddMember("colorR", particle->m_color.m_x, allocator);
				par.AddMember("colorG", particle->m_color.m_y, allocator);
				par.AddMember("colorB", particle->m_color.m_z, allocator);
				par.AddMember("coneRotation", particle->m_coneRotation, allocator);
				par.AddMember("coneAngle", particle->m_coneAngle, allocator);
				par.AddMember("randomFactor", particle->m_randomFactor, allocator);
				par.AddMember("imageFile", rapidjson::Value(particle->m_imageFile.c_str(), allocator), allocator);
				par.AddMember("stripCount", particle->m_stripCount, allocator);
				par.AddMember("frameNumber", particle->m_frameNumber, allocator);
				par.AddMember("layer", particle->m_layer, allocator);
				par.AddMember("friction", particle->m_friction, allocator);
				par.AddMember("fps", particle->m_fps, allocator);
				par.AddMember("loopAnimation", particle->m_loopAnimation, allocator);


				entityData.AddMember("particle", par, allocator);
				hasComponents = true;
			}
		}

		// Check if the entity has AudioComponent and save it
		if (signature.test(ecs::ComponentType::TYPEAUDIOCOMPONENT)) {
			ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(
				ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(entityId)
				);

			if (ac) {
				rapidjson::Value audioArray(rapidjson::kArrayType);

				for (const auto& audioFile : ac->m_AudioFiles) {
					rapidjson::Value audioObject(rapidjson::kObjectType);

					rapidjson::Value filePathValue;
					filePathValue.SetString(audioFile.m_FilePath.c_str(), allocator);

					audioObject.AddMember("name", rapidjson::Value().SetString(audioFile.m_Name.c_str(), allocator), allocator);
					audioObject.AddMember("volume", audioFile.m_Volume, allocator);
					audioObject.AddMember("loop", audioFile.m_Loop, allocator);
					audioObject.AddMember("playOnStart", audioFile.m_PlayOnStart, allocator);
					audioObject.AddMember("isBGM", audioFile.m_IsBGM, allocator);
					audioObject.AddMember("isSFX", audioFile.m_IsSFX, allocator);


					audioArray.PushBack(audioObject, allocator);
				}

				entityData.AddMember("audio", audioArray, allocator);
				hasComponents = true;
			}
		}

		if (signature.test(ecs::ComponentType::TYPERAYCASTINGCOMPONENT))
		{
			ecs::RaycastComponent* rc = static_cast<ecs::RaycastComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPERAYCASTINGCOMPONENT]->m_GetEntityComponent(entityId));
			if (rc)
			{
				rapidjson::Value raycast(rapidjson::kObjectType);
				// Create an array to store script objects
				rapidjson::Value rayArray(rapidjson::kArrayType);

				for (const auto& raycast2 : rc->m_raycast) {
					// Create an object for each script
					rapidjson::Value raycastObject(rapidjson::kObjectType);
					
					rapidjson::Value raycastID;
					raycastID.SetString(raycast2.m_rayID.c_str(), allocator);
					raycastObject.AddMember("raycastID", raycastID, allocator);


					// Add the boolean value to the object
					raycastObject.AddMember("israycasting", raycast2.m_isRaycasting, allocator);

					raycastObject.AddMember("targetposition", rapidjson::Value().SetObject()
						.AddMember("x", raycast2.m_targetPosition.m_x, allocator)
						.AddMember("y", raycast2.m_targetPosition.m_y, allocator), allocator);

					if (raycast2.m_Layers.size() > 0) {
						rapidjson::Value layerArray(rapidjson::kArrayType);

						for (const auto layer : raycast2.m_Layers) {
							rapidjson::Value layerObject(rapidjson::kObjectType);

							layerObject.AddMember("layer", (int)layer, allocator);

							layerArray.PushBack(layerObject, allocator);
						}

						raycastObject.AddMember("Layer Vector", layerArray, allocator);
					}
					// Add the object to the array
					rayArray.PushBack(raycastObject, allocator);
				}

				// Add the script array to the "script" object
				raycast.AddMember("raycasts", rayArray, allocator);
				entityData.AddMember("raycast", raycast, allocator);
				hasComponents = true;
			}
		}

		// Check if the entity has PathFinding Component and save it
		if (signature.test(ecs::ComponentType::TYPEPATHFINDINGCOMPONENT))
		{
			ecs::PathfindingComponent* pc = static_cast<ecs::PathfindingComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEPATHFINDINGCOMPONENT]->m_GetEntityComponent(entityId));
			if (pc)
			{
				rapidjson::Value pathfinding(rapidjson::kObjectType);

				// Add start position
				pathfinding.AddMember("startPosition", rapidjson::Value().SetObject()
					.AddMember("x", pc->m_StartPos.m_x, allocator)
					.AddMember("y", pc->m_StartPos.m_y, allocator), allocator);

				// Add target position
				pathfinding.AddMember("targetPosition", rapidjson::Value().SetObject()
					.AddMember("x", pc->m_TargetPos.m_x, allocator)
					.AddMember("y", pc->m_TargetPos.m_y, allocator), allocator);

				
				pathfinding.AddMember("gridKey", pc->m_GridKey, allocator);

				// Add the pathfinding component data to the entity data
				entityData.AddMember("pathfinding", pathfinding, allocator);
				hasComponents = true;
			}
		}

		if (signature.test(ecs::ComponentType::TYPELIGHTINGCOMPONENT)) {
			ecs::LightingComponent *lc = static_cast<ecs::LightingComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPELIGHTINGCOMPONENT]->m_GetEntityComponent(entityId));
			if (lc) {
				m_saveComponentreflect(lc, entityData, allocator);
			}
		}

		if (signature.test(ecs::ComponentType::TYPEVIDEOCOMPONENT)) {
			ecs::VideoComponent* vc = static_cast<ecs::VideoComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEVIDEOCOMPONENT]->m_GetEntityComponent(entityId));
			if (vc) {
				m_saveComponentreflect(vc, entityData, allocator);
			}
		}

		// Add children
		std::optional<std::vector<ecs::EntityID>> childrenOptional = ecs::Hierachy::m_GetChild(entityId);
		if (childrenOptional.has_value()) {
			std::vector<ecs::EntityID> children = childrenOptional.value();
			if (!children.empty()) {
				rapidjson::Value childrenArray(rapidjson::kArrayType);
				for (const auto& childID : children) {
					m_SaveEntity(childID, childrenArray, allocator, savedEntities);
				}
				entityData.AddMember("children", childrenArray, allocator);
			}
		}

		if (hasComponents) {
			parentArray.PushBack(entityData, allocator);
			savedEntities.insert(entityId);
		}
	}

	void Serialize::m_LoadEntity(const rapidjson::Value& entityData, std::optional<ecs::EntityID> parentID, const std::string& sceneName)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::EntityID newEntityId = ecs->m_CreateEntity(sceneName);

		if (entityData.HasMember("name") && entityData["name"].IsObject()) {
			ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(newEntityId));
			const rapidjson::Value& name = entityData["name"];



			if (name.HasMember("namestr") && name["namestr"].IsString()) {
				nc->m_entityName = name["namestr"].GetString();  // Store the name
			}
			if (name.HasMember("layer") && name["layer"].IsInt()) {
				 int layer = name["layer"].GetInt();
				 nc->m_Layer = (layer::LAYERS)layer;
			}
			if (name.HasMember("isprefab") && name["isprefab"].IsBool()) {
				nc->m_isPrefab = name["isprefab"].GetBool();
			}
			if (name.HasMember("tagstr") && name["tagstr"].IsString()) {
				nc->m_entityTag = name["tagstr"].GetString();
			}
			if (nc->m_isPrefab) {
				if (name.HasMember("prefabname") && name["prefabname"].IsString()) {
					nc->m_prefabName = name["prefabname"].GetString();
				}
				if (name.HasMember("issync") && name["issync"].IsBool()) {
					nc->m_syncPrefab = name["issync"].GetBool();
				}
			}

		}

		// Load TransformComponent if it exists
		if (entityData.HasMember("transform") && entityData["transform"].IsObject()) {
			ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(newEntityId));

			const rapidjson::Value& transform = entityData["transform"];
			if (transform.HasMember("position") && transform["position"].IsObject()) {
				tc->m_position.m_x = transform["position"]["x"].GetFloat();
				tc->m_position.m_y = transform["position"]["y"].GetFloat();
			}
			if (transform.HasMember("rotation")) {
				tc->m_rotation = transform["rotation"].GetFloat();
			}
			if (transform.HasMember("scale") && transform["scale"].IsObject()) {
				tc->m_scale.m_x = transform["scale"]["x"].GetFloat();
				tc->m_scale.m_y = transform["scale"]["y"].GetFloat();
			}
	
		}

		// Load Collider Component if it exists
		if (entityData.HasMember("collider") && entityData["collider"].IsObject()) {

			ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, newEntityId));

			if (cc) {
				const rapidjson::Value& collider = entityData["collider"];
				if (collider.HasMember("size") && collider["size"].IsObject()) {
					cc->m_Size.m_x = collider["size"]["x"].GetFloat();
					cc->m_Size.m_y = collider["size"]["y"].GetFloat();
				}
				if (collider.HasMember("offset") && collider["offset"].IsObject()) {
					cc->m_OffSet.m_x = collider["offset"]["x"].GetFloat();
					cc->m_OffSet.m_y = collider["offset"]["y"].GetFloat();
				}
				if (collider.HasMember("drawDebug")) {
					cc->m_drawDebug = collider["drawDebug"].GetBool();
				}
				if (collider.HasMember("radius")) {
					cc->m_radius = collider["radius"].GetFloat();
				}
				if (collider.HasMember("shapetype")) {
					cc->m_type = (physicspipe::EntityType)collider["shapetype"].GetInt();
				}
				if (collider.HasMember("checkcollision")) {
					cc->m_collisionCheck = collider["checkcollision"].GetBool();
				}
				if (collider.HasMember("checkresponse")) {
					cc->m_collisionResponse = collider["checkresponse"].GetBool();
				}
			}
		}

		// Load Player Component if it exists
		if (entityData.HasMember("enemy") && entityData["enemy"].IsObject()) {
			ecs::EnemyComponent* pc = static_cast<ecs::EnemyComponent*>(ecs->m_AddComponent(ecs::TYPEENEMYCOMPONENT, newEntityId));

			if (pc) {
				const rapidjson::Value& player = entityData["enemy"];
				if (player.HasMember("enemytag") && player["enemytag"].IsInt()) {
					pc->m_enemyTag = player["enemytag"].GetInt();
				}
				if (player.HasMember("enemytype") && player["enemytype"].IsInt()) {
					pc->m_enemyTypeInt = player["enemytype"].GetInt();
				}
				if (player.HasMember("enemybehaviour") && player["enemybehaviour"].IsInt()) {
					pc->m_enemyRoamBehaviourInt = player["enemybehaviour"].GetInt();
				}

				//const rapidjson::Value& name = entityData["EnemyComponent"];
				//LoadComponent<decltype(ecs::EnemyComponent::Names())> loader{ ecs::EnemyComponent::Names() };

				//pc->ApplyFunction([&](auto& member) {
				//	loader(member, name);
				//	});
			}
		}

		// Load RigidBody Component if it exists
		if (entityData.HasMember("rigidbody") && entityData["rigidbody"].IsObject()) {
			ecs::RigidBodyComponent* rb = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, newEntityId));

			if (rb) {
				const rapidjson::Value& rigidbody = entityData["rigidbody"];

				if (rigidbody.HasMember("linearVelocity") && rigidbody["linearVelocity"].IsObject()) {
					rb->m_Velocity.m_x = rigidbody["linearVelocity"]["x"].GetFloat();
					rb->m_Velocity.m_y = rigidbody["linearVelocity"]["y"].GetFloat();
				}
				if (rigidbody.HasMember("linearAcceleration") && rigidbody["linearAcceleration"].IsObject()) {
					rb->m_Acceleration.m_x = rigidbody["linearAcceleration"]["x"].GetFloat();
					rb->m_Acceleration.m_y = rigidbody["linearAcceleration"]["y"].GetFloat();
				}

				if (rigidbody.HasMember("rotation")) {
					rb->m_Rotation = rigidbody["rotation"].GetFloat();
				}
				if (rigidbody.HasMember("angularVelocity")) {
					rb->m_AngularVelocity = rigidbody["angularVelocity"].GetFloat();
				}
				if (rigidbody.HasMember("angularAcceleration")) {
					rb->m_AngularAcceleration = rigidbody["angularAcceleration"].GetFloat();
				}

				if (rigidbody.HasMember("mass")) {
					rb->m_Mass = rigidbody["mass"].GetFloat();
				}
				if (rigidbody.HasMember("inverseMass")) {
					rb->m_InverseMass = rigidbody["inverseMass"].GetFloat();
				}

				if (rigidbody.HasMember("linearDamping")) {
					rb->m_LinearDamping = rigidbody["linearDamping"].GetFloat();
				}
				if (rigidbody.HasMember("angularDamping")) {
					rb->m_AngularDamping = rigidbody["angularDamping"].GetFloat();
				}

				if (rigidbody.HasMember("force") && rigidbody["force"].IsObject()) {
					rb->m_Force.m_x = rigidbody["force"]["x"].GetFloat();
					rb->m_Force.m_y = rigidbody["force"]["y"].GetFloat();
				}

				if (rigidbody.HasMember("torque")) {
					rb->m_Torque = rigidbody["torque"].GetFloat();
				}
				if (rigidbody.HasMember("kinematic")) {
					rb->m_IsKinematic = rigidbody["kinematic"].GetBool();
				}
				if (rigidbody.HasMember("static")) {
					rb->m_IsStatic = rigidbody["static"].GetBool();
				}

			}
		}

		// Load Sprite Component if it exists
		if (entityData.HasMember("sprite") && entityData["sprite"].IsObject()) {
			ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, newEntityId));

			if (sc) {
				const rapidjson::Value& sprite = entityData["sprite"];
				if (sprite.HasMember("imagefile")) {
					sc->m_imageFile = sprite["imagefile"].GetString();
				}
				if (sprite.HasMember("layer")) {
					sc->m_layer = sprite["layer"].GetInt();
				}

				if (sprite.HasMember("color") && sprite["color"].IsObject()) {
					const rapidjson::Value& color = sprite["color"];
					if (color.HasMember("r")) sc->m_color.m_x = color["r"].GetFloat();
					if (color.HasMember("g")) sc->m_color.m_y = color["g"].GetFloat();
					if (color.HasMember("b")) sc->m_color.m_z = color["b"].GetFloat();
				}
				if (sprite.HasMember("alpha")) {
					sc->m_alpha = sprite["alpha"].GetFloat();
				}
				if (sprite.HasMember("isLit"))
				{
					sc->m_isIlluminated = sprite["isLit"].GetBool();
				}
			}
		}

		// Load text Component if it exists
		if (entityData.HasMember("text") && entityData["text"].IsObject()) {
			ecs::TextComponent* tc = static_cast<ecs::TextComponent*>(ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, newEntityId));

			if (tc) {
				const rapidjson::Value& text = entityData["text"];
				if (text.HasMember("text")) {
					tc->m_text = text["text"].GetString();
				}
				if (text.HasMember("filename")) {
					tc->m_fileName = text["filename"].GetString();
				}
				if (text.HasMember("fontsize")) {
					tc->m_fontSize = text["fontsize"].GetFloat();
				}
				if (text.HasMember("layer")) {
					tc->m_fontLayer = text["layer"].GetInt();
				}
				if (text.HasMember("color")) {
					const rapidjson::Value& color = text["color"];
					if (color.HasMember("r")) tc->m_color.m_x = color["r"].GetFloat();
					if (color.HasMember("g")) tc->m_color.m_y = color["g"].GetFloat();
					if (color.HasMember("b")) tc->m_color.m_z = color["b"].GetFloat();
				}
				if (text.HasMember("isCentered")) {
					tc->m_isCentered = text["isCentered"].GetBool();
				}

			}
		}

		// Load button Component if it exists
		if (entityData.HasMember("button") && entityData["button"].IsObject()) {
			ecs::ButtonComponent* bc = static_cast<ecs::ButtonComponent*>(ecs->m_AddComponent(ecs::TYPEBUTTONCOMPONENT, newEntityId));

			if (bc) {
				const rapidjson::Value& button = entityData["button"];

				if (button.HasMember("force") && button["force"].IsObject()) {
					bc->m_Position.m_x = button["force"]["x"].GetFloat();
					bc->m_Position.m_y = button["force"]["y"].GetFloat();
				}

				if (button.HasMember("scale") && button["scale"].IsObject()) {
					bc->m_Scale.m_x = button["scale"]["x"].GetFloat();
					bc->m_Scale.m_y = button["scale"]["y"].GetFloat();
				}

				if (button.HasMember("enablehover"))
				{
					bc->m_EnableHover = button["enablehover"].GetBool();
				}
			}
		}

		// Load Animation Component if it exists
		if (entityData.HasMember("animation") && entityData["animation"].IsObject()) {
			ecs::AnimationComponent* ac = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, newEntityId));

			if (ac) {
				const rapidjson::Value& animation = entityData["animation"];
				if (animation.HasMember("frameNumber"))
				{
					ac->m_frameNumber = animation["frameNumber"].GetInt();
				}
				if (animation.HasMember("framesPerSecond"))
				{
					ac->m_framesPerSecond = animation["framesPerSecond"].GetInt();
				}
				if (animation.HasMember("frameTimer"))
				{
					ac->m_frameTimer = animation["frameTimer"].GetFloat();
				}
				if (animation.HasMember("isAnimating"))
				{
					ac->m_isAnimating = animation["isAnimating"].GetBool();
				}
				if (animation.HasMember("stripCount"))
				{
					ac->m_stripCount = animation["stripCount"].GetInt();
				}
			}
		}

		// Load camera Component if it exists
		if (entityData.HasMember("camera") && entityData["camera"].IsObject()) {
			ecs::CameraComponent* cc = static_cast<ecs::CameraComponent*>(ecs->m_AddComponent(ecs::TYPECAMERACOMPONENT, newEntityId));

			if (cc) {
				const rapidjson::Value& camera = entityData["camera"];

				if (camera.HasMember("planes") && camera["planes"].IsObject()) {
					const rapidjson::Value& planes = camera["planes"];
					if (planes.HasMember("left")) cc->m_left = planes["left"].GetFloat();
					if (planes.HasMember("right")) cc->m_right = planes["right"].GetFloat();
					if (planes.HasMember("top")) cc->m_top = planes["top"].GetFloat();
					if (planes.HasMember("bottom")) cc->m_bottom = planes["bottom"].GetFloat();
				}

				if (camera.HasMember("aspectRatio")) {
					cc->m_aspectRatio = camera["aspectRatio"].GetFloat();
				}
			}
		}

		// Load Script Component if it exists
		if (entityData.HasMember("script") && entityData["script"].IsObject()) {
			ecs::ScriptComponent* sc = static_cast<ecs::ScriptComponent*>(ecs->m_AddComponent(ecs::TYPESCRIPTCOMPONENT, newEntityId));

			if (sc) {
				const rapidjson::Value& script = entityData["script"];

				if (script.HasMember("scripts") && script["scripts"].IsArray()) {
					const rapidjson::Value& scriptArray = script["scripts"];
					for (rapidjson::SizeType i = 0; i < scriptArray.Size(); i++) {
						if (scriptArray[i].IsObject()) { // Ensure the element is an object
							const rapidjson::Value& scriptObject = scriptArray[i];

							// Extract the script name
							if (scriptObject.HasMember("name") && scriptObject["name"].IsString()) {
								std::string scriptName = scriptObject["name"].GetString();

								// Extract the enabled boolean
								bool enabled = false; // Default value
								if (scriptObject.HasMember("enabled") && scriptObject["enabled"].IsBool()) {
									enabled = scriptObject["enabled"].GetBool();
								}

								std::unordered_map<std::string, std::string> variableMap;
								if (scriptObject.HasMember("map") && scriptObject["map"].IsArray()) {
									for (const auto& variable : scriptObject["map"].GetArray()) {
										if (!variable.HasMember("variable") || !variable["variable"].IsString() ||
											!variable.HasMember("base64") || !variable["base64"].IsString()) {
											continue; // Skip invalid map entries
										}

										std::string key = variable["variable"].GetString();
										std::string value = variable["base64"].GetString();
										variableMap[key] = value;
									}
								}

								// Add to the script list
								sc->m_scripts.push_back(std::make_tuple(scriptName, enabled, variableMap));
							}
						}
					}
				}
			}
		}


		if (entityData.HasMember("tilemap") && entityData["tilemap"].IsObject()) {
			ecs::TilemapComponent* tile = static_cast<ecs::TilemapComponent*>(ecs->m_AddComponent(ecs::TYPETILEMAPCOMPONENT, newEntityId));

			if (tile) {
				const rapidjson::Value& tilemap = entityData["tilemap"];
				if (tilemap.HasMember("imagefile")) {
					tile->m_tilemapFile = tilemap["imagefile"].GetString();
				}
				if (tilemap.HasMember("layer")) {
					tile->m_tileLayer = tilemap["layer"].GetInt();
				}

				if (tilemap.HasMember("color") && tilemap["color"].IsObject()) {
					const rapidjson::Value& color = tilemap["color"];
					if (color.HasMember("r")) tile->m_color.m_x = color["r"].GetFloat();
					if (color.HasMember("g")) tile->m_color.m_y = color["g"].GetFloat();
					if (color.HasMember("b")) tile->m_color.m_z = color["b"].GetFloat();
				}
				if (tilemap.HasMember("alpha")) {
					tile->m_alpha = tilemap["alpha"].GetFloat();
				}
				if (tilemap.HasMember("rowlength")) {
					tile->m_rowLength = tilemap["rowlength"].GetInt();
				}
				if (tilemap.HasMember("columnlength")) {
					tile->m_columnLength = tilemap["columnlength"].GetInt();
				}
				if (tilemap.HasMember("pictureRowLength")) {
					tile->m_pictureRowLength = tilemap["pictureRowLength"].GetInt();
				}
				if (tilemap.HasMember("pictureColumnLength")) {
					tile->m_pictureColumnLength = tilemap["pictureColumnLength"].GetInt();
				}
				if (tilemap.HasMember("tilePictureIndex") && tilemap["tilePictureIndex"].IsArray()) 
				{
					const rapidjson::Value& tileArray = tilemap["tilePictureIndex"];
					tile->m_tilePictureIndex.resize(tileArray.Size());
					for (rapidjson::SizeType i = 0; i < tileArray.Size(); ++i) {
						if (tileArray[i].IsArray()) 
						{
							tile->m_tilePictureIndex[i].resize(tileArray[i].Size());
							for (rapidjson::SizeType j = 0; j < tileArray[i].Size(); ++j)
							{
								const rapidjson::Value& tileRow = tileArray[i];
								tile->m_tilePictureIndex[i][j] = tileRow[j].GetInt();
							}
						}
					}
				}
			}
		}

		// Load GridComponent if it exists
		if (entityData.HasMember("grid") && entityData["grid"].IsObject())
		{
			ecs::GridComponent* gridc = static_cast<ecs::GridComponent*>(
				ecs->m_AddComponent(ecs::TYPEGRIDCOMPONENT, newEntityId));

			if (gridc)
			{
				const rapidjson::Value& grid = entityData["grid"];

				if (grid.HasMember("gridRowLength"))
				{
					gridc->m_GridRowLength = grid["gridRowLength"].GetInt();
				}
				if (grid.HasMember("gridColumnLength"))
				{
					gridc->m_GridColumnLength = grid["gridColumnLength"].GetInt();
				}
				if (grid.HasMember("setCollidable"))
				{
					gridc->m_SetCollidable = grid["setCollidable"].GetBool();
				}

				if (grid.HasMember("isWall") && grid["isWall"].IsArray())
				{
					const rapidjson::Value& wallArray = grid["isWall"];
					gridc->m_IsWall.resize(wallArray.Size());

					for (rapidjson::SizeType i = 0; i < wallArray.Size(); ++i)
					{
						if (wallArray[i].IsArray())
						{
							gridc->m_IsWall[i].resize(wallArray[i].Size());
							for (rapidjson::SizeType j = 0; j < wallArray[i].Size(); ++j)
							{
								gridc->m_IsWall[i][j] = wallArray[i][j].GetInt();
							}
						}
					}
				}

				// Load grid key
				if (grid.HasMember("gridKey") && grid["gridKey"].IsInt()) {
					gridc->m_GridKey = grid["gridKey"].GetInt();
				}
			}
		}




		// Load AudioComponent if it exists
		if (entityData.HasMember("audio") && entityData["audio"].IsArray()) {
			ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(
				ecs->m_AddComponent(ecs::TYPEAUDIOCOMPONENT, newEntityId)
				);

			if (ac) {
				const rapidjson::Value& audioArray = entityData["audio"];
				for (rapidjson::SizeType i = 0; i < audioArray.Size(); ++i) {
					const rapidjson::Value& audioObject = audioArray[i];
					ecs::AudioFile audioFile;

					if (audioObject.HasMember("name") && audioObject["name"].IsString()) {
						audioFile.m_Name = audioObject["name"].GetString();
					}
					if (audioObject.HasMember("volume")) {
						audioFile.m_Volume = audioObject["volume"].GetFloat();
					}
					if (audioObject.HasMember("loop")) {
						audioFile.m_Loop = audioObject["loop"].GetBool();
					}
					if (audioObject.HasMember("playonstart")) {
						audioFile.m_PlayOnStart = audioObject["playonstart"].GetBool();
					}
					if (audioObject.HasMember("isBGM")) {
						audioFile.m_IsBGM = audioObject["isBGM"].GetBool();
					}
					if (audioObject.HasMember("isSFX")) {
						audioFile.m_IsSFX = audioObject["isSFX"].GetBool();
					}

					ac->m_AudioFiles.push_back(audioFile);
				}
			}
		}

		if (entityData.HasMember("raycast") && entityData["raycast"].IsObject()) {
			ecs::RaycastComponent* rc = static_cast<ecs::RaycastComponent*>(ecs->m_AddComponent(ecs::TYPERAYCASTINGCOMPONENT,newEntityId));
			if (rc) {
				// Assuming `entityData` is the JSON object containing the entity's data
				if (entityData.HasMember("raycast") && entityData["raycast"].IsObject()) {
					const rapidjson::Value& raycastData = entityData["raycast"];

					if (raycastData.HasMember("raycasts") && raycastData["raycasts"].IsArray()) {
						const rapidjson::Value& rayArray = raycastData["raycasts"];

						rc->m_raycast.clear(); // Clear any existing data in `m_raycast`

						for (const auto& raycastObject : rayArray.GetArray()) {
							ecs::RaycastComponent::Raycast raycast;

							if (raycastObject.HasMember("raycastID") && raycastObject["raycastID"].IsString()) {
								raycast.m_rayID = raycastObject["raycastID"].GetString();
							}

							// Read boolean value
							if (raycastObject.HasMember("israycasting") && raycastObject["israycasting"].IsBool()) {
								raycast.m_isRaycasting = raycastObject["israycasting"].GetBool();
							}

							// Read target position
							if (raycastObject.HasMember("targetposition") && raycastObject["targetposition"].IsObject()) {
								const rapidjson::Value& targetPos = raycastObject["targetposition"];
								if (targetPos.HasMember("x") && targetPos["x"].IsFloat()) {
									raycast.m_targetPosition.m_x = targetPos["x"].GetFloat();
								}
								if (targetPos.HasMember("y") && targetPos["y"].IsFloat()) {
									raycast.m_targetPosition.m_y = targetPos["y"].GetFloat();
								}
							}

							// Read layers
							if (raycastObject.HasMember("Layer Vector") && raycastObject["Layer Vector"].IsArray()) {
								const rapidjson::Value& layerArray = raycastObject["Layer Vector"];
								raycast.m_Layers.clear();

								for (const auto& layerObject : layerArray.GetArray()) {
									if (layerObject.HasMember("layer") && layerObject["layer"].IsInt()) {
										raycast.m_Layers.push_back(static_cast<layer::LAYERS>(layerObject["layer"].GetInt()));
									}
								}
							}

							// Add the raycast object to the component's vector
							rc->m_raycast.push_back(raycast);
						}
					}
				}
			}
		}

		// Load PathfindingComponent if it exists
		if (entityData.HasMember("pathfinding") && entityData["pathfinding"].IsObject()) {
			ecs::PathfindingComponent* pc = static_cast<ecs::PathfindingComponent*>(
				ecs->m_AddComponent(ecs::TYPEPATHFINDINGCOMPONENT, newEntityId)
				);

			if (pc) {
				const rapidjson::Value& pathfindingObject = entityData["pathfinding"];

				// Load start position
				if (pathfindingObject.HasMember("startPosition") && pathfindingObject["startPosition"].IsObject()) {
					const rapidjson::Value& startPosObject = pathfindingObject["startPosition"];
					if (startPosObject.HasMember("x") && startPosObject["x"].IsFloat()) {
						pc->m_StartPos.m_x = startPosObject["x"].GetFloat();
					}
					if (startPosObject.HasMember("y") && startPosObject["y"].IsFloat()) {
						pc->m_StartPos.m_y = startPosObject["y"].GetFloat();
					}
				}

				// Load target position
				if (pathfindingObject.HasMember("targetPosition") && pathfindingObject["targetPosition"].IsObject()) {
					const rapidjson::Value& targetPosObject = pathfindingObject["targetPosition"];
					if (targetPosObject.HasMember("x") && targetPosObject["x"].IsFloat()) {
						pc->m_TargetPos.m_x = targetPosObject["x"].GetFloat();
					}
					if (targetPosObject.HasMember("y") && targetPosObject["y"].IsFloat()) {
						pc->m_TargetPos.m_y = targetPosObject["y"].GetFloat();
					}
				}

				// Load grid key
				if (pathfindingObject.HasMember("gridKey") && pathfindingObject["gridKey"].IsInt()) {
					pc->m_GridKey = pathfindingObject["gridKey"].GetInt();
				}
			}
		}

		// Load Particle Component if it exists
		if (entityData.HasMember("particle") && entityData["particle"].IsObject()) {
			ecs::ParticleComponent* particle = static_cast<ecs::ParticleComponent*>(ecs->m_AddComponent(ecs::TYPEPARTICLECOMPONENT, newEntityId));

			if (particle) {
				const rapidjson::Value& par = entityData["particle"];

				if (par.HasMember("willSpawn")) particle->m_willSpawn = par["willSpawn"].GetBool();
				if (par.HasMember("noOfParticles")) particle->m_noOfParticles = par["noOfParticles"].GetInt();
				if (par.HasMember("lifeSpan")) particle->m_lifeSpan = par["lifeSpan"].GetFloat();
				if (par.HasMember("velocityX")) particle->m_velocity.m_x = par["velocityX"].GetFloat();
				if (par.HasMember("velocityY")) particle->m_velocity.m_y = par["velocityY"].GetFloat();
				if (par.HasMember("sizeX")) particle->m_particleSize.m_x = par["sizeX"].GetFloat();
				if (par.HasMember("sizeY")) particle->m_particleSize.m_y = par["sizeY"].GetFloat();
				if (par.HasMember("accelerationX")) particle->m_acceleration.m_x = par["accelerationX"].GetFloat();
				if (par.HasMember("accelerationY")) particle->m_acceleration.m_y = par["accelerationY"].GetFloat();
				if (par.HasMember("colorR")) particle->m_color.m_x = par["colorR"].GetFloat();
				if (par.HasMember("colorG")) particle->m_color.m_y = par["colorG"].GetFloat();
				if (par.HasMember("colorB")) particle->m_color.m_z = par["colorB"].GetFloat();
				if (par.HasMember("coneRotation")) particle->m_coneRotation = par["coneRotation"].GetFloat();
				if (par.HasMember("coneAngle")) particle->m_coneAngle = par["coneAngle"].GetFloat();
				if (par.HasMember("randomFactor")) particle->m_randomFactor = par["randomFactor"].GetFloat();
				if (par.HasMember("imageFile")) particle->m_imageFile = par["imageFile"].GetString();
				if (par.HasMember("stripCount")) particle->m_stripCount = par["stripCount"].GetInt();
				if (par.HasMember("frameNumber")) particle->m_frameNumber = par["frameNumber"].GetInt();
				if (par.HasMember("layer")) particle->m_layer = par["layer"].GetInt();
				if (par.HasMember("friction")) particle->m_friction = par["friction"].GetFloat();
				if (par.HasMember("fps")) particle->m_fps = par["fps"].GetInt();
				if (par.HasMember("loopAnimation")) particle->m_willSpawn = par["loopAnimation"].GetBool();
			}
		}



		if (entityData.HasMember(ecs::LightingComponent::classname()) && entityData[ecs::LightingComponent::classname()].IsObject()) {
			ecs::LightingComponent* lc = static_cast<ecs::LightingComponent*>(
				ecs->m_AddComponent(ecs::TYPELIGHTINGCOMPONENT, newEntityId)
				);

			if (lc) {
				m_LoadComponentreflect(lc, entityData);
			}
		}

		if (entityData.HasMember(ecs::VideoComponent::classname()) && entityData[ecs::VideoComponent::classname()].IsObject()) {
			ecs::VideoComponent* vc = static_cast<ecs::VideoComponent*>(
				ecs->m_AddComponent(ecs::TYPEVIDEOCOMPONENT, newEntityId)
				);

			if (vc) {
				m_LoadComponentreflect(vc, entityData);
			}
		}


		//Attach entity to parent
		if (parentID.has_value()) {
			ecs::Hierachy::m_SetParent(parentID.value(), newEntityId);
		}

		// Load children 
		if (entityData.HasMember("children") && entityData["children"].IsArray()) {
			const rapidjson::Value& childrenArray = entityData["children"];
			for (rapidjson::SizeType i = 0; i < childrenArray.Size(); i++) {
				m_LoadEntity(childrenArray[i], newEntityId, sceneName);
			}
		}
	}
	void Serialization::Serialize::m_SavePhysicsLayerMatrix() {
		std::ofstream file(layerFilePath);
		if (!file.is_open()) {
			LOGGING_ERROR("Could not open LayerConfig.txt for writing.");
			return;
		}
		auto size = physicslayer::PhysicsLayer::m_GetSize();
		auto matrix = physicslayer::PhysicsLayer::m_GetInstance()->m_GetMat();
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				file << matrix[i][j] << " ";
			}
			file << "\n";
		}

		file << "LayerBitset: " << ecs::ECS::m_GetInstance()->m_layersStack.m_layerBitSet << std::endl;

		file.close();
		//LOGGING_INFO("Collision matrix saved to PhysicsLayerMatrix.txt");
	}

	void Serialization::Serialize::m_LoadPhysicsLayerMatrix() {
		std::ifstream file(layerFilePath);
		if (!file.is_open()) {
			LOGGING_ERROR("Could not open LayerConfig.txt for reading.");
			return;
		}

		std::string line;
		int row = 0;
		auto size = physicslayer::PhysicsLayer::m_GetSize();
		physicslayer::PhysicsLayer* layer = physicslayer::PhysicsLayer::m_GetInstance();
		while ( row < size && std::getline(file, line)) {
			std::istringstream iss(line);
			int col = 0;
			int value;
			while (iss >> value && col < size) {
				layer->m_SetCollision(row, col, value == 1);
				++col;
			}
			++row;
		}

		std::getline(file, line);
		std::istringstream iss(line);
		std::string temp;
		iss >> temp;
		if (temp == "LayerBitset:") {
			std::string _bitset;
			iss >> _bitset;
			ecs::ECS::m_GetInstance()->m_layersStack.m_layerBitSet = std::bitset<layer::MAXLAYER>(_bitset);
		}


		file.close();
		LOGGING_INFO("Collision matrix loaded from LayerConfig.txt");
	}


	std::string Serialize::m_EncodeBase64(const void* data, size_t size)
	{
		static const char encodingTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		if (!data || size == 0) {
			return ""; // Return empty string if no data
		}

		const uint8_t* bytes = static_cast<const uint8_t*>(data);
		std::string encoded;
		encoded.reserve((size + 2) / 3 * 4); // Reserve space for performance

		for (size_t i = 0; i < size; i += 3) {
			uint32_t value = 0;
			int count = 0;

			// Read up to 3 bytes
			for (int j = 0; j < 3; ++j) {
				if (i + j < size) {
					value |= (bytes[i + j] << (16 - j * 8));
					count++;
				}
			}

			// Encode into 4 Base64 characters
			for (int j = 0; j < 4; ++j) {
				if (j <= (count + 1)) {
					encoded += encodingTable[(value >> (18 - j * 6)) & 0x3F];
				}
				else {
					encoded += '='; // Padding
				}
			}
		}

		return encoded;
	}

	std::unique_ptr<void, Serialize::VoidDeleter> Serialize::DecodeBase64(const std::string& base64) {
		static const int DECODING_TABLE[256] = {
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 32-47  (+ and /)
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // 48-63  (0-9)
			-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,           // 64-79  (A-O)
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 80-95  (P-Z)
			-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 96-111 (a-o)
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1  // 112-127 (p-z)
		};

		if (base64.empty()) return nullptr;

		size_t inputLen = base64.length();
		size_t padding = (base64[inputLen - 1] == '=') + (base64[inputLen - 2] == '=');
		
		size_t outSize = (inputLen * 3) / 4 - padding;

		// Allocate memory using malloc (will be managed by unique_ptr)
		auto buffer = std::unique_ptr<void, VoidDeleter>(malloc(outSize), VoidDeleter());
		if (!buffer) return nullptr; // Memory allocation failed

		uint8_t* output = static_cast<uint8_t*>(buffer.get());
		uint32_t value = 0;
		int bits = -8;
		size_t index = 0;

		for (char c : base64) {
			if (c == '=') break;
			int decoded = (c >= 0 && c <= 127) ? DECODING_TABLE[c] : -1;
			if (decoded == -1) continue;

			value = (value << 6) | decoded;
			bits += 6;

			if (bits >= 0) {
				output[index++] = (value >> bits) & 0xFF;
				bits -= 8;
			}
		}

		return buffer;
	}
}

