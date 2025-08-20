/******************************************************************/
/*!
\file      Performance.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Implements functions to track and report system performance,
		   including total system time, individual system time,
		   and frames per second (FPS).

This file provides an interface for tracking performance metrics
related to various systems in the engine (e.g., movement, collision,
rendering) and allows for time measurements and performance logging.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "Performance.h"
#include "../Debugging/Logging.h"

namespace performancetracker {
	//Global Varaibles
	float Performance::m_engineTime = 0.f;
	int Performance::m_totalSystem = ecs::TOTALTYPESYSTEM;
	std::unordered_map<ecs::TypeSystem, std::pair<std::string, float>> Performance::m_systemTimeList;

	Performance::Performance(){
		//m_engineTime = 0.f;
	}
	/******************************************************************/
	/*!
	\fn        std::string Performance::m_typeToString(ecs::TypeSystem type)
	\brief     Converts a system type enum to a string representation.
	\param[in] type  The system type to be converted.
	\return    The string representation of the system type.
	*/
	/******************************************************************/
	std::string Performance::m_typeToString(ecs::TypeSystem type) {
		switch (type) {
		case ecs::TypeSystem::TYPECOLLISIONSYSTEM:
			return "Collision System";
		case ecs::TypeSystem::TYPECOLLISIONRESPONSESYSTEM:
			return "Collision Response System";
		case ecs::TypeSystem::TYPEDEBUGDRAWINGSYSTEM:
			return "Render Debug System";
		case ecs::TypeSystem::TYPERENDERTEXTSYSTEM:
			return "Render Text System";
		case ecs::TypeSystem::TYPERENDERSYSTEM:
			return "Render System";
		case ecs::TypeSystem::TYPEPHYSICSSYSTEM:
			return "Physics System";
		case ecs::TypeSystem::TYPELOGICSYSTEM:
			return "Logic System";
		case ecs::TypeSystem::TYPEBUTTONSYSTEM:
			return "Button System";
		case ecs::TypeSystem::TYPEANIMATIONSYSTEM:
			return "Animation System";
		case ecs::TypeSystem::TYPECAMERASYSTEM:
			return "Camera System";
		case ecs::TypeSystem::TYPETRANSFORMSYSTEM:
			return "Transform System";
		case ecs::TypeSystem::TYPETILEMAPSYSTEM:
			return "Tile Map System";
		case ecs::TypeSystem::TYPEAUDIOSYSTEM:
			return "Audio System";
		case ecs::TypeSystem::TYPEGRIDSYSTEM:
			return "Grid System";
		case ecs::TypeSystem::TYPEPATHFINDINGSYSTEM:
			return "Pathfinding System";
		case ecs::TypeSystem::TYPERAYCASTSYSTEM:
			return "Raycast System";
		case ecs::TypeSystem::TYPELIGHTINGSYSTEM:
			return "Lighting System";
		case ecs::TypeSystem::TYPEVIDEOSYSTEM:
			return "Video System";
		case ecs::TypeSystem::TOTALTYPESYSTEM:
			return "ImGui System";
		default:
			return "Error (Add System to Performance.cpp)";
		}
	}
	/******************************************************************/
	/*!
	\fn        void Performance::m_ResetTotalSystemTime()
	\brief     Resets the total engine time to zero. This is used to
			   reinitialize the performance tracking process.
	*/
	/******************************************************************/
	void Performance::m_ResetTotalSystemTime() {
		m_engineTime = 0;
	}

	/******************************************************************/
	/*!
	\fn        void Performance::m_UpdateTotalSystemTime(float time)
	\brief     Updates the total engine time by adding the provided time.
	\param[in] time  The time to be added to the total system time.
	*/
	/******************************************************************/
	void Performance::m_UpdateTotalSystemTime(float time) {
		m_engineTime += time;
	}
	/******************************************************************/
	/*!
	\fn        void Performance::m_AddSystem(ecs::TypeSystem System)
	\brief     Adds a system to the performance tracker and initializes
			   its time to zero.
	\param[in] System  The system to be added to the tracker.
	*/
	/******************************************************************/
	void Performance::m_AddSystem(ecs::TypeSystem System) {
		m_systemTimeList[System] = { m_typeToString(System),0.f};
	}
	/******************************************************************/
	/*!
	\fn        void Performance::m_UpdateSystemTime(ecs::TypeSystem system, float time)
	\brief     Updates the tracked time for the specified system.
	\param[in] system  The system whose time will be updated.
	\param[in] time    The time to be set for the system.
	*/
	/******************************************************************/
	void Performance::m_UpdateSystemTime(ecs::TypeSystem system, float time) {
		m_systemTimeList[system].second = time;
	}
	/******************************************************************/
	/*!
	\fn        float Performance::m_GetTotalSystemTime()
	\brief     Retrieves the total time tracked for all systems.
	\return    The total engine time.
	*/
	/******************************************************************/
	float Performance::m_GetTotalSystemTime() {
		return m_engineTime;
	}
	/******************************************************************/
	/*!
	\fn        float Performance::m_GetSystemTime(ecs::TypeSystem sys)
	\brief     Retrieves the time tracked for a specific system.
	\param[in] sys  The system whose time is being queried.
	\return    The time tracked for the system.
	*/
	/******************************************************************/
	float Performance::m_GetSystemTime(ecs::TypeSystem sys) {
		return m_systemTimeList[sys].second;
	}
	/******************************************************************/
	/*!
	\fn        std::string Performance::m_GetSystemString(ecs::TypeSystem Sys)
	\brief     Retrieves the string representation of a specific system type.
	\param[in] Sys  The system whose string representation is being queried.
	\return    The string representation of the system.
	*/
	/******************************************************************/
	std::string Performance::m_GetSystemString(ecs::TypeSystem Sys) {
		return m_systemTimeList[Sys].first;
	}
	/******************************************************************/
	/*!
	\fn        void Performance::m_PrintPerformance()
	\brief     Prints the performance metrics for all systems, including the
			   time taken by each system and the percentage of total engine time.
	\warning   This function is currently commented out and not in use.
	*/
	/******************************************************************/
	void Performance::m_PrintPerformance(){
		//std::cout << "###########################################" << std::endl;
		//for (auto& val : systemTimeList) {
		//	std::string system = typeToString(val.first);
		//	float systemTime = (val.second / engineTime) * 100;
		//	std::cout << "Time taken for " << system << ": " << val.second << std::endl;
		//	std::cout << "Percentage overall: " << systemTime << std::endl;
		//}
		//float avgTime = engineTime / totalSystem;
		//std::cout << "Average time:" << avgTime << std::endl;
		//std::cout << "###########################################" << std::endl;
	}
	/******************************************************************/
	/*!
	\fn        void Performance::m_PrintFPS(float dt)
	\brief     Prints the current frames per second (FPS) based on the provided
			   delta time.
	\param[in] dt  The delta time, used to calculate FPS.
	*/
	/******************************************************************/
	void Performance::m_PrintFPS(float dt) {
		LOGGING_INFO("FPS: %.2f", 1.0f / dt);
	}

}