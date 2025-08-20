/********************************************************************/
/*!
\file      Action.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Nov 12, 2024
\brief     This header file declares the necessary functions that any derived class must implement

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include <stack>
#include "../ECS/Component/Component.h"
#include "../ECS/ECSList.h"
#include "../ECS/ECS.h"

namespace actions {
	/******************************************************************/
	/*!
		\brief     This is a pure virtual class for any actions that we want to track for undoing or redoing
	*/
	/******************************************************************/
	class Action {
	public:
		virtual void m_UndoAction() = 0;
		virtual void m_RedoAction() = 0;
		virtual ~Action() = default;
	};
}
