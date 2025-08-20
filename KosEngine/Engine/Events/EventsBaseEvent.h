/*!
\file      BaseEvent.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Oct 17, 2024
\brief     This file holds the declaration and definitions for the enums that are used to type each event, along with the base
		   class for events to inherit from and the dispatch class which will handle the storing of the listeners to events along with
		   sending messages to each listener.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "../Config/pch.h"



namespace events {
	//enum class EventType {
	//	EVENTNONE = 0,
	//	EVENTBUTTONPRESS,
	//	EVENTKEYPRESS,EVENTKEYRELEASE,
	//	EVENTMOUSEPRESS,EVENTMOUSERELEASE
	//};

	enum class Actions {
		BASEACTION,
		ADDCOMP,
		REMOVECOMP,
		TRANSFORMCOMP,
		ADDIMAGE,
		ADDENT,
		DELENT,
		MOVECTC,
		MOVEPTC,
		MOVECTP,
		MODIFYANIM,
		MODIFYAUDIO,
		MODIFYCAMERA,
		MODIFYCOLL,
		MODIFYENEMY,
		MODIFYGRID,
		MODIFYLIGHT,
		MODIFYPATHFINDING,
		MODIFYRIGID,
		MODIFYSPRITE,
		MODIFYTEXT,
		ADDAUDIO,
		REMOVEAUDIO,
		MODIFYBUTTON,
		MODIFYVIDEO,
		MODIFYTILEMAP,
		MODIFYPARTICLE,
		MODIFYNAME,
		ADDRAY,
		REMOVERAY,
		UNDO,
		REDO
	};

	enum class ButtonEvents{
		EVENTBUTTONPRESS,
		EVENTAUDIOFROMIMGUI
	};

	enum class KeyEvents {
		EVENTKEYPRESS,
		EVENTKEYRELEASE
	};

	enum class MouseEvents {
		EVENTMOUSEPRESS,
		EVENTMOUSERELEASE
	};


	template<typename T>
	class BaseEvent {
	protected:
		T m_eventType;
		bool m_handled;
	public:
		BaseEvent(T givenType) : m_eventType{ givenType }, m_handled{ false } {}
		virtual ~BaseEvent() = default;
		inline const T m_GetEventType() const { return m_eventType; }
		template<typename U>
		inline U m_ToType()const {
			return static_cast<const U&>(*this);
		}
		virtual bool m_IsHandled() const{ return m_handled; }
	};


	template<typename T>
	class EventDispatcher {
	private:
		std::unordered_map<T, std::vector<std::function<void(const BaseEvent<T>&)>>> m_listeners;
		int m_nextID = 0;
		std::map<int, std::pair<T, typename std::vector<std::function<void(const BaseEvent<T>&)>>::iterator>> m_listenerIDs;
	public:
		int m_RegisterListener(T givenEvent, std::function<void(const BaseEvent<T>&)> givenCallback) {
			m_listeners[givenEvent].emplace_back(givenCallback);
			int retID = m_nextID++;
			m_listenerIDs[retID] = { givenEvent, std::prev(m_listeners[givenEvent].end()) };
			return retID;
		}

		void m_DispatchToListeners(const BaseEvent<T>& givenEvent [[maybe_unused]] ) {
			if (m_listeners.find(givenEvent.m_GetEventType()) == m_listeners.end()) {
				return;
			}

			for (auto&& currListener : m_listeners.at(givenEvent.m_GetEventType())) {
				if (!givenEvent.m_IsHandled()) {
					currListener(givenEvent);
				}
			}
		}

		void m_UnregisterListener(int ID) {
			auto pairInMap = m_listenerIDs.find(ID);
			if (pairInMap != m_listenerIDs.end()) {
				auto& currListenerInfo = pairInMap->second;
				m_listeners[currListenerInfo.first].erase(currListenerInfo.second);
				m_listenerIDs.erase(pairInMap);
			}
		}
	};
}