/*!
\file      EventHandler.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Oct 19, 2024
\brief     This file holds the Event handler class that holds 3 dispatchers one for each TYPE of event
		   This file also contains macros for easier subscribing/registering events along with dispatching and unsubscribing/unregistering

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "EventsMouseEvent.h"
#include "EventsKeyEvent.h"
#include "EventsButtonEvent.h"
#include "EventsAction.h"
#include <stack>
#include <memory>

#define REGISTER_BUTTON_LISTENER(eventType, func, arg) events::EventHandler::m_GetDispatchInstance()->m_buttonDispatch.m_RegisterListener(eventType, std::bind(&func, arg, std::placeholders::_1));
#define REGISTER_MOUSE_LISTENER(eventType, func, arg) events::EventHandler::m_GetDispatchInstance()->m_mouseDispatch.m_RegisterListener(eventType, std::bind(&func, arg, std::placeholders::_1));
#define REGISTER_KEY_LISTENER(eventType, func, arg) events::EventHandler::m_GetDispatchInstance()->m_keyDispatch.m_RegisterListener(eventType, std::bind(&func, arg, std::placeholders::_1));
#define REGISTER_ACTION_LISTENER(eventType, func, arg) events::EventHandler::m_GetDispatchInstance()->m_actionDispatch.m_RegisterListener(eventType, std::bind(&func, arg, std::placeholders::_1));

#define DISPATCH_BUTTON_EVENT(givenEvent) events::EventHandler::m_GetDispatchInstance()->m_buttonDispatch.m_DispatchToListeners(givenEvent)
#define DISPATCH_MOUSE_EVENT(givenEvent) events::EventHandler::m_GetDispatchInstance()->m_mouseDispatch.m_DispatchToListeners(givenEvent)
#define DISPATCH_KEY_EVENT(givenEvent) events::EventHandler::m_GetDispatchInstance()->m_keyDispatch.m_DispatchToListeners(givenEvent)
#define DISPATCH_ACTION_EVENT(givenEvent) events::EventHandler::m_GetDispatchInstance()->m_actionDispatch.m_DispatchToListeners(givenEvent)

#define REMOVE_BUTTON_LISTENER(ID) events::EventHandler::m_GetDispatchInstance()->m_buttonDispatch.m_UnregisterListener(ID)
#define REMOVE_MOUSE_LISTENER(ID) events::EventHandler::m_GetDispatchInstance()->m_mouseDispatch.m_UnregisterListener(ID)
#define REMOVE_KEY_LISTENER(ID) events::EventHandler::m_GetDispatchInstance()->m_keyDispatch.m_UnregisterListener(ID)
#define REMOVE_ACTION_LISTENER(ID) events::EventHandler::m_GetDispatchInstance()->m_actionDispatch.m_UnregisterListener(ID)
namespace events {
	class EventHandler {
	public:
		EventHandler() : m_keyDispatch(), m_mouseDispatch(),m_buttonDispatch(), m_actionDispatch(){}
		EventDispatcher<KeyEvents> m_keyDispatch;
		EventDispatcher<MouseEvents> m_mouseDispatch;
		EventDispatcher<ButtonEvents> m_buttonDispatch;
		EventDispatcher<Actions> m_actionDispatch;
		static EventHandler* m_GetDispatchInstance();

	private:
		static std::unique_ptr<EventHandler> m_instance;
	};

}