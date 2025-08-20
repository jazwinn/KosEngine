/******************************************************************/
/*!
\file      Performance.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Defines the Performance class for tracking and reporting
		   system performance, including total engine time and individual
		   system times.

The header provides declarations for the Performance class functions that
handle performance tracking and output for various engine systems such as
movement, rendering, and collision.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef PERFORMANCE_H
#define PERFORMANCE_H
#include "../Config/pch.h"
#include <../ECS/System/SystemType.h>

namespace performancetracker{
	/******************************************************************/
	/*!
	\class     Performance
	\brief     The Performance class tracks and reports the time spent on
			   different systems in the engine and the total time across
			   all systems. It also provides FPS tracking.
	*/
	/******************************************************************/
	class Performance {
	public:
		Performance();
		/******************************************************************/
		/*!
		\fn        static void Performance::m_ResetTotalSystemTime()
		\brief     Resets the total time spent on all systems to zero.
		*/
		/******************************************************************/
		static void m_ResetTotalSystemTime();
		/******************************************************************/
		/*!
		\fn        static void Performance::m_AddSystem(ecs::TypeSystem System)
		\brief     Adds a system to the performance tracker with an initial time of zero.
		\param[in] System  The system to be tracked.
		*/
		/******************************************************************/
		static void m_AddSystem(ecs::TypeSystem System);
		/******************************************************************/
		/*!
		\fn        static void Performance::m_PrintPerformance()
		\brief     Prints the performance data for all tracked systems.
				   This includes time spent on each system and their percentage
				   contribution to total engine time.
		*/
		/******************************************************************/
		static void m_PrintPerformance();	
		/******************************************************************/
		/*!
		\fn        static void Performance::m_PrintFPS(float dt)
		\brief     Prints the frames per second (FPS) based on the provided delta time.
		\param[in] dt  The delta time used to calculate FPS.
		*/
		/******************************************************************/
		static void m_PrintFPS(float);
		/******************************************************************/
		/*!
		\fn        static void Performance::m_UpdateTotalSystemTime(float time)
		\brief     Adds the specified time to the total system time.

		\param[in] time  The time to be added.
		*/
		/******************************************************************/
		static void m_UpdateTotalSystemTime(float);
		/******************************************************************/
		/*!
		\fn        static float Performance::m_GetTotalSystemTime()
		\brief     Retrieves the total time spent across all systems.
		\return    The total system time.
		*/
		/******************************************************************/
		static float m_GetTotalSystemTime();
		/******************************************************************/
		/*!
		\fn        static float Performance::m_GetSystemTime(ecs::TypeSystem sys)
		\brief     Retrieves the time tracked for a specific system.
		\param[in] sys  The system whose tracked time is being queried.
		\return    The time spent on the specified system.
		*/
		/******************************************************************/
		static float m_GetSystemTime(ecs::TypeSystem);
		/******************************************************************/
		/*!
		\fn        static std::string Performance::m_GetSystemString(ecs::TypeSystem Sys)
		\brief     Retrieves the string representation of a system type.
		\param[in] Sys  The system type to be converted.
		\return    The string representation of the system type.
		*/
		/******************************************************************/
		static std::string m_GetSystemString(ecs::TypeSystem);
		/******************************************************************/
		/*!
		\fn        static void Performance::m_UpdateSystemTime(ecs::TypeSystem system, float time)
		\brief     Updates the tracked time for a specific system.
		\param[in] system  The system whose tracked time will be updated.
		\param[in] time    The time to be set for the system.
		*/
		/******************************************************************/
		static void m_UpdateSystemTime(ecs::TypeSystem, float);
	private:

		/******************************************************************/
		/*!
		\fn      static std::string Performance::m_typeToString(ecs::TypeSystem)
		\brief   Converts a system type to its string representation.
		\param   type - The system type to be converted.
		\return  The string representation of the system type.
		*/
		/******************************************************************/
		static std::string m_typeToString(ecs::TypeSystem);

		/******************************************************************/
		/*!
		\var     static float Performance::m_engineTime
		\brief   Total time spent across all engine systems.
		\details Tracks the accumulated time for all systems in the engine.
		*/
		/******************************************************************/
		static float m_engineTime;

		/******************************************************************/
		/*!
		\var     static int Performance::m_totalSystem
		\brief   Number of systems currently being tracked.
		\details Used to determine the total count of different systems tracked by the performance tracker.
		*/
		/******************************************************************/
		static int m_totalSystem;

		/******************************************************************/
		/*!
		\var     static std::unordered_map<ecs::TypeSystem, std::pair<std::string, float>> Performance::m_systemTimeList
		\brief   Maps system types to their respective names and tracked times.
		\details Tracks the name and time spent on each system, providing detailed performance data.
		*/
		/******************************************************************/
		static std::unordered_map<ecs::TypeSystem, std::pair<std::string, float>> m_systemTimeList;


	};
}

#endif
