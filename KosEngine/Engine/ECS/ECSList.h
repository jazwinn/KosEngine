/******************************************************************/
/*!
\file      ECSList.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains all the data that is frequently used in
		   other non-ECS systems.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef ECSLIST_H
#define ECSLIST_H
#include "../Config/pch.h"
#include "../ECS/Component/ComponentList.h"

namespace ecs {

	using EntityID = unsigned int;
	using compSignature = std::bitset<TOTALTYPECOMPONENT>;

	// Max number of entities allowed to be created
	static EntityID MaxEntity = 2000;

	enum GAMESTATE {
		START,
		RUNNING,
		STOP,
		WAIT,
		TERMINATE
	};

}



#endif ECSLIST_H