/******************************************************************/
/*!
\file      MessagingSystem.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     The main functions of the messaging system (Adding a callbacks to an event type and to send messages.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "MessagingMessageSystem.h"

namespace messaging {

	void MessageSystem::m_AddListener(MessageType listenToType, MessageCallback callback) {
		m_listeners[listenToType].emplace_back(callback);
	}


	void MessageSystem::m_SendMessage(const Message& givenMsg) {
		auto iter = m_listeners.find(givenMsg.m_GetType());
		if (iter != m_listeners.end()) {
			for (auto& callback : m_listeners[givenMsg.m_GetType()]) {
				callback(givenMsg);
			}
		}
	}
}