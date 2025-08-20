/******************************************************************/
/*!
\file      PathfindingSystem.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 29, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#pragma once
#pragma once
#ifndef PATHSYS_H
#define PATHSYS_H


#include "System.h"
#include "../ECS/ECSList.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs {

	class PathfindingSystem : public ISystem {

	private:


	public:

		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update(const std::string&) override;



	private:

		std::vector<TransformComponent*> m_vecTransformComponentPtr;

		std::vector<PathfindingComponent*> m_vecPathfindingComponentPtr;

		std::vector<NameComponent*> m_vecNameComponentPtr;
	};

}



#endif