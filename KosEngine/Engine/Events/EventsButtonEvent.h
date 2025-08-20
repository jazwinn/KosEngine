/*!
\file      ButtonEvent.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Oct 21, 2024
\brief     This file holds the declaration and definiion of the specialised button events

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#pragma once
#include "EventsBaseEvent.h"

namespace events {
	/******************************************************************/
	/*!
		\brief     This is a derived class from BaseEvent to hold what button is being pressed
	*/
	/******************************************************************/
	class ButtonPressEvent : public BaseEvent<ButtonEvents> {
	private:
		int m_button;
	public:
		ButtonPressEvent(int givenButton) : BaseEvent<ButtonEvents>(ButtonEvents::EVENTBUTTONPRESS), m_button{givenButton} {}
		int m_GetButton() { return m_button; }
	};

	/******************************************************************/
	/*!
		\brief     This is a derived class from BaseEvent to hold what audio file is being played
	*/
	/******************************************************************/
	class AudioFromImgui : public BaseEvent<ButtonEvents> {
	private:
		std::string m_file;
	public:
		AudioFromImgui(std::string givenFile) : BaseEvent<ButtonEvents>(ButtonEvents::EVENTAUDIOFROMIMGUI), m_file{ givenFile } {}
		std::string m_GetFile() { return m_file; }
	};
}