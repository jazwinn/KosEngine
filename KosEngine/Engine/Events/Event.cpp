/*!
\file      Event.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      8 March, 2025
\brief     This file holds the definitions of the subscribe and trigger 
		   function.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "Event.h"


namespace Event {

	void EventSystem::SubscribeEvent(EventType TypeEvent, EventListener listener) {

		Listeners[TypeEvent].push_back(listener);

	}

	void EventSystem::TriggerEvent(EventType TypeEvent) {

		if (Listeners.find(TypeEvent) != Listeners.end()) {
			for (auto& Listener : Listeners[TypeEvent]) {
				Listener();
			}
		}
	}


}