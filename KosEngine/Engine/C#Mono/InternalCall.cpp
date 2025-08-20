/********************************************************************/
/*!
\file      InternalCall.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 27 , 2024
\brief     This header file implements the functions that allow C# scripts to interact with ECS components.

Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "InternalCall.h"
#include "../Inputs/Input.h"
#include "../Helper/Helper.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/SceneManager.h"
#include "../Graphics/GraphicsCamera.h"
#include "../ECS/Hierachy.h"

#include "../Pathfinding/AStarPathfinding.h"
#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>

#define MONO_ADD_INTERNAL_CALL(METHOD_NAME) \
    mono_add_internal_call("InternalCall::" #METHOD_NAME, METHOD_NAME);

#define ASSERTNOCOMPONENT(COMPONENTNAME, ID) \
    LOGGING_WARN("Entity " + std::to_string(ID) + " does not have " + #COMPONENTNAME)

namespace script {

	bool InternalCall::m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
	{

		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));

		if (transform) {
			*trans = { transform->m_position.m_x, transform->m_position.m_y };
			*scale = { transform->m_scale.m_x, transform->m_scale.m_y };
			*rotate = { transform->m_rotation };

		}
		else {
			ASSERTNOCOMPONENT(TransformComponent, entity);

		}



		return true;
	}

	bool InternalCall::m_InternalSetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));

		if (transform) {

			transform->m_position = *trans;
			transform->m_scale = *scale;
			transform->m_rotation = *rotate;
		}
		else {
			ASSERTNOCOMPONENT(TransformComponent, entity);

		}
		return true;
	}

	bool InternalCall::m_InternalGetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
	{

		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity)); 

		if (transform) {
			vector2::Vec2 translate, rotate; 
			float scale; 
			mat3x3::Mat3Decompose(transform->m_transformation, translate, rotate, scale); 
			*trans = translate;
		}
		else {
			ASSERTNOCOMPONENT(TransformComponent, entity);

		}
		return true;
	}

	bool InternalCall::m_InternalSetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
	{
		auto* transform = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entity));
		if (transform) {
			transform->m_position = *trans;
		}
		else {
			ASSERTNOCOMPONENT(TransformComponent, entity);

		}
		return true;
	}

	//Collider Component
	bool InternalCall::m_InternalGetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, unsigned int* m_blockedFlag, float* isCollided, bool* collisionCheck, bool* collisionresponse)
	{
		auto* collider = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entity));

		if (collider) {
			*size = collider->m_Size;
			*offset = collider->m_OffSet;
			*drawDebug = collider->m_drawDebug;
			*radius = collider->m_radius;
			*isCollided = collider->m_isCollided;
			*m_blockedFlag = collider->m_blockedFlag;
			*collisionCheck = collider->m_collisionCheck;
			*collisionresponse = collider->m_collisionResponse;
		}
		else {
			ASSERTNOCOMPONENT(ColliderComponent, entity);
		}
		

		return true;
	}

	bool InternalCall::m_InternalSetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, unsigned int* m_blockedFlag, float* isCollided, bool* collisionCheck, bool* collisionresponse)
	{
		auto* collider = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entity));

		if (collider) {

			collider->m_Size = *size;
			collider->m_OffSet = *offset;
			collider->m_drawDebug = *drawDebug;
			collider->m_radius = *radius;
			collider->m_isCollided = *isCollided;
			collider->m_blockedFlag = *m_blockedFlag;
			collider->m_collisionResponse = *collisionresponse;
			collider->m_collisionCheck = *collisionCheck;
		}
		else {
			ASSERTNOCOMPONENT(ColliderComponent, entity);
		}

		return true;
	}

	//Player Component
	bool InternalCall::m_InternalGetEnemyComponent(ecs::EntityID entity, int* enemytag, int* enemytype, int* enemybehaviour)
	{
		auto* player = static_cast<ecs::EnemyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEENEMYCOMPONENT]->m_GetEntityComponent(entity));

		if (player) {
			*enemytag = player->m_enemyTag;
			*enemytype = player->m_enemyTypeInt;
			*enemybehaviour = player->m_enemyRoamBehaviourInt;
		}
		else {
			ASSERTNOCOMPONENT(EnemyComponent, entity);
		}
		

		return true;
	}

	bool InternalCall::m_InternalSetEnemyComponent(ecs::EntityID entity, int* enemytag, int* enemytype, int* enemybehaviour)
	{
		auto* player = static_cast<ecs::EnemyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEENEMYCOMPONENT]->m_GetEntityComponent(entity));

		if (player) {
			player->m_enemyTag = *enemytag;
			player->m_enemyTag = *enemytype;
			player->m_enemyRoamBehaviourInt = *enemybehaviour;
		}
		else {
			ASSERTNOCOMPONENT(EnemyComponent, entity);
		}


		return true;
	}

	//RigidBody Component
	bool InternalCall::m_InternalGetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation, vector2::Vec2* previouspos, vector2::Vec2* directionvector, vector2::Vec2* force)
	{
		auto* rbComponent = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entity));

		if (rbComponent) {
			*velocity = rbComponent->m_Velocity;
			*acceleration = rbComponent->m_Acceleration;
			*rotation = rbComponent->m_Rotation;
			*previouspos = rbComponent->m_PrevPos;
			*directionvector = rbComponent->m_DirectionVector;
			*force = rbComponent->m_Force;
		}
		else {
			ASSERTNOCOMPONENT(RigidBodyComponent, entity);
		}


		return true;
	}

	bool InternalCall::m_InternalSetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation, vector2::Vec2* previouspos, vector2::Vec2* directionvector, vector2::Vec2* force)
	{
		auto* rbComponent = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entity));

		if (rbComponent) {
			rbComponent->m_Velocity = *velocity;
			rbComponent->m_Acceleration = *acceleration;
			rbComponent->m_Rotation = *rotation;
			rbComponent->m_PrevPos = *previouspos;
			rbComponent->m_DirectionVector = *directionvector;
			rbComponent->m_Force = *force;
		}
		else {
			ASSERTNOCOMPONENT(RigidBodyComponent, entity);
		}
		

		return true;
	}

	//Text Component
	bool InternalCall::m_InternalGetTextComponent(ecs::EntityID entity, MonoString** text, MonoString** fileName, int* fontLayer, float* fontSize, vector3::Vec3* color)
	{
		auto* textComponent = static_cast<ecs::TextComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entity));

		if (textComponent) {
			*text = mono_string_new(mono_domain_get(), textComponent->m_text.c_str());
			*fileName = mono_string_new(mono_domain_get(), textComponent->m_fileName.c_str());

			*fontLayer = textComponent->m_fontLayer;
			*fontSize = textComponent->m_fontSize;
			*color = textComponent->m_color;
		}
		else {
			ASSERTNOCOMPONENT(TextComponent, entity);
		}



		return true;
	}

	bool InternalCall::m_InternalSetTextComponent(ecs::EntityID entity, MonoString* text, MonoString* fileName, int* fontLayer, float* fontSize, const vector3::Vec3* color)
	{
		auto* textComponent = static_cast<ecs::TextComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETEXTCOMPONENT]->m_GetEntityComponent(entity));

		if (textComponent) {
			char* nativeText = mono_string_to_utf8(text);
			std::string ctText = nativeText;
			textComponent->m_text = ctText;

			char* nativeFile = mono_string_to_utf8(fileName);
			std::string ctFileName = nativeFile;
			textComponent->m_fileName = ctFileName;

			textComponent->m_fontLayer = *fontLayer;
			textComponent->m_fontSize = *fontSize;
			textComponent->m_color = *color;

			mono_free(nativeText);
			mono_free(nativeFile);
		}
		else {
			ASSERTNOCOMPONENT(TextComponent, entity);
		}

		return true;
	}

	//Animation Component
	bool InternalCall::m_InternalGetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount)
	{
		auto* animComponent = static_cast<ecs::AnimationComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entity));

		if (animComponent) {
			*frameNumber = animComponent->m_frameNumber;
			*framesPerSecond = animComponent->m_framesPerSecond;
			*frameTimer = animComponent->m_frameTimer;
			*isAnimating = animComponent->m_isAnimating;
			*stripcount = animComponent->m_stripCount;
		}
		else {
			ASSERTNOCOMPONENT(AnimationComponent, entity);
		}


		return true;
	}

	bool InternalCall::m_InternalSetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount)
	{
		auto* animComponent = static_cast<ecs::AnimationComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entity));

		if (animComponent) {
			animComponent->m_frameNumber = *frameNumber;
			animComponent->m_framesPerSecond = *framesPerSecond;
			animComponent->m_frameTimer = *frameTimer;
			animComponent->m_isAnimating = *isAnimating;
			animComponent->m_stripCount = *stripcount;
		}
		else {
			ASSERTNOCOMPONENT(AnimationComponent, entity);
		}


		return true;
	}


	//Sprite Component
	bool InternalCall::m_InternalGetSpriteComponent(ecs::EntityID entity, MonoString** imageFile, int* layer, vector3::Vec3* color, float* alpha)
	{
		auto* spriteComponent = static_cast<ecs::SpriteComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entity));

		if (spriteComponent) {
			*imageFile = mono_string_new(mono_domain_get(), spriteComponent->m_imageFile.c_str());
			*layer = spriteComponent->m_layer;
			*color = spriteComponent->m_color;
			*alpha = spriteComponent->m_alpha;
		}
		else {
			ASSERTNOCOMPONENT(SpriteComponent, entity);
		}




		return true;
	}

	bool InternalCall::m_InternalSetSpriteComponent(ecs::EntityID entity, MonoString* imageFile, const int* layer, const vector3::Vec3* color, const float* alpha)
	{
		auto* spriteComponent = static_cast<ecs::SpriteComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(entity));

		if (spriteComponent) {
			char* nativeString = mono_string_to_utf8(imageFile);
			std::string imagefile = nativeString;

			spriteComponent->m_imageFile = imagefile;
			spriteComponent->m_layer = *layer;
			spriteComponent->m_color = *color;
			spriteComponent->m_alpha = *alpha;

			mono_free(nativeString);
		}
		else {
			ASSERTNOCOMPONENT(SpriteComponent, entity);
		}


		return true;
	}

	//Camera Component
	bool InternalCall::m_InternalGetCameraComponent(ecs::EntityID entity, float* left, float* right, float* top, float* bottom, float* aspectRatio)
	{
		auto* cameraComponent = static_cast<ecs::CameraComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECAMERACOMPONENT]->m_GetEntityComponent(entity));

		if (cameraComponent) {
			*left = cameraComponent->m_left;
			*right = cameraComponent->m_right;
			*top = cameraComponent->m_top;
			*bottom = cameraComponent->m_bottom;
			*aspectRatio = cameraComponent->m_aspectRatio;
		}
		else {
			ASSERTNOCOMPONENT(CameraComponent, entity);
		}


		return true;
	}

	bool InternalCall::m_InternalSetCameraComponent(ecs::EntityID entity, float left, float right, float top, float bottom, float aspectRatio)
	{
		auto* cameraComponent = static_cast<ecs::CameraComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECAMERACOMPONENT]->m_GetEntityComponent(entity));

		if (cameraComponent) {
			cameraComponent->m_left = left;
			cameraComponent->m_right = right;
			cameraComponent->m_top = top;
			cameraComponent->m_bottom = bottom;
			cameraComponent->m_aspectRatio = aspectRatio;
		}
		else {
			ASSERTNOCOMPONENT(CameraComponent, entity);
		}


		return true;
	}

	//Button Components
	bool InternalCall::m_InternalGetButtonComponent(ecs::EntityID entity, vector2::Vec2* position, vector2::Vec2* scale, bool* isClick)
	{
		auto* buttonComponent = static_cast<ecs::ButtonComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEBUTTONCOMPONENT]->m_GetEntityComponent(entity));

		if (buttonComponent) {
			*position = buttonComponent->m_Position;
			*scale = buttonComponent->m_Scale;
			*isClick = buttonComponent->m_IsClick;
		}
		else {
			ASSERTNOCOMPONENT(ButtonComponent, entity);
		}


		return true;
	}

	bool InternalCall::m_InternalIsButtonHovered(ecs::EntityID entity)
	{
		auto* buttonComponent = static_cast<ecs::ButtonComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEBUTTONCOMPONENT]->m_GetEntityComponent(entity));

		if (buttonComponent) {
			return buttonComponent->m_IsHover;
		}
		else {
			ASSERTNOCOMPONENT(ButtonComponent, entity);
		}


		return false;
	}

	bool InternalCall::m_InternalSetButtonComponent(ecs::EntityID entity, const vector2::Vec2& position, const vector2::Vec2& scale, bool isClick)
	{
		auto* buttonComponent = static_cast<ecs::ButtonComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEBUTTONCOMPONENT]->m_GetEntityComponent(entity));

		if (buttonComponent) {
			buttonComponent->m_Position = position;
			buttonComponent->m_Scale = scale;
			buttonComponent->m_IsClick = isClick;
		}
		else {
			ASSERTNOCOMPONENT(ButtonComponent, entity);
		}


		return true;
	}

	//Script Components
	MonoArray* InternalCall::m_InternalGetScriptNames(ecs::EntityID entity)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::ScriptComponent* scriptComponent = static_cast<ecs::ScriptComponent*>(
			ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(entity)
			);

		if (!scriptComponent) return nullptr;
		

		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		MonoDomain* domain = assetManager->m_scriptManager.m_GetDomain();

		const auto& scripts = scriptComponent->m_scripts;
		MonoArray* scriptArray = mono_array_new(domain, mono_get_string_class(), scripts.size());


		for (size_t i = 0; i < scripts.size(); ++i)
		{
			MonoString* monoString = mono_string_new(assetManager->m_scriptManager.m_GetDomain(), std::get<0>(scripts[i]).c_str());
			mono_array_set(scriptArray, MonoString*, i, monoString);
		}

		return scriptArray;
	}

	bool InternalCall::m_InternalAddScriptInstance([[maybe_unused]] ecs::EntityID entity, [[maybe_unused]] MonoString* monoScriptName,[[maybe_unused]] MonoObject* instance)
	{
		//ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		//auto* scriptComponent = static_cast<ecs::ScriptComponent*>(
		//	ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(entity)
		//	);

		//if (!scriptComponent)
		//{
		//	LOGGING_WARN("Failed to add script instance: No ScriptComponent found for entity: " + std::to_string(entity));
		//	return false;
		//}

		//char* nativeScriptName = mono_string_to_utf8(monoScriptName);
		//std::string scriptName = nativeScriptName;
		//mono_free(nativeScriptName);

		//auto& scriptInstances = scriptComponent->m_scriptInstances;
		//if (scriptInstances.find(scriptName) != scriptInstances.end())
		//{
		//	LOGGING_WARN("Script instance '" + scriptName + "' already exists for entity: " + std::to_string(entity) + ". Overwriting.");
		//}

		//scriptInstances[scriptName] = instance;
		//LOGGING_WARN("Script instance '" + scriptName + "' successfully added/updated for entity: " + std::to_string(entity));
		return true;
	}

	bool InternalCall::m_InternalGetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
	{
		auto* rbcomp = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entity));
		if (rbcomp) {
			*vec = rbcomp->m_Velocity;
		}
		else {
			ASSERTNOCOMPONENT(RigidBodyComponent, entity);
			return false;
		}
		

		return true;
	}

	bool InternalCall::m_InternalSetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
	{
		auto* rbcomp = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entity));
		if (rbcomp) {
			rbcomp->m_Velocity = *vec;
		}
		else {
			ASSERTNOCOMPONENT(RigidBodyComponent, entity);
		}


		return true;
	}


	float InternalCall::m_InternalCallGetDeltaTime()
	{

		return Helper::Helpers::GetInstance()->m_fixedDeltaTime;
	}

	/*
	 TIME SCALE
	*/
	void InternalCall::m_InternalCallSetTimeScale(const float x) {
		Helper::Helpers::GetInstance()->m_timeScale = x;
	}

	float InternalCall::m_InternalCallGetTimeScale()
	{
		return Helper::Helpers::GetInstance()->m_timeScale;
	}

	void InternalCall::m_InternalCallResetTimeScale() {
		Helper::Helpers::GetInstance()->m_timeScale = 1.0f;
	}


	int InternalCall::m_InternalCallGetTagID(MonoString* monostring)
	{

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		char* nativeString = mono_string_to_utf8(monostring);
		std::string tag = nativeString;
		mono_free(nativeString);

		for (const auto& scene : ecs->m_ECS_SceneMap) {
			if (scene.second.m_isPrefab == false && scene.second.m_isActive) {
				for (const auto& id : scene.second.m_sceneIDs) {
					ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
					
					if (nc->m_entityTag == tag ) {
						//check prefab and ignore if prefab
						


						return id;

						break;
					}
				}
			}
		}


		return -1;
	}

	int InternalCall::m_InternalGetEntityIdFromGridKey(int gridkey) {
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		for (const auto& scene : ecs->m_ECS_SceneMap) {
			if (scene.second.m_isPrefab == false && scene.second.m_isActive) {
				for (const auto& id : scene.second.m_sceneIDs) {
					ecs::GridComponent* gc = static_cast<ecs::GridComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(id));
					if (gc != NULL)
					{
						if (gc->m_GridKey == gridkey) {
							return gc->m_Entity;
						}
					}
				}
			}
		}
		return -1;
	}

	void InternalCall::m_ChangeLayer(ecs::EntityID id, unsigned int layerid)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
		if (nc) {
			nc->m_Layer = (layer::LAYERS)layerid;
		}

	}

	float InternalCall::m_getFPS()
	{
		return Helper::Helpers::GetInstance()->m_fps;
	}

	void InternalCall::m_InternalCallGetLightingComponent(ecs::EntityID id, vector2::Vec2* innerouterradius, vector3::Vec3* color, float* intensity)
	{
		auto* LC = static_cast<ecs::LightingComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPELIGHTINGCOMPONENT]->m_GetEntityComponent(id));

		if (LC) {
			*innerouterradius = LC->m_innerOuterRadius;
			*color = LC->m_colour;
			*intensity = LC->m_intensity;
		}
		else {
			ASSERTNOCOMPONENT(LC, id);
		}


		return;

	}

	void InternalCall::m_InternalCallSetLightingComponent(ecs::EntityID id, vector2::Vec2* innerouterradius, vector3::Vec3* color, float* intensity)
	{
		auto* LC = static_cast<ecs::LightingComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPELIGHTINGCOMPONENT]->m_GetEntityComponent(id));

		if (LC) {
			LC->m_innerOuterRadius = *innerouterradius;
			LC->m_colour = *color;
			LC->m_intensity = *intensity;
		}
		else {
			ASSERTNOCOMPONENT(LC, id);
		}


		return;
	}

	bool InternalCall::m_IsLayerVisable(int layer)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		
		return ecs->m_layersStack.m_IsLayerVisable(layer);;
	}

	void InternalCall::m_GetColliderDecomposedTRS(ecs::EntityID id, vector2::Vec2* _translate, vector2::Vec2* _rotate, float* _scale)
	{

		auto* CC = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(id));

		if (CC) {
			vector2::Vec2 translate, rotate;
			float scale;
			mat3x3::Mat3Decompose(CC->m_collider_Transformation, translate, rotate, scale);
			*_translate = translate;
			*_rotate = rotate;
			*_scale = scale;
		}
		else {
			ASSERTNOCOMPONENT(CC, id);
		}


		return;
	}

	float InternalCall::m_GetRandomFloat(float min, float max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd()); // Mersenne Twister RNG
		std::uniform_real_distribution<float> dist(min, max);
		return dist(gen);
	}

	int InternalCall::m_GetRandomInt(int min, int max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(gen);
	}

	MonoString* InternalCall::m_GetScenefromID(ecs::EntityID id)
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		std::string results{};
		if (id >= 0) {
			scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
			auto result = scenemanager->GetSceneByEntityID(id);
			if (result.has_value()) {
				results = result.value();
			}
		}



		return mono_string_new(assetmanager->m_scriptManager.m_GetDomain(), results.c_str());
	}

	void InternalCall::m_HideEntityandchildren(ecs::EntityID id)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs->m_layersStack.m_hideEntitywithChild(id);
	}

	void InternalCall::m_UnHideEntityandchildren(ecs::EntityID id)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs->m_layersStack.m_unhideEntitywithChild(id);
	}

	void InternalCall::m_StopVideo(ecs::EntityID id )
	{

		auto* VC = static_cast<ecs::VideoComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEVIDEOCOMPONENT]->m_GetEntityComponent(id));

		if (VC) {
			VC->play = false;
		}
		else {
			ASSERTNOCOMPONENT(VideoComponent, id);
		}

	}

	void InternalCall::m_PauseVideo(ecs::EntityID id , bool _boolean)
	{
		auto* VC = static_cast<ecs::VideoComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEVIDEOCOMPONENT]->m_GetEntityComponent(id));

		if (VC) {
			VC->pause = _boolean;
		}
		else {
			ASSERTNOCOMPONENT(VideoComponent, id);
		}


	}

	void InternalCall::m_StartVideo(ecs::EntityID id )
	{
		auto* VC = static_cast<ecs::VideoComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEVIDEOCOMPONENT]->m_GetEntityComponent(id));

		if (VC) {
			VC->play = true;
		}
		else {
			ASSERTNOCOMPONENT(VideoComponent, id);
		}
	}

	bool InternalCall::m_HasVideoFinish(ecs::EntityID id)
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		auto& videomap = assetmanager->m_videoManager.m_videoMap;
		auto it = videomap.find(id);
		if (it != videomap.end()) {
			return it->second->HasStopped();
		}

		return false;

	}

	MonoArray* InternalCall::m_InternalCallGetTagIDs(MonoString* monostring)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		char* nativeString = mono_string_to_utf8(monostring);
		std::string tag = nativeString;
		mono_free(nativeString);

		std::vector<int> tagIDs;

		for (const auto& scene : ecs->m_ECS_SceneMap) {
			if (scene.second.m_isPrefab == false && scene.second.m_isActive) {
				for (const auto& id : scene.second.m_sceneIDs) {
					ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
					if (nc->m_entityTag == tag) {
						tagIDs.push_back(id);
					}
				}
			}
		}

		if (tagIDs.size() > 0) {
			MonoArray* Array = mono_array_new(assetmanager::AssetManager::m_funcGetInstance()->m_scriptManager.m_GetDomain(), mono_get_int32_class(), tagIDs.size());
			for (size_t i = 0; i < tagIDs.size(); ++i) {
				mono_array_set(Array, int, i, tagIDs[i]);
			}

			return Array;
		}


		return nullptr;
	}

	MonoArray* InternalCall::m_InternalCallGetCollidedEntities(ecs::EntityID entity)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entity));
		if (cc)
		{
			if (cc->m_isCollided) {
				assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
				MonoArray* Array = mono_array_new(assetmanager->m_scriptManager.m_GetDomain(), mono_get_int32_class(), cc->m_collidedWith.size());
				for (size_t i = 0; i < cc->m_collidedWith.size(); ++i) {
					mono_array_set(Array, int, i, cc->m_collidedWith[i]);
				}

				return Array;
			}

		}
		else {
			ASSERTNOCOMPONENT(ColliderComponent, entity);
		}
		

		return nullptr;
	}

	MonoString* InternalCall::m_InternalCallGetTag(ecs::EntityID entity)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entity));
		if (!nc)
		{
			ASSERTNOCOMPONENT(ColliderComponent, entity);
			return nullptr;
		}

		return mono_string_new(assetmanager->m_scriptManager.m_GetDomain(), nc->m_entityTag.c_str());

	}

	void InternalCall::m_InternalCallSetSceneActive(MonoString* monoString)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		//scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();

		char* nativeString = mono_string_to_utf8(monoString);

		//set all scene to inactive
		for (auto& scene : ecs->m_ECS_SceneMap) {
			if (scene.second.m_isPrefab == false)
				scene.second.m_isActive = false;

		}

		const auto& scene = ecs->m_ECS_SceneMap.find(nativeString);

		if (scene != ecs->m_ECS_SceneMap.end()) {
			scene->second.m_isActive = true;
		}


		mono_free(nativeString);
	}

	void InternalCall::m_UnloadAllScene()
	{
		scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();

		scenemanager->m_ClearAllScene();

	}

	void InternalCall::m_InternalCallLoadScene(MonoString* monoString)
	{
		scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();

		char* nativeString = mono_string_to_utf8(monoString);
		std::string scenefile = std::string{ nativeString } + ".json";

		const auto& scene = scenemanager->m_unloadScenePath.find(scenefile);
		if (scene != scenemanager->m_unloadScenePath.end()) {

			scenemanager->m_LoadScene(scene->second);
		}

		mono_free(nativeString);

		throw 1;//stop the loop in the logic system
	}

	int InternalCall::m_InternalCallAddPrefab(MonoString* prefab, const float* x, const float* y, const float* rotation)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		//scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();	

		char* nativeString = mono_string_to_utf8(prefab);
		std::string prefabfile = std::string{ nativeString } + ".prefab";

		if (ecs->m_ECS_SceneMap.find(prefabfile) != ecs->m_ECS_SceneMap.end()) {
			ecs::EntityID id = prefab::Prefab::m_CreatePrefab(prefabfile);
			ecs::TransformComponent* transCom = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
			transCom->m_position = { *x, *y };
			transCom->m_rotation = *rotation;

			mono_free(nativeString);
			return (int)id;
		}
		else {
			LOGGING_ERROR("Prefab not loaded");

			mono_free(nativeString);
			return -1;
		}
	}

	void InternalCall::m_InternalGetWorldMousePosition(vector2::Vec2* mousecord) {
		//Get mouse pos

		Helper::Helpers* helper = Helper::Helpers::GetInstance();
		vector2::Vec2 mouse_Pos = Input::InputSystem::m_getMousePosition();
		//window height width
		float width = Helper::Helpers::GetInstance()->m_windowWidth;
		float height = Helper::Helpers::GetInstance()->m_windowHeight;
		//world coordinate
		float world_Mouse_Pos_X = ((mouse_Pos.m_x - helper->m_viewportOffsetX )- (width / 2.f)) / (width / 2.f);
		float world_Mouse_Pos_Y = (std::abs((mouse_Pos.m_y - helper-> m_viewportOffsetY)) - (height / 2.f)) / (height / 2.f);
		vector2::Vec2 world_Mouse_Pos{ world_Mouse_Pos_X, world_Mouse_Pos_Y };
		//include the camera
		//scale according to camera scale
		world_Mouse_Pos.m_x *= graphicpipe::GraphicsCamera::m_currCameraScaleX;
		world_Mouse_Pos.m_y *= graphicpipe::GraphicsCamera::m_currCameraScaleY;

		//aspect ratio
		world_Mouse_Pos.m_x *= 1.f / graphicpipe::GraphicsCamera::m_aspectRatio;

		//translate 
		world_Mouse_Pos.m_x += graphicpipe::GraphicsCamera::m_currCameraMatrix[2][0];
		world_Mouse_Pos.m_y += graphicpipe::GraphicsCamera::m_currCameraMatrix[2][1];

		*mousecord = world_Mouse_Pos;

	}

	void InternalCall::m_InternalCallCloseWindow()
	{
		Helper::Helpers::GetInstance()->m_closeWindow = true;
	}

	MonoArray* InternalCall::m_InternalCallGetChildrenID(ecs::EntityID id)
	{

		const auto& childs = ecs::Hierachy::m_GetChild(id);

		if (childs.has_value()) {
			assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
			MonoArray* Array = mono_array_new(assetmanager->m_scriptManager.m_GetDomain(), mono_get_int32_class(), childs.value().size());
			for (size_t i = 0; i < childs.value().size(); ++i) {
				mono_array_set(Array, int, i, childs.value()[i]);

			}

			return Array;
		}

		return nullptr;
	}

	int InternalCall::m_InternalCallGetParentID(ecs::EntityID id)
	{

		auto result = ecs::Hierachy::m_GetParent(id);

		if (result.has_value()) {
			return result.value();
		}

		return -1;
	}

	void InternalCall::m_InternalCallPlayAudio(ecs::EntityID id, MonoString* monoString)
	{
		char* nativeString = mono_string_to_utf8(monoString);

		//check if audio component is present
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::AudioComponent* aud = static_cast<ecs::AudioComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(id));

		if (aud == nullptr) return;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		std::filesystem::path filepath = nativeString;
		const auto& it = std::find_if(aud->m_AudioFiles.begin(), aud->m_AudioFiles.end(), [filepath](const auto& audio) {return audio.m_Name == filepath.filename().stem().string(); });
		
		if (it != aud->m_AudioFiles.end()) {
			assetmanager->m_audioManager.m_PlayAudioForEntity(id, filepath.filename().stem().string(), it->m_Volume);
		}
		

		mono_free(nativeString);
	}

	void InternalCall::m_InternalCallStopAudio(ecs::EntityID id, MonoString* monoString)
	{
		char* nativeString = mono_string_to_utf8(monoString);
		std::filesystem::path filepath = nativeString;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_StopAudioForEntity(id, filepath.filename().stem().string());

		mono_free(nativeString);
	}

	void InternalCall::m_InternalCallStopAllAudio()
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_StopAllSounds();

	}

	void InternalCall::m_InternalCallPauseAudio(ecs::EntityID id, MonoString* monoString)
	{

		char* nativeString = mono_string_to_utf8(monoString);
		std::filesystem::path filepath = nativeString;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_PauseAudioForEntity(id, filepath.filename().stem().string());

		mono_free(nativeString);


	}

	void InternalCall::m_InternalCallUnPauseAudio(ecs::EntityID id, MonoString* monoString)
	{

		char* nativeString = mono_string_to_utf8(monoString);
		std::filesystem::path filepath = nativeString;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_UnpauseAudioForEntity(id, filepath.filename().stem().string());

		mono_free(nativeString);


	}

	void InternalCall::m_InternalCallPauseAllAudio()
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_PauseAllSounds();

	}

	void InternalCall::m_InternalCallUnPauseAllAudio()
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_UnpauseAllSounds();
	}

	void InternalCall::m_InternalCallSetGlobalBGMVolume(float volume)
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_SetGlobalBGMVolume(volume);
	}

	void InternalCall::m_InternalCallSetGlobalSFXVolume(float volume)
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_SetGlobalSFXVolume(volume);
	}

	float InternalCall::m_InternalCallGetGlobalBGMVolume() 
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		return assetmanager->m_audioManager.m_GlobalBGMVolume;
	}

	float InternalCall::m_InternalCallGetGlobalSFXVolume()
	{
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		return assetmanager->m_audioManager.m_GlobalSFXVolume;
	}

	bool InternalCall::m_InternalCallAudioIsPlayingForEntity(ecs::EntityID id, MonoString* monoString) 
	{
		char* nativeString = mono_string_to_utf8(monoString);
		std::filesystem::path filepath = nativeString;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		bool result = assetmanager->m_audioManager.m_IsPlayingForEntity(id, filepath.filename().stem().string());

		mono_free(nativeString);

		return result;
	}

	bool InternalCall::m_InternalCallCheckIsBGM(ecs::EntityID id, MonoString* monoString) {
		char* nativeString = mono_string_to_utf8(monoString);
		std::filesystem::path filepath = nativeString;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		bool result = assetmanager->m_audioManager.m_CheckIsBGMForEntity(id, filepath.filename().stem().string());

		mono_free(nativeString);

		return result;
	}

	bool InternalCall::m_InternalCallCheckIsSFX(ecs::EntityID id, MonoString* monoString) {
		char* nativeString = mono_string_to_utf8(monoString);
		std::filesystem::path filepath = nativeString;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		bool result = assetmanager->m_audioManager.m_CheckIsSFXForEntity(id, filepath.filename().stem().string());

		mono_free(nativeString);

		return result;

	}

	void InternalCall::m_InternalCallSetPanAudio(ecs::EntityID id, MonoString* monoString, float pan)
	{
		char* nativeString = mono_string_to_utf8(monoString);
		std::filesystem::path filepath = nativeString;

		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		assetmanager->m_audioManager.m_SetPanForEntity(id, filepath.filename().stem().string(), pan);
		//std::cout << pan << std::endl;
		mono_free(nativeString);

	}

	void InternalCall::m_InternalCallHideCursor(bool _boolean)
	{
		Input::InputSystem::m_HideCursor(_boolean);
	}

	bool InternalCall::m_InternalCallIsWindowMinimise()
	{
		return Helper::Helpers::GetInstance()->m_windowMinimise;
	}

	float InternalCall::m_getAccumulatedDeltaTime()
	{

		return Helper::Helpers::GetInstance()->m_accumulatedTime;
	}

	int InternalCall::m_InternalCallGetSteps()
	{
		return Helper::Helpers::GetInstance()->currentNumberOfSteps;
	}

	float InternalCall::m_InternalCallGetGameTime()
	{
		return Helper::Helpers::GetInstance()->m_gameRunTime;
	}

	void InternalCall::m_EnableScript(ecs::EntityID id, MonoString* monoString)
	{
		char* nativeString = mono_string_to_utf8(monoString);
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::ScriptComponent* script = static_cast<ecs::ScriptComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(id));

		if (script == NULL) return;

		const auto& results = std::find_if(script->m_scripts.begin(), script->m_scripts.end(), [&](const auto& x) {return std::get<0>(x) == std::string(nativeString);});

		if (results == script->m_scripts.end()) return;

		std::get<1>(*results) = true;

		mono_free(nativeString);
	}

	void InternalCall::m_DisableScript(ecs::EntityID id, MonoString* monoString)
	{
		char* nativeString = mono_string_to_utf8(monoString);
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::ScriptComponent* script = static_cast<ecs::ScriptComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(id));

		if (script == NULL) return;

		const auto& results = std::find_if(script->m_scripts.begin(), script->m_scripts.end(), [&](const auto& x) {return std::get<0>(x) == std::string(nativeString);});

		if (results == script->m_scripts.end()) return;

		std::get<1>(*results) = false;

		mono_free(nativeString);
	}

	MonoArray* InternalCall::m_RetrieveCollidableEntities(MonoString* scene)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		char* nativeString = mono_string_to_utf8(scene);

		auto scenes = ecs->m_ECS_SceneMap.find(nativeString);

		std::vector<ecs::EntityID> results;

		if (scenes == ecs->m_ECS_SceneMap.end()) return nullptr;
		for (auto ids : scenes->second.m_sceneIDs) {
			auto* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ids));

			if (cc != NULL) {
				if (cc->m_collisionResponse) {
					results.push_back(ids);
				}
			}
		}

		if (results.size() < 0) nullptr;

		MonoArray* Array = mono_array_new(assetmanager::AssetManager::m_funcGetInstance()->m_scriptManager.m_GetDomain(), mono_get_int32_class(), results.size());
		for (size_t i = 0; i < results.size(); ++i) {
			mono_array_set(Array, int, i, results[i]);

		}


		return Array;
	}

	bool InternalCall::m_InternalCallGetRayCast( ecs::EntityID id, MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition)
	{

		char* nativeString = mono_string_to_utf8(monoString);
		std::string string{ nativeString };
		mono_free(nativeString);

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::RaycastComponent* rc = static_cast<ecs::RaycastComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERAYCASTINGCOMPONENT]->m_GetEntityComponent(id));

		if (rc) {
			
			auto it = std::find_if(rc->m_raycast.begin(), rc->m_raycast.end(), [string](const auto& x) { return x.m_rayID == string; });
			if (it != rc->m_raycast.end()) {

				*isRaycasting = it->m_isRaycasting;
				*targetposition = it->m_targetPosition;
				*m_distance = it->m_distance;
				*targetReached = it->m_targetReached;
				*hitposition = it->m_hitposition;

			}
			else {
				return false;
			}
			

		}
		else {
			ASSERTNOCOMPONENT(RaycastComponent, id);
		}



		
		return true;
	}

	bool InternalCall::m_InternalCallSetRayCast(ecs::EntityID id, MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition)
	{
		char* nativeString = mono_string_to_utf8(monoString);
		std::string string{ nativeString };
		mono_free(nativeString);

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::RaycastComponent* rc = static_cast<ecs::RaycastComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERAYCASTINGCOMPONENT]->m_GetEntityComponent(id));

		if (rc) {

			auto it = std::find_if(rc->m_raycast.begin(), rc->m_raycast.end(), [string](const auto& x) { return x.m_rayID == string; });
			if (it != rc->m_raycast.end()) {

				it->m_isRaycasting = *isRaycasting;
				it->m_targetPosition = *targetposition;
				it->m_distance = *m_distance ;
				it->m_targetReached = *targetReached;
				it->m_hitposition = *hitposition;

			}
			else {
				ASSERTNOCOMPONENT(RaycastComponent, id);
				return false;
			}


		}





		return true;
	}

	//Particle Component

	bool InternalCall::m_InternalGetParticleComponent(ecs::EntityID entity, bool* willSpawn, int* noOfParticles, float* lifeSpan, [[maybe_unused]] vector2::Vec2* size, vector2::Vec2* velocity,
		vector2::Vec2* acceleration, vector3::Vec3* color, float* coneRotation, float* coneAngle, float* randomFactor, MonoString** imageFile, int* stripCount,
		int* frameNumber, int* layer, float* friction, int* fps, [[maybe_unused]]bool* loopAnimation)
	{
		auto* particleComponent = static_cast<ecs::ParticleComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entity));
		if (particleComponent) {
			*willSpawn = particleComponent->m_willSpawn;
			*noOfParticles = particleComponent->m_noOfParticles;
			*lifeSpan = particleComponent->m_lifeSpan;
			*velocity = particleComponent->m_velocity;
			*acceleration = particleComponent->m_acceleration;
			*color = particleComponent->m_color;
			*coneRotation = particleComponent->m_coneRotation;
			*coneAngle = particleComponent->m_coneAngle;
			*randomFactor = particleComponent->m_randomFactor;
			*imageFile = mono_string_new(mono_domain_get(), particleComponent->m_imageFile.c_str());
			*stripCount = particleComponent->m_stripCount;
			*frameNumber = particleComponent->m_frameNumber;
			*layer = particleComponent->m_layer;
			*friction = particleComponent->m_friction;
			*fps = particleComponent->m_fps;
			return true;
		}
		else
		{
			ASSERTNOCOMPONENT(ParticleComponent, entity);
			return false;
		}
	}

	bool InternalCall::m_InternalSetParticleComponent(ecs::EntityID entity, bool* willSpawn, int* noOfParticles, float* lifeSpan, [[maybe_unused]]vector2::Vec2* size, vector2::Vec2* velocity, vector2::Vec2* acceleration, vector3::Vec3* color, float* coneRotation, float* coneAngle, float* randomFactor, MonoString* imageFile, int* stripCount, int* frameNumber, int* layer, float* friction, int* fps,[[maybe_unused]] bool* loopAnimation)
	{
		auto* particleComponent = static_cast<ecs::ParticleComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entity));
		if (particleComponent) {
			particleComponent->m_willSpawn = *willSpawn;
			particleComponent->m_noOfParticles = *noOfParticles;
			particleComponent->m_lifeSpan = *lifeSpan;
			particleComponent->m_velocity = *velocity;
			particleComponent->m_acceleration = *acceleration;
			particleComponent->m_color = *color;
			particleComponent->m_coneRotation = *coneRotation;
			particleComponent->m_coneAngle = *coneAngle;
			particleComponent->m_randomFactor = *randomFactor;

			char* nativeString = mono_string_to_utf8(imageFile);
			std::string imagefile = nativeString;

			particleComponent->m_imageFile = imagefile;
			particleComponent->m_stripCount = *stripCount;
			particleComponent->m_frameNumber = *frameNumber;
			particleComponent->m_layer = *layer;
			particleComponent->m_friction = *friction;
			particleComponent->m_fps = *fps;
			return true;
		}
		else 
		{
			ASSERTNOCOMPONENT(ParticleComponent, entity);
			return false;
		}
	}

	bool InternalCall::m_InternalCallSpawnParticle(ecs::EntityID entity)
	{
		auto* particleComponent = static_cast<ecs::ParticleComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entity));
		if (particleComponent)
		{
			particleComponent->m_willSpawn = true;
			return true;
		}
		else
		{
			ASSERTNOCOMPONENT(ParticleComponent, entity);
			return false;
		}
	}

	bool InternalCall::m_InternalCallSetParticleLayer(ecs::EntityID entity, int* layer)
	{
		auto* particleComponent = static_cast<ecs::ParticleComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entity));
		if (particleComponent)
		{
			particleComponent->m_layer = *layer;
			return true;
		}
		else
		{
			ASSERTNOCOMPONENT(ParticleComponent, entity);
			return false;
		}
	}

	bool InternalCall::m_InternalCallSetParticleConeRotation(ecs::EntityID entity, float* angle)
	{
		auto* particleComponent = static_cast<ecs::ParticleComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entity));
		if (particleComponent)
		{
			particleComponent->m_coneRotation= *angle;
			return true;
		}
		else
		{
			ASSERTNOCOMPONENT(ParticleComponent, entity);
			return false;
		}
	}

	bool InternalCall::m_InternalCallDespawnParticle(ecs::EntityID entity)
	{
		auto* particleComponent = static_cast<ecs::ParticleComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEPARTICLECOMPONENT]->m_GetEntityComponent(entity));
		if (particleComponent)
		{
			particleComponent->m_willSpawn = false;
			return true;
		}
		else
		{
			ASSERTNOCOMPONENT(ParticleComponent, entity);
			return false;
		}
	}


	// Grid Component
	bool InternalCall::m_InternalGetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey) {
		auto* gridComponent = static_cast<ecs::GridComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(entity));
		if (gridComponent) {
			*anchor = gridComponent->m_Anchor;
			*gridRowLength = gridComponent->m_GridRowLength;
			*gridColumnLength = gridComponent->m_GridColumnLength;
			*setCollidable = gridComponent->m_SetCollidable;
			*gridKey = gridComponent->m_GridKey;
		}
		else {
			ASSERTNOCOMPONENT(GridComponent, entity);
			return false;
		}


		return true;
	}

	bool InternalCall::m_InternalSetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey) {
		auto* gridComponent = static_cast<ecs::GridComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(entity));
		if (gridComponent) {
			gridComponent->m_Anchor = *anchor;
			gridComponent->m_GridRowLength = *gridRowLength;
			gridComponent->m_GridColumnLength = *gridColumnLength;
			gridComponent->m_SetCollidable = *setCollidable;
			gridComponent->m_GridKey = *gridKey;
		}
		else {
			ASSERTNOCOMPONENT(GridComponent, entity);
			return false;
		}
		return true;
	}

	bool InternalCall::m_InternalCallGetPathfinding(ecs::EntityID id, vector2::Vec2* m_startpos, vector2::Vec2* m_startend, int* gridkey)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::PathfindingComponent* pfc = static_cast<ecs::PathfindingComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPATHFINDINGCOMPONENT]->m_GetEntityComponent(id));

		if (pfc) {

			*m_startpos = pfc->m_StartPos;
			*m_startend = pfc->m_TargetPos;


			*gridkey = pfc->m_GridKey;

			//auto* assetmanager = assetmanager::AssetManager::m_funcGetInstance();


			/**nodeArray_x = mono_array_new(assetmanager::AssetManager::m_funcGetInstance()->m_scriptManager.m_GetDomain(), mono_get_int32_class(), pfc->m_Path.size());
			*nodeArray_y = mono_array_new(assetmanager::AssetManager::m_funcGetInstance()->m_scriptManager.m_GetDomain(), mono_get_int32_class(), pfc->m_Path.size());

			if (nodeArray_x != NULL && nodeArray_y != NULL) {
				for (size_t i = 0; i < pfc->m_Path.size(); ++i) {
					mono_array_set(*nodeArray_x, int, i, pfc->m_Path[i].x);
					mono_array_set(*nodeArray_y, int, i, pfc->m_Path[i].y);
				}
			}*/

			


			return true;
		}
		else {
			ASSERTNOCOMPONENT(PathfindingComponent, id);
			return false;
		}



		//return false;
	}

	void InternalCall::m_InternalCallGetPath(int gridKey, int* startX, int* startY, int* targetX, int* targetY, MonoArray** nodeArray_x, MonoArray** nodeArray_y)
	{
		AStarPathfinding pathfinder;
		int entityID = InternalCall::m_InternalGetEntityIdFromGridKey(gridKey);
		auto* gridComponent = static_cast<ecs::GridComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(entityID));
		
		if (gridComponent)
		{
			std::vector<Node> path = pathfinder.FindPath(gridComponent, *startX, *startY, *targetX, *targetY);
			auto* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

			*nodeArray_x = mono_array_new(assetmanager->m_scriptManager.m_GetDomain(), mono_get_int32_class(), path.size());
			*nodeArray_y = mono_array_new(assetmanager->m_scriptManager.m_GetDomain(), mono_get_int32_class(), path.size());
			if (nodeArray_x != NULL && nodeArray_y != NULL) {
				for (size_t i = 0; i < path.size(); ++i) {
					mono_array_set(*nodeArray_x, int, i, path[i].x);
					mono_array_set(*nodeArray_y, int, i, path[i].y);
				}
			}
			return;
		}
		if (!gridComponent)
		{
			return;
		}
	}



	void InternalCall::m_InternalCallSetTargetPathfinding(ecs::EntityID id, vector2::Vec2* m_targetgridposition)
	{

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::PathfindingComponent* pfc = static_cast<ecs::PathfindingComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPATHFINDINGCOMPONENT]->m_GetEntityComponent(id));

		if (pfc) {

			pfc->m_TargetPos = *m_targetgridposition;


		}
		else {
			ASSERTNOCOMPONENT(PathfindingComponent, id);
			return ;
		}
		return;
	}

	void InternalCall::m_EnableLayer(unsigned int layer)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs->m_layersStack.m_EnableLayer((layer::LAYERS)layer);
	}

	void InternalCall::m_DisableLayer(unsigned int layer)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs->m_layersStack.m_DisableLayer((layer::LAYERS)layer);
	}

	float InternalCall::m_GetUnfixedDeltaTime()
	{
		
		 float dt = Helper::Helpers::GetInstance()->m_deltaTime;
		 return dt;
	}

	void InternalCall::m_GetNameComponent(ecs::EntityID id, void** outptr)
	{

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
		*outptr = static_cast<void*>(nc); // Assign pointer to output parameter

	}




	void InternalCall::m_InternalCallDeleteEntity(ecs::EntityID id)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		if (ecs->m_ECS_EntityMap.find(id) == ecs->m_ECS_EntityMap.end()) {
			return;
		}

		ecs->m_DeleteEntity(id);

	}

	float InternalCall::m_InternalCallIsCollided(ecs::EntityID entity)
	{

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entity));
		if (cc) {
			return cc->m_isCollided;
		}
		else {
			ASSERTNOCOMPONENT(ColliderComponent, entity);
			return false;
		}
		

	}

	void InternalCall::m_InternalGetMousePosition(vector2::Vec2* returnValue)
	{
		*returnValue = Input::InputSystem::m_getMousePosition();
	}

	bool InternalCall::m_InternalCallIsKeyPressed(keyCode key)
	{
		return Input::InputSystem::m_isKeyPressed(key);
	}

	bool InternalCall::m_InternalCallIsKeyReleased(keyCode key)
	{
		return Input::InputSystem::m_isKeyReleased(key);
	}

	bool InternalCall::m_InternalCallIsKeyTriggered(keyCode key)
	{
		return Input::InputSystem::m_isKeyTriggered(key);
	}

	bool InternalCall::m_InternalCallIsControllerTriggered(keyCode key)
	{
		return Input::InputSystem::m_isControllerButtonTriggered(key);
	}

	bool InternalCall::m_InternalCallIsControllerPress(keyCode key)
	{
		return Input::InputSystem::m_isControllerButtonPressed(key);
	}

	bool InternalCall::m_InternalCallIsControllerReleased(keyCode key)
	{
		return Input::InputSystem::m_isControllerButtonReleased(key);
	}

	float InternalCall::m_InternalCallGetRightJoyStickRotation()
	{

		return Input::InputSystem::controllerRightJoyStickRotation;
	}
	bool InternalCall::m_InternalCallIsControllerPresent()
	{
		return glfwJoystickPresent(GLFW_JOYSTICK_1);
	}

	MonoArray* InternalCall::m_InternalCallGetJoyStickAxis()
	{

		std::vector<float> controlleraxis = Input::InputSystem::m_getControllerAxiss();

		MonoArray* Array = mono_array_new(assetmanager::AssetManager::m_funcGetInstance()->m_scriptManager.m_GetDomain(), mono_get_single_class(), 4);
		for (size_t i = 0; i < 4; ++i) {
			mono_array_set(Array, float, i, controlleraxis[i]);

		}

		return Array;
	}

	void InternalCall::m_InternalCallOverideMouseWithCursor(bool _boolean)
	{
		Input::InputSystem::overwriteMousePosWithController = _boolean;
	}


	void InternalCall::m_RegisterInternalCalls()
	{
		MONO_ADD_INTERNAL_CALL(m_InternalGetTransformComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetTransformComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetTranslate);
		MONO_ADD_INTERNAL_CALL(m_InternalSetTranslate);

		MONO_ADD_INTERNAL_CALL(m_InternalGetColliderComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetColliderComponent);
		MONO_ADD_INTERNAL_CALL(m_GetColliderDecomposedTRS);

		MONO_ADD_INTERNAL_CALL(m_InternalGetEnemyComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetEnemyComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetRigidBodyComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetRigidBodyComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetTextComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetTextComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetAnimationComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetAnimationComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetSpriteComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetSpriteComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetCameraComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetCameraComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetButtonComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetButtonComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalIsButtonHovered);

		MONO_ADD_INTERNAL_CALL(m_InternalCallGetLightingComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSetLightingComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetScriptNames);
		MONO_ADD_INTERNAL_CALL(m_InternalAddScriptInstance);

		MONO_ADD_INTERNAL_CALL(m_InternalGetVelocity);
		MONO_ADD_INTERNAL_CALL(m_InternalSetVelocity);

		MONO_ADD_INTERNAL_CALL(m_InternalCallGetDeltaTime);
		MONO_ADD_INTERNAL_CALL(m_GetUnfixedDeltaTime);

		MONO_ADD_INTERNAL_CALL(m_InternalCallGetTagID);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetTagIDs);

		MONO_ADD_INTERNAL_CALL(m_InternalCallGetCollidedEntities);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetTag);

		MONO_ADD_INTERNAL_CALL(m_InternalCallSetSceneActive);

		MONO_ADD_INTERNAL_CALL(m_UnloadAllScene);
		MONO_ADD_INTERNAL_CALL(m_InternalCallLoadScene);

		MONO_ADD_INTERNAL_CALL(m_InternalCallAddPrefab);
		MONO_ADD_INTERNAL_CALL(m_InternalCallDeleteEntity);

		MONO_ADD_INTERNAL_CALL(m_InternalCallIsCollided); 

		MONO_ADD_INTERNAL_CALL(m_InternalGetMousePosition);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsKeyPressed);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsKeyTriggered);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsKeyReleased);
		MONO_ADD_INTERNAL_CALL(m_InternalGetWorldMousePosition);

		MONO_ADD_INTERNAL_CALL(m_InternalCallSetTimeScale);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetTimeScale);
		MONO_ADD_INTERNAL_CALL(m_InternalCallResetTimeScale);
		MONO_ADD_INTERNAL_CALL(m_InternalCallCloseWindow);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetChildrenID);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetParentID);


		MONO_ADD_INTERNAL_CALL(m_InternalCallAudioIsPlayingForEntity);
		MONO_ADD_INTERNAL_CALL(m_InternalCallPlayAudio);
		MONO_ADD_INTERNAL_CALL(m_InternalCallStopAudio);
		MONO_ADD_INTERNAL_CALL(m_InternalCallStopAllAudio);
		MONO_ADD_INTERNAL_CALL(m_InternalCallPauseAudio);
		MONO_ADD_INTERNAL_CALL(m_InternalCallUnPauseAudio);
		MONO_ADD_INTERNAL_CALL(m_InternalCallPauseAllAudio);
		MONO_ADD_INTERNAL_CALL(m_InternalCallUnPauseAllAudio);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSetGlobalBGMVolume);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSetGlobalSFXVolume);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetGlobalBGMVolume);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetGlobalSFXVolume);
		MONO_ADD_INTERNAL_CALL(m_InternalCallCheckIsBGM);
		MONO_ADD_INTERNAL_CALL(m_InternalCallCheckIsSFX);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSetPanAudio);



		MONO_ADD_INTERNAL_CALL(m_InternalCallIsWindowMinimise);
		MONO_ADD_INTERNAL_CALL(m_getAccumulatedDeltaTime);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetSteps);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetGameTime);
		MONO_ADD_INTERNAL_CALL(m_InternalGetGridComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetGridComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetParticleComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalSetParticleComponent);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSpawnParticle);
		MONO_ADD_INTERNAL_CALL(m_InternalCallDespawnParticle);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSetParticleLayer);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSetParticleConeRotation);


		MONO_ADD_INTERNAL_CALL(m_EnableScript);
		MONO_ADD_INTERNAL_CALL(m_DisableScript);
		MONO_ADD_INTERNAL_CALL(m_RetrieveCollidableEntities);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetRayCast);
		MONO_ADD_INTERNAL_CALL(m_InternalCallSetRayCast);

		MONO_ADD_INTERNAL_CALL(m_InternalCallGetPathfinding);

		MONO_ADD_INTERNAL_CALL(m_InternalCallGetPath);


		MONO_ADD_INTERNAL_CALL(m_EnableLayer);
		MONO_ADD_INTERNAL_CALL(m_DisableLayer);

		MONO_ADD_INTERNAL_CALL(m_InternalCallSetTargetPathfinding);

		MONO_ADD_INTERNAL_CALL(m_GetNameComponent);

		MONO_ADD_INTERNAL_CALL(m_InternalGetEntityIdFromGridKey);

		MONO_ADD_INTERNAL_CALL(m_ChangeLayer);

		MONO_ADD_INTERNAL_CALL(m_getFPS);

		MONO_ADD_INTERNAL_CALL(m_IsLayerVisable);

		MONO_ADD_INTERNAL_CALL(m_GetRandomFloat);
		MONO_ADD_INTERNAL_CALL(m_GetRandomInt);

		MONO_ADD_INTERNAL_CALL(m_GetScenefromID);

		MONO_ADD_INTERNAL_CALL(m_HideEntityandchildren);
		MONO_ADD_INTERNAL_CALL(m_UnHideEntityandchildren);

		MONO_ADD_INTERNAL_CALL(m_InternalCallIsControllerTriggered);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsControllerPress);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsControllerReleased);
		MONO_ADD_INTERNAL_CALL(m_InternalCallIsControllerPresent);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetJoyStickAxis);
		MONO_ADD_INTERNAL_CALL(m_InternalCallGetRightJoyStickRotation);
		MONO_ADD_INTERNAL_CALL(m_InternalCallOverideMouseWithCursor);
		MONO_ADD_INTERNAL_CALL(m_InternalCallHideCursor);

		MONO_ADD_INTERNAL_CALL(m_StopVideo);
		MONO_ADD_INTERNAL_CALL(m_PauseVideo);
		MONO_ADD_INTERNAL_CALL(m_StartVideo);
		MONO_ADD_INTERNAL_CALL(m_HasVideoFinish);
		///SO HELP ME THEN OVER HERE
	}
}