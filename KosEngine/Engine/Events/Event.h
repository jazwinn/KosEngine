/*!
\file      Event.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      8 March, 2025
\brief     This file holds the declaration of the subscribe and trigger
		   function.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <vector>
#include <unordered_map>

namespace Event{

	enum EventType {


	};

	class EventSystem {
		//TURN TO SINGLETON?

	public:
		using EventListener = std::function<void()>; // function pointing to the "reaction" of the event

		void SubscribeEvent(EventType, EventListener listener);

		//void UnsubscribeEvent(EventType, EventListener listener);

		void TriggerEvent(EventType);

	private:
		//				Type of Event		Vector of "subscribers"
		std::unordered_map<EventType, std::vector<EventListener>> Listeners;

	};







}




#endif EVENT_H
