/******************************************************************/
/*!
\file      Component.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file contains the parent class that all componetns
		   will have to inherit from


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef COM_H
#define COM_H

#include "../Config/pch.h"
#include "../ECS/Component/ComponentList.h"
#include "../ECS/ECSList.h"
#include "../Asset Manager/Reflection.h"


namespace ecs {
/******************************************************************/
/*!
\class     Component
\brief     Parent class that every components inherits from
*/
/******************************************************************/
	class Component {

	public:

		//Entity is assigned to components
		bool m_IsLive{};

		//Store Entity ID
		EntityID m_Entity{};

		//store entity's scene
		std::string m_scene{};



	};

}
//parent class

#endif COM_H