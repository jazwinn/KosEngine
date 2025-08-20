/******************************************************************/
/*!
\file      ECS.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the Entity Component
		   System. It acts as a factory to add and remove entities.
		   It also assignment entities to component datas.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "ECS.h"
#include "../Debugging/Performance.h"
#include "../Debugging/Logging.h"
#include "../Asset Manager/SceneManager.h"
#include "Hierachy.h"
#include "../Helper/Helper.h"

//ECS Varaible

namespace ecs{

	std::unique_ptr<ECS> ECS::m_InstancePtr = nullptr;



	void ECS::m_Init() {
		ECS* ecs = ECS::m_GetInstance();
		//loops through all the system
		for (auto& System : ecs->m_ECS_SystemMap) {
			System.second->m_Init();
		}


	}

	void ECS::m_Load() {

		ECS* ecs = ECS::m_GetInstance();

		//Allocate memory to each component pool
		m_AddComponentToECS<NameComponent>(TYPENAMECOMPONENT);
		m_AddComponentToECS<TransformComponent>(TYPETRANSFORMCOMPONENT);
		m_AddComponentToECS<SpriteComponent>(TYPESPRITECOMPONENT);
		m_AddComponentToECS<UISpriteComponent>(TYPEUISPRITECOMPONENT);
		m_AddComponentToECS<ColliderComponent>(TYPECOLLIDERCOMPONENT);
		m_AddComponentToECS<RigidBodyComponent>(TYPERIGIDBODYCOMPONENT);
		m_AddComponentToECS<EnemyComponent>(TYPEENEMYCOMPONENT);
		m_AddComponentToECS<TextComponent>(TYPETEXTCOMPONENT);
		m_AddComponentToECS<AnimationComponent>(TYPEANIMATIONCOMPONENT);
		m_AddComponentToECS<CameraComponent>(TYPECAMERACOMPONENT);
		m_AddComponentToECS<ScriptComponent>(TYPESCRIPTCOMPONENT);
		m_AddComponentToECS<ButtonComponent>(TYPEBUTTONCOMPONENT);
		m_AddComponentToECS<TilemapComponent>(TYPETILEMAPCOMPONENT);
		m_AddComponentToECS<AudioComponent>(TYPEAUDIOCOMPONENT);
		m_AddComponentToECS<LightingComponent>(TYPELIGHTINGCOMPONENT);
		m_AddComponentToECS<GridComponent>(TYPEGRIDCOMPONENT);
		m_AddComponentToECS<RaycastComponent>(TYPERAYCASTINGCOMPONENT);
		m_AddComponentToECS<PathfindingComponent>(TYPEPATHFINDINGCOMPONENT);
		m_AddComponentToECS<ParticleComponent>(TYPEPARTICLECOMPONENT);
		m_AddComponentToECS<VideoComponent>(TYPEVIDEOCOMPONENT);



		//Allocate memory to each system
		ecs->m_ECS_SystemMap[TYPETRANSFORMSYSTEM] = std::make_shared<TransformSystem>();
		ecs->m_ECS_SystemMap[TYPECOLLISIONSYSTEM] = std::make_shared<CollisionSystem>();
		ecs->m_ECS_SystemMap[TYPEPHYSICSSYSTEM] = std::make_shared<PhysicsSystem>(); 
		ecs->m_ECS_SystemMap[TYPECOLLISIONRESPONSESYSTEM] = std::make_shared<CollisionResponseSystem>();
		ecs->m_ECS_SystemMap[TYPELOGICSYSTEM] = std::make_shared<LogicSystem>();
		ecs->m_ECS_SystemMap[TYPEBUTTONSYSTEM] = std::make_shared<ButtonSystem>();

		ecs->m_ECS_SystemMap[TYPERENDERSYSTEM] = std::make_shared<RenderSystem>();
		ecs->m_ECS_SystemMap[TYPEUIRENDERSYSTEM] = std::make_shared<UIRenderSystem>();
		ecs->m_ECS_SystemMap[TYPERENDERTEXTSYSTEM] = std::make_shared<RenderTextSystem>();
		ecs->m_ECS_SystemMap[TYPEDEBUGDRAWINGSYSTEM] = std::make_shared<DebugDrawingSystem>();
		ecs->m_ECS_SystemMap[TYPEAUDIOSYSTEM] = std::make_shared<AudioSystem>();
		ecs->m_ECS_SystemMap[TYPECAMERASYSTEM] = std::make_shared<CameraSystem>();
		ecs->m_ECS_SystemMap[TYPEANIMATIONSYSTEM] = std::make_shared<AnimationSystem>();
		ecs->m_ECS_SystemMap[TYPETILEMAPSYSTEM] = std::make_shared<TilemapSystem>();
		ecs->m_ECS_SystemMap[TYPELIGHTINGSYSTEM] = std::make_shared<LightingSystem>();


		ecs->m_ECS_SystemMap[TYPEGRIDSYSTEM] = std::make_shared<GridSystem>();
		ecs->m_ECS_SystemMap[TYPERAYCASTSYSTEM] = std::make_shared<RayCastSystem>();
		ecs->m_ECS_SystemMap[TYPEPATHFINDINGSYSTEM] = std::make_shared<PathfindingSystem>();
		ecs->m_ECS_SystemMap[TYPEPARTICLESYSTEM] = std::make_shared<ParticleSystem>();

		ecs->m_ECS_SystemMap[TYPEVIDEOSYSTEM] = std::make_shared<VideoSystem>();


		

		//Initialize all system Peformance
		performancetracker::Performance Perform{};
		for (int n{}; n <= TOTALTYPESYSTEM; n++) {
			Perform.m_AddSystem((TypeSystem)n);
		}
	}


	void ECS::m_Update(float DT) {

		ECS* ecs = ECS::m_GetInstance();
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		//update deltatime
		ecs->m_DeltaTime = DT;

		performancetracker::Performance::m_ResetTotalSystemTime();
		
		//check for gamestate
		if (ecs->m_state != ecs->m_nextState) {
			if (ecs->m_nextState == START) {
				//reset game time
				help->m_gameRunTime = 0.f;

				//if next state is start, run the logic start script
				help->currentNumberOfSteps = 0;
				help->m_timeScale = 1;
				std::shared_ptr<LogicSystem> sys = std::dynamic_pointer_cast<LogicSystem>(ecs->m_ECS_SystemMap.find(TYPELOGICSYSTEM)->second);
				sys->m_StartLogic(); //TODO? place scene?

				ecs->m_state = RUNNING;
				ecs->m_nextState = RUNNING;
			}
			else {
				ecs->m_state = ecs->m_nextState;
			}

				
		}

		if (ecs->m_state == RUNNING) {
			help->m_gameRunTime += help->m_deltaTime;
		}



		//loops through all the system
		for (auto& System : ecs->m_ECS_SystemMap) {
			
			



			if (ecs->m_state != RUNNING) {
				if (System.first == TYPECOLLISIONRESPONSESYSTEM || System.first == TYPERAYCASTSYSTEM ||
					System.first == TYPELOGICSYSTEM || System.first == TYPEPHYSICSSYSTEM || 
					System.first == TYPEANIMATIONSYSTEM || System.first == TYPEPATHFINDINGSYSTEM) {
					//skip physics and logic if not running
					continue;
				}
			}

			//switch (ecs->m_state) {

			//case STOP:
			//	break;
			//case START:
			//	break;
			//case RUNNING:
			//	break;
			//case WAIT:
			//	break;
			//case TERMINATE:
			//	break;

			//}

			std::chrono::duration<float> duration{};


			//iterate through all the scenes, check if true or false
			std::vector<decltype(ecs->m_ECS_SceneMap)::key_type> keys;
			for (const auto& scene : ecs->m_ECS_SceneMap) {
				keys.push_back(scene.first);
			}

			for (const auto& key : keys) {
				auto it = ecs->m_ECS_SceneMap.find(key);
				if (it != ecs->m_ECS_SceneMap.end() && it->second.m_isActive) {
					auto start = std::chrono::steady_clock::now();
					System.second->m_Update(key);
					auto end = std::chrono::steady_clock::now();
					duration = end - start;
				}
			}

			performancetracker::Performance::m_UpdateTotalSystemTime(duration.count());
			performancetracker::Performance::m_UpdateSystemTime(System.first, duration.count());
		

		}
		
	}

	void ECS::m_Unload() {



		//delete ecs;
	}

	template<typename T>
	void ECS::m_AddComponentToECS(ComponentType type)
	{
		ECS* ecs = ECS::m_GetInstance();
		ecs->m_ECS_CombinedComponentPool[type] = std::make_shared<ComponentPool<T>>();

		//ecs->m_callFunctionToComponent_Map[type] = [](void* component, void (*func)(void*)) { T* typeComponent = static_cast<T*>(component); typeComponent->ApplyFunction(func); }; // TODO in future
		//rbc->ApplyFunction(DrawComponents(rbc->Names()));
	}

	void* ECS::m_AddComponent(ComponentType Type, EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();

		//checks if component already exist
		if (ecs->m_ECS_EntityMap[ID].test(Type)) {
			LOGGING_WARN("Entity Already Has Component");
			return NULL;
		}

		void* ComponentPtr = ecs->m_ECS_CombinedComponentPool[Type]->m_AssignComponent(ID);

		//assign component to a scene
		//check for entity scene
		const auto& scene = scenes::SceneManager::GetSceneByEntityID(ID);
		if (scene.has_value()) {
			static_cast<Component*>(ComponentPtr)->m_scene = scene.value();
		}
		else {
			LOGGING_ASSERT_WITH_MSG("Entity not assigned scene");
		}

		ecs->m_ECS_EntityMap.find(ID)->second.set(Type);

		//checks if new component fufils any of the system requirements
		m_RegisterSystems(ID);

		return ComponentPtr;
	}

	bool ECS::m_RemoveComponent(ComponentType Type, EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();

		//checks if component already exist
		if (!ecs->m_ECS_EntityMap[ID].test(Type)) {
			LOGGING_WARN("Entity Component has already been removed");
			return false;
		}

		ecs->m_ECS_CombinedComponentPool[Type]->m_DeleteEntityComponent(ID);

		//deregister everthing
		m_DeregisterSystem(ID);

		ecs->m_ECS_EntityMap.find(ID)->second.reset(Type);

		//register everything
		m_RegisterSystems(ID);

		return true;
	}

	void ECS::m_RegisterSystems(EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();
		for (auto& system : ecs->m_ECS_SystemMap) {
			if ((ecs->m_ECS_EntityMap.find(ID)->second & system.second->m_SystemSignature) == system.second->m_SystemSignature) {

				system.second->m_RegisterSystem(ID);

			}
		}
	}

	void ECS::m_DeregisterSystem(EntityID ID) {

		ECS* ecs = ECS::m_GetInstance();
		for (auto& system : ecs->m_ECS_SystemMap) {
			if ((ecs->m_ECS_EntityMap.find(ID)->second & system.second->m_SystemSignature) == system.second->m_SystemSignature) {

				system.second->m_DeregisterSystem(ID);

			}
		}

	}

	EntityID ECS::m_CreateEntity(std::string scene) {

		ECS* ecs = ECS::m_GetInstance();

		//TODO change to sth other than assert
		//if (ecs->m_EntityCount >= (MaxEntity - 1)) {// -1 so as to keep all the last component pool as default to reset
		//	LOGGING_ASSERT_WITH_MSG("Max Entity Count has been reached");
		//}
		//assert(ecs->m_EntityCount < MaxEntity, "Max Entity Count has been reached");

		EntityID ID = ecs->m_EntityCount;

		// set bitflag to 0
		ecs->m_ECS_EntityMap[ID] = 0;


		ecs->m_EntityCount++;


		//assign entity to default layer
		ecs->m_layersStack.m_layerMap[layer::DEFAULT].second.push_back(ID);

		//assign entity to scenes
		ecs->m_ECS_SceneMap.find(scene)->second.m_sceneIDs.push_back(ID);

		//add transform component and name component as default
		m_AddComponent(TYPENAMECOMPONENT, ID);
		m_AddComponent(TYPETRANSFORMCOMPONENT, ID);

		return ID;
	}

	EntityID ECS::m_DuplicateEntity(EntityID DuplicatesID, std::string scene) {
		ECS* ecs = ECS::m_GetInstance();


		if (scene.empty()) {
			const auto& result = scenes::SceneManager::GetSceneByEntityID(DuplicatesID);
			if (!result.has_value()) {
				LOGGING_ASSERT_WITH_MSG("Scene does not exits");
			}
			scene = result.value();
		}
		else {
			if (ecs->m_ECS_SceneMap.find(scene) == ecs->m_ECS_SceneMap.end()) {
				LOGGING_ASSERT_WITH_MSG("Scene does not exits");
			}
		}


		EntityID NewEntity = ecs->m_CreateEntity(scene);

		compSignature DuplicateSignature = ecs->m_ECS_EntityMap.find(DuplicatesID)->second;

		for (size_t n{}; n < TOTALTYPECOMPONENT; n++) {

			if (DuplicateSignature.test((ComponentType)n)) {

				Component* comp = static_cast<Component*>(ecs->m_ECS_CombinedComponentPool[(ComponentType)n]->m_DuplicateComponent(DuplicatesID, NewEntity));
				comp->m_scene = scene;

			}

		}

		ecs->m_ECS_EntityMap.find(NewEntity)->second = DuplicateSignature;
		m_RegisterSystems(NewEntity);

		//checks if duplicates entity has parent and assign it
		if (Hierachy::m_GetParent(DuplicatesID).has_value()) {
			TransformComponent* transform = (TransformComponent*)(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(Hierachy::m_GetParent(DuplicatesID).value()));
			transform->m_childID.push_back(NewEntity);
		}

		//checks if entity has child call recursion
		if (Hierachy::m_GetChild(DuplicatesID).has_value()) {
			//clear child id of vector for new entity
			TransformComponent* transform = (TransformComponent*)(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(NewEntity));
			transform->m_childID.clear();

			std::vector<EntityID> childID = Hierachy::m_GetChild(DuplicatesID).value();
			for (const auto& child : childID) {
				EntityID dupChild = m_DuplicateEntity(child, scene);
				Hierachy::m_SetParent(NewEntity, dupChild);
			}
		}

		return NewEntity;

	}

	bool ECS::m_DeleteEntity(EntityID ID) {

		

		ECS* ecs = ECS::m_GetInstance();
		//check if id is a thing
		if (ecs->m_ECS_EntityMap.find(ID) == ecs->m_ECS_EntityMap.end()) {
			LOGGING_ERROR("Entity Does Not Exist");
			return false;
		}

		//Hierachy::m_RemoveParent(ID);

		if (Hierachy::m_GetParent(ID).has_value()) {
			EntityID parent = Hierachy::m_GetParent(ID).value();
			// if parent id is deleted, no need to remove its child
			if (ecs->m_ECS_EntityMap.find(parent) != ecs->m_ECS_EntityMap.end()) {
				TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
				size_t pos{};
				for (EntityID& id : parentTransform->m_childID) {
					if (ID == id) {
						parentTransform->m_childID.erase(parentTransform->m_childID.begin() + pos);
						break;
					}
					pos++;
				}
			}
		}



		// refector
		m_DeregisterSystem(ID);



		// remove entity from scene
		const auto& result = scenes::SceneManager::GetSceneByEntityID(ID);
		auto& entityList = ecs->m_ECS_SceneMap.find(result.value())->second.m_sceneIDs;
		auto it = std::find(entityList.begin(), entityList.end(), ID);
		ecs->m_ECS_SceneMap.find(result.value())->second.m_sceneIDs.erase(it);





		//get child
		if (Hierachy::m_GetChild(ID).has_value()) {
			std::vector<EntityID> childs = Hierachy::m_GetChild(ID).value();
			for (auto& x : childs) {
				m_DeleteEntity(x);
			}
		}

		// reset all components
		for (size_t n{}; n < TOTALTYPECOMPONENT; n++) {
			if (ecs->m_ECS_EntityMap.find(ID)->second.test(n)) {
				ecs->m_ECS_CombinedComponentPool[(ComponentType)n]->m_DeleteEntityComponent(ID);
			}
		}

		//store delete entity
		m_deletedentity.push_back(std::make_pair(ID, ecs->m_ECS_EntityMap.find(ID)->second));
		ecs->m_ECS_EntityMap.erase(ID);


		return true;
	}

	bool ECS::m_RestoreEntity(EntityID id)
	{
		ECS* ecs = ECS::m_GetInstance();

		const auto& it = std::find_if(m_deletedentity.begin(), m_deletedentity.end(), [id](const auto& pair) {return pair.first == id;});
		if (it == m_deletedentity.end()) return false; //return if entiy id has not been deleted

		//get entity scene
		auto* tc = static_cast<TransformComponent*>(m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
		const auto& scene = tc->m_scene;

		//see if scene still exist in the engine
		const auto& sceneit = ecs->m_ECS_SceneMap.find(scene);
		if (sceneit == ecs->m_ECS_SceneMap.end()) return false;// scene no longer loaded
		sceneit->second.m_sceneIDs.push_back(id);


		// if id has parent, assign it back to the parent
		if (Hierachy::m_GetParent(id).has_value()) {
			Hierachy::m_SetParent(Hierachy::m_GetParent(id).value(), id);

		}

		//add entity back to entitymap
		ecs->m_ECS_EntityMap[id] = it->second;




		m_RegisterSystems(id);


		


		//restore all child
		if (Hierachy::m_GetChild(id).has_value()) {
			std::vector<EntityID> childs = Hierachy::m_GetChild(id).value();
			for (auto& x : childs) {
				m_RestoreEntity(x);
			}
		}



		//remove entity from vector
		m_deletedentity.erase(it);

		return false;
	}

	


	

}



