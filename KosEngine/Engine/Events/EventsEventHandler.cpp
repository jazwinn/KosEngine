/*!
\file      EventHandler.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Oct 19, 2024
\brief     This file holds the function definition for getting the instance of event handler

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "EventsEventHandler.h"

namespace events {
	std::unique_ptr<EventHandler> EventHandler::m_instance = nullptr;

	/******************************************************************/
	/*!
		\brief     This is a function to get the instance of EventHandler
	*/
	/******************************************************************/
	EventHandler* EventHandler::m_GetDispatchInstance() {
		if (!m_instance) {
			m_instance = std::make_unique<EventHandler>();
		}
		return m_instance.get();
	}
}