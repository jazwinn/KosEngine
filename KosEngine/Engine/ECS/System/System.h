/******************************************************************/
/*!
\file      System.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the system class that is the parent
		   class to all systems in the entity component system

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H


#include "../ECS/Component/Component.h"
#include "SystemType.h"

namespace ecs {

	class ISystem {

	public:

		compSignature m_SystemSignature; // set signature based on what component the systems need
		/******************************************************************/
		/*!
		\def       m_RegisterSystem(EntityID)
		\brief     checks entity map if entity meets system requirement.
				   If it does, it stores the pointer of the entity's component
		\param[in] EntiyID The id of the entity
		*/
		/******************************************************************/
		virtual void m_RegisterSystem(EntityID) = 0;
		/******************************************************************/
		/*!
		\def       m_DeregisterSystem(EntityID)
		\brief     Removes component pointer from the vector
		\param[in] EntiyID The id of the entity
		*/
		/******************************************************************/
		virtual void m_DeregisterSystem(EntityID) = 0;
		/******************************************************************/
		/*!
		\def       m_Init()
		\brief     Initialzie the System, set it's bitflag
		*/
		/******************************************************************/
		virtual void m_Init() = 0;
		/******************************************************************/
		/*!
		\def       m_Update(const std::string&)
		\brief     Retrieves component data and carry out the logic.
		*/
		/******************************************************************/
		virtual void m_Update(const std::string&) = 0;


	};

}









#endif SYSTEM_H

