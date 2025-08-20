/*!
\file      EventsDragFloat.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      8 March, 2025
\brief     This header file declares and defines Member and Comp enums along with the drag.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "EventsDragFloat.h"

namespace dragfloat {
	std::unique_ptr<dragfloat::DragFloatCheck> dragfloat::DragFloatCheck::m_instance = nullptr;

	/******************************************************************/
	/*!
		\brief     This is a derived class from BaseEvent to get the instance of the dragFloatCheck class
	*/
	/******************************************************************/
	dragfloat::DragFloatCheck* dragfloat::DragFloatCheck::m_GetInstance() {
		if (!m_instance) {
			m_instance = std::make_unique<DragFloatCheck>();
		}
		return m_instance.get();
	}
}