/********************************************************************/
/*!
\file      ActionManager.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Nov 12, 2024
\brief     This header file defines the function m_GetManagerInstance for the ActionManager class

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "ActionManager.h"

namespace actions {
	std::unique_ptr<ActionManager> ActionManager::m_instance = nullptr;

	/******************************************************************/
	/*!
		\brief     This is a function to get the instance of the action manager class
	*/
	/******************************************************************/
	actions::ActionManager* actions::ActionManager::m_GetManagerInstance() {
		if (!m_instance) {
			m_instance = std::make_unique<ActionManager>();
		}
		return m_instance.get();
	}
}