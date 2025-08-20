/********************************************************************/
/*!
\file      json_handler.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 29, 2024
\brief     This header file declares functions that handle JSON-related operations for component serialization and
		   deserialization in the ECS framework.
		   - m_LoadConfig: Loads configuration settings such as window dimensions and FPS cap from a config file.
		   - m_JsonFileValidation: Validates if the JSON file exists and creates a new one if it doesn't.
		   - m_LoadComponentsJson: Loads component data from a JSON file into the ECS.
		   - m_SaveComponentsJson: Saves component data from the ECS to a JSON file.
		   - m_SaveEntity: Serializes individual entity data to a JSON structure.
		   - m_LoadEntity: Deserializes individual entity data from a JSON structure.
		   - m_SavePhysicsLayerMatrix: Saves the current physics layer matrix to a JSON file.
		   - m_LoadPhysicsLayerMatrix: Loads the physics layer matrix from a JSON file.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef JSONHANDLER_H
#define JSONHANDLER_H


#include "../ECS/ECS.h"
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Physics/PhysicsLayer.h"
#include "../Graphics/GraphicsLighting.h"

namespace Serialization {
	class Serialize {
	

		template <typename T>//T refers to component
		static void m_saveComponentreflect(T* component, rapidjson::Value& entityData, rapidjson::Document::AllocatorType& allocator);

		template <typename T>//T refers to component
		static void m_LoadComponentreflect(T* component, const rapidjson::Value& entityData);

	public:

		/******************************************************************/
		/*!
			\fn        Serialize::m_LoadComponentsJson(const std::string& jsonFilePath)
			\brief     Loads component data from a JSON file into the ECS framework.
			\param[in] jsonFilePath The path to the JSON file containing the component data.
			\details   This function reads the specified JSON file and parses component data for various components such as
					   `TransformComponent`, `MovementComponent`, `ColliderComponent`, `PlayerComponent`, and others.
					   The data is then applied to the appropriate entities in the ECS.
		*/
		/******************************************************************/
		static void m_LoadComponentsJson(const std::filesystem::path& jsonFilePath);

		/******************************************************************/
		/*!
			\fn        Serialize::m_SaveComponentsJson(const std::string& filePath, const std::unordered_map<ecs::EntityID, std::bitset<ecs::ComponentType::TOTALTYPECOMPONENT>>& ECS_EntityMap, const std::vector<std::string>& objTextEntries, const std::vector<ecs::EntityID>& objEntityId)
			\brief     Saves component data from the ECS to a JSON file.
			\param[in] filePath      The path to save the JSON file.
			\param[in] ECS_EntityMap The map of entities and their associated components.
			\param[in] objTextEntries The list of entity names to save.
			\param[in] objEntityId   The list of entity IDs to save.
			\details   This function iterates through the ECS entity map and saves relevant component data such as `TransformComponent`,
					   `MovementComponent`, `ColliderComponent`, `PlayerComponent`, and more. The saved data is written to a JSON file
					   for later retrieval and reloading.
		*/
		/******************************************************************/
		static void m_SaveComponentsJson(const std::filesystem::path& filePath);

		/******************************************************************/
		/*!
			\fn        Serialize::m_SaveEntity(ecs::EntityID entityId, rapidjson::Value& parentArray, rapidjson::Document::AllocatorType& allocator, std::unordered_set<ecs::EntityID>& savedEntities)
			\brief     Saves an individual entity's component data to a JSON structure.
			\param[in] entityId       The ID of the entity to be saved.
			\param[out] parentArray   The JSON value to which the entity data will be appended.
			\param[in] allocator      The allocator used for JSON memory management.
			\param[in,out] savedEntities A set that keeps track of saved entities to avoid redundant saves.
			\details   This function serializes components such as `TransformComponent`, `CameraComponent`, `ScriptComponent`, and others.
					   It appends the serialized entity data to the given JSON array, allowing hierarchical saving of parent-child relationships.
		*/
		/******************************************************************/
		static void m_SaveEntity(ecs::EntityID entityId, rapidjson::Value& parentArray, rapidjson::Document::AllocatorType& allocator, std::unordered_set<ecs::EntityID>& savedEntities);

		/******************************************************************/
		/*!
			\fn        Serialize::m_LoadEntity(const rapidjson::Value& entityData, std::optional<ecs::EntityID> parentID, const std::string& sceneName)
			\brief     Loads an individual entity's component data from a JSON structure.
			\param[in] entityData     The JSON value containing the entity data.
			\param[in] parentID       The optional ID of the parent entity, used to maintain parent-child relationships.
			\param[in] sceneName      The name of the scene to which this entity belongs.
			\details   This function parses component data from a JSON object and assigns it to the entity in the ECS. Components such as
					   `TransformComponent`, `CameraComponent`, `ScriptComponent`, `TextComponent`, `SpriteComponent`, etc., are reconstructed.
		*/
		/******************************************************************/
		static void m_LoadEntity(const rapidjson::Value& entityData, std::optional<ecs::EntityID> parentID, const std::string& sceneName);

		/******************************************************************/
		/*!
			\fn        Serialize::m_LoadConfig()
			\brief     Loads configuration settings such as window dimensions and FPS cap from a config file.
			\details   This function reads the configuration file `Config.txt`, extracting window height, width, and FPS cap values,
					   and assigns them to the corresponding variables in the `Helper::Helpers` class.
		*/
		/******************************************************************/
		static void m_LoadConfig(std::string);
		static std::vector<std::string> m_LoadFilePath(std::string);

		static bool m_UpdateConfigStartScene(const std::string& newStartScene);

		/******************************************************************/
		/*!
			\fn        Serialize::m_JsonFileValidation(const std::string& filePath)
			\brief     Validates if the JSON file exists and creates a new one if it doesn't.
			\param[in] filePath The path to the JSON file to be validated.
			\details   This function checks if the specified JSON file exists. If it does not, it creates a new JSON file and initializes
					   it with an empty array structure (`[]`).
		*/
		/******************************************************************/
		static void m_JsonFileValidation(const std::string& filePath);

		/******************************************************************/
		/*!
		\fn      static void m_SavePhysicsLayerMatrix()
		\brief   Saves the current physics layer matrix to a JSON file.
		\details This function serializes the matrix that determines interactions between physics layers,
				 allowing it to be persisted and reloaded later.
		*/
		/******************************************************************/
		static void m_SavePhysicsLayerMatrix();

		/******************************************************************/
		/*!
		\fn      static void m_LoadPhysicsLayerMatrix()
		\brief   Loads the physics layer matrix from a JSON file.
		\details This function reads a JSON file to restore the matrix that determines interactions
				 between physics layers, ensuring consistent physics behavior across sessions.
		*/
		/******************************************************************/
		static void m_LoadPhysicsLayerMatrix();

		///******************************************************************/
		///*!
		//	\fn        void Serialize::m_SaveGlobalSettings()
		//	\brief     Saves the global settings of the application to a JSON file.
		//	\details   This function serializes all global settings, including graphics and user preferences, into a JSON format and writes them to a file. This ensures that settings are persistent across sessions and can be reloaded at startup.
		//*/
		///******************************************************************/
		//static void m_SaveGlobaalSettings();

		///******************************************************************/
		///*!
		//	\fn        void Serialize::m_LoadGlobalSettings()
		//	\brief     Loads global settings from a JSON file.
		//	\details   This function reads a JSON file containing global settings, such as window size, audio preferences, and graphical settings, and applies these settings to the application upon startup or when settings are reset.
		//*/
		///******************************************************************/
		//static void m_LoadGlobalSettings();

		static std::string m_EncodeBase64(const void* data, size_t);

		struct VoidDeleter {
			void operator()(void* ptr) const {
				free(ptr);  // Free the memory allocated by malloc
			}
		};


		static std::unique_ptr<void, VoidDeleter> DecodeBase64(const std::string& base64);
	
		static std::string configFilePath;

	};


	template <typename T>
	struct SaveComponent {

		T m_Array;
		int count{};



		void operator()(float& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);
			value.AddMember(key, _args, allocator);
			count++;
		}


		void operator()(int& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			value.AddMember(key, _args, allocator);
			count++;
		}

		void operator()(graphicpipe::LightType& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			value.AddMember(key, _args, allocator);
			count++;
		}

		void operator()(vector2::Vec2& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			value.AddMember(key, rapidjson::Value().SetObject()
				.AddMember("x", _args.m_x, allocator)
				.AddMember("y", _args.m_y, allocator), allocator);
			count++;
		}

		void operator()(vector3::Vec3& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			value.AddMember(key, rapidjson::Value().SetObject()
				.AddMember("x", _args.m_x, allocator)
				.AddMember("y", _args.m_y, allocator)
				.AddMember("z", _args.m_z, allocator), allocator);
			count++;
		}


		void operator()(bool& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			value.AddMember(key, _args, allocator);
			count++;
		}

		void operator()(std::string& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			rapidjson::Value jsonString;
			jsonString.SetString(_args.c_str(), allocator); // Set the string
			value.AddMember(rapidjson::Value(m_Array[count].c_str(), allocator), jsonString, allocator); // Add the key-value pair
			count++;
		}


		template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
		void operator()(EnumType& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			// Create a rapidjson::Value key from the string
			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			// Add the member using the key and integer value
			value.AddMember(key, static_cast<int>(_args), allocator);

			count++;
		}

		template <typename U>
		void operator()(std::vector<U>& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			if constexpr (std::is_class_v<U>) {
				rapidjson::Value Array(rapidjson::kArrayType);
				for (U& x : _args) {
					rapidjson::Value name(rapidjson::kObjectType);
					SaveComponent<decltype(x.Names())> saver{ x.Names() };

					x.ApplyFunction([&](auto& member) {
						saver(member, name, allocator);  // Apply SaveEntity to each member
						});

					Array.PushBack(name, allocator);
				}

				rapidjson::Value key;
				key.SetString(m_Array[count].c_str(), allocator);

				value.AddMember(key, Array, allocator);
			}
			else {
				rapidjson::Value Array(rapidjson::kArrayType);
				for (U& x : _args) {

					rapidjson::Value name(rapidjson::kObjectType);
					(*this)(x, name, allocator); // Handle non-class types
					Array.PushBack(name, allocator);
					count--;// minus so no subsciprt error

				}

				rapidjson::Value key;
				key.SetString(m_Array[count].c_str(), allocator);

				value.AddMember(key, Array, allocator);
			}
			count++;
		}

		template <typename K>
		void operator()(K& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
			if constexpr (std::is_class_v<K>) {
				rapidjson::Value name(rapidjson::kObjectType);
				SaveComponent<decltype(_args.Names())> saver{ _args.Names() };

				_args.ApplyFunction([&](auto& member) {
					saver(member, name, allocator);  // Apply SaveEntity to each member
					});

				value.AddMember(rapidjson::Value(_args.classname(), allocator), name, allocator);
				//_args.ApplyFunction(DrawComponents<decltype(_args.Names())>{_args.Names()});
				count++;
			}

		}

	};

	template <typename T>
	struct LoadComponent {

		T m_Array;
		int count{};



		void operator()(float& _args, const rapidjson::Value& value) {

			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsFloat()) {
				_args = value[m_Array[count].c_str()].GetFloat();
			}

			count++;
		}


		void operator()(int& _args, const rapidjson::Value& value) {

			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsInt()) {
				_args = value[m_Array[count].c_str()].GetInt();
			}

			count++;
		}

		void operator()(graphicpipe::LightType& _args, const rapidjson::Value& value) {

			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsInt()) {
				_args = static_cast<graphicpipe::LightType>(value[m_Array[count].c_str()].GetInt());
			}

			count++;
		}

		void operator()(vector2::Vec2& _args, const rapidjson::Value& value) {

			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsObject()) {
				const rapidjson::Value& vector = value[m_Array[count].c_str()];
				if (vector.HasMember("x") && vector["x"].IsFloat()) {
					_args.m_x = vector["x"].GetFloat();
				}
				if (vector.HasMember("y") && vector["y"].IsFloat()) {
					_args.m_y = vector["y"].GetFloat();
				}
			}

			count++;
		}

		void operator()(vector3::Vec3& _args, const rapidjson::Value& value) {

			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsObject()) {
				const rapidjson::Value& vector = value[m_Array[count].c_str()];
				if (vector.HasMember("x") && vector["x"].IsFloat()) {
					_args.m_x = vector["x"].GetFloat();
				}
				if (vector.HasMember("y") && vector["y"].IsFloat()) {
					_args.m_y = vector["y"].GetFloat();
				}
				if (vector.HasMember("z") && vector["z"].IsFloat()) {
					_args.m_z = vector["z"].GetFloat();
				}
			}

			count++;
		}


		void operator()(bool& _args, const rapidjson::Value& value) {

			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsBool()) {
				_args = value[m_Array[count].c_str()].GetBool();
			}

			count++;
		}

		void operator()(std::string& _args, const rapidjson::Value& value) {
			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsString()) {
				_args = value[m_Array[count].c_str()].GetString();
			}

			count++;
		}

		template <typename EnumType, typename = std::enable_if_t<std::is_enum_v<EnumType>>>
		void operator()(EnumType& _args, const rapidjson::Value& value) {
			if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsInt()) {
				_args = static_cast<EnumType>(value[m_Array[count].c_str()].GetInt());
			}
			count++;
		}

		template <typename U>
		void operator()(std::vector<U>& _args, const rapidjson::Value& value) {
			if constexpr (std::is_class_v<U>) {
				if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsArray()) {

					const rapidjson::Value& Array = value[m_Array[count].c_str()];

					for (rapidjson::SizeType i = 0; i < Array.Size(); i++) {
						const rapidjson::Value& Object = Array[i];

						LoadComponent<decltype(U::Names())> loader{ U::Names() };

						U temp;

						temp.ApplyFunction([&](auto& member) {
							loader(member, Object);
							});

						_args.push_back(temp);
					}
				}

			}
			else {
				const rapidjson::Value& Array = value[m_Array[count].c_str()];

				for (rapidjson::SizeType i = 0; i < Array.Size(); i++) {
					const rapidjson::Value& name = Array[i];

					U temp;
					(*this)(temp, name); // Handle non-class types

					_args.push_back(temp);
					count--; //offset 
				}
			}
			count++;
		}

		template <typename K>
		void operator()(K& _args, const rapidjson::Value& value) {
			if constexpr (std::is_class_v<K>) {
				const rapidjson::Value& name = value[_args.classname()];
				LoadComponent<decltype(_args.Names())> loader{ _args.Names() };

				_args.ApplyFunction([&](auto& member) {
					loader(member, name);
					});

				count++;
			}

		}


	};

	template<typename T>
	inline void Serialize::m_saveComponentreflect(T* component, rapidjson::Value& entityData, rapidjson::Document::AllocatorType& allocator)
	{
		if (component == nullptr) return;
		rapidjson::Value name(rapidjson::kObjectType);
		SaveComponent<decltype(component->Names())> saver{ component->Names() };  // Create SaveEntity with member names

		component->ApplyFunction([&](auto& member) {
			saver(member, name, allocator);  // Apply SaveEntity to each member
			});

		entityData.AddMember(rapidjson::Value(component->classname(), allocator), name, allocator);
	}
	template<typename T>
	inline void Serialize::m_LoadComponentreflect(T* component, const rapidjson::Value& entityData)
	{
		if (component == nullptr) return;

		const rapidjson::Value& name = entityData[T::classname()];
		LoadComponent<decltype(T::Names())> loader{ T::Names() };
		
		component->ApplyFunction([&](auto& member) {
			loader(member, name);
			});

	}
}
#endif JSONHANDLER_H

