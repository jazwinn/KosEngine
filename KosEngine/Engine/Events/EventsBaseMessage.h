/******************************************************************/
/*!
\file      BaseMessage.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the enum of the different types of messages we want to send 
		   along with the Message class which will hold that type and the string that we want to attach to that type

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "../Config/pch.h"

namespace messaging {
	enum class MessageType {
		NONE, AUDIOPLAY
	};

	class Message {
	public:
		Message(MessageType givenType, const std::string& givenData) : m_type(givenType), m_data(givenData){}
		const MessageType& m_GetType() const { return m_type; }
		const std::string& m_GetData() const { return m_data; }

	private:
		MessageType m_type;
		std::string m_data;
	};
}