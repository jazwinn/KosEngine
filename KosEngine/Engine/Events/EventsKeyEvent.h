/*!
\file      KeyEvent.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Oct 21, 2024
\brief     This file holds the declaration and definition of the specialised key events.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#pragma once
#include "EventsBaseEvent.h"

namespace events {
	class KeyPressedEvent : public BaseEvent<KeyEvents> {
	private:
		int m_keyPressed;
	public:
		KeyPressedEvent(int givenButton) : BaseEvent<KeyEvents>(KeyEvents::EVENTKEYPRESS), m_keyPressed(givenButton){};
	};

	class KeyReleasedEvent : public BaseEvent<KeyEvents> {
	private:
		int m_keyReleased;
	public:
		KeyReleasedEvent(int givenButton) : BaseEvent<KeyEvents>(KeyEvents::EVENTKEYRELEASE), m_keyReleased(givenButton){};
	};
}